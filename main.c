
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <dbus/dbus.h>
#include "monitor.h"
#include "message.h"
#include "message_cache.h"
#include "proc.h"

static DBusHandlerResult log_message(DBusConnection* conn, DBusMessage* msg, void* userdata);

bool running = true;
void handle_exit_signal(int signum)
{
    (void)(signum);
    running = false;
}

int main()
{
    // 使用行缓冲，方便通过管道处理
    setvbuf(stdout, NULL, _IOLBF, 128);

    //  建立连接
    DBusError err;
    dbus_error_init(&err);

    // 成为监视器
    DBusConnection* conn = become_monitor(NULL, 0, &err);
    if(conn == NULL || dbus_error_is_set(&err))
    {
        fprintf(stderr, "%s:%s\n", err.name, err.message);
        return EXIT_FAILURE;
    }

    // 创建函数调用的缓存
    message_cache_t* cache = message_cache_new(64);

    // 设置监视回调函数
    dbus_connection_add_filter(conn, log_message, cache, NULL);
    
    // 绑定信号
    signal(SIGINT, handle_exit_signal);
    signal(SIGTERM, handle_exit_signal);
    signal(SIGHUP, handle_exit_signal);

    // DBus主循环
    running = true;
    while(running && dbus_connection_read_write_dispatch(conn, -1));
    message_cache_free(cache);
    dbus_connection_close(conn);
    dbus_connection_unref(conn);
}

/***********************************************************************************
 * @brief DBus filter 回调函数
 * @param[in] conn DBus连接
 * @param[in] msg DBus消息
 * @param[in] userdata 用户数据
 * *********************************************************************************/
static DBusHandlerResult log_message(DBusConnection* conn, DBusMessage* msg, void* userdata)
{
    (void)(conn);
    message_cache_t* cache = userdata;

    if(dbus_message_is_signal(msg, DBUS_INTERFACE_LOCAL, "Disconnected"))
    {
        running = false;
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    int msg_type = dbus_message_get_type(msg);
    switch(msg_type)
    {
        case DBUS_MESSAGE_TYPE_METHOD_CALL: 
        {
            // 复制消息
            dbus_uint32_t serial = dbus_message_get_serial(msg);
            DBusMessage* copy = dbus_message_copy(msg);
            dbus_message_set_serial(copy, serial);
            // 存入缓存
            message_cache_set(cache, copy);
            break;
        }

        case DBUS_MESSAGE_TYPE_METHOD_RETURN:
        {
            dbus_uint32_t serial = dbus_message_get_reply_serial(msg);
            message_cache_remove(cache, serial);
            break;
        }

        case DBUS_MESSAGE_TYPE_ERROR:
        {
            // 查找缓存
            dbus_uint32_t serial = dbus_message_get_reply_serial(msg);
            DBusMessage* saved = message_cache_get(cache, serial);
            if(saved != NULL)
            {
                const char* sender = dbus_message_get_sender(saved);
                const char* destination = dbus_message_get_destination(saved);
                uint32_t sender_pid = proc_get_pid_by_dbus(sender);
                uint32_t destination_pid = proc_get_pid_by_dbus(destination);
                char exe[PATH_MAX+1];
                proc_get_exec_path(sender_pid, exe);
                printf("\nsender: PID(%u) NAME(%s) EXE(%s)\n", sender_pid, sender, exe);
                proc_get_exec_path(destination_pid, exe);
                printf("destination: PID(%u) NAME(%s) EXE(%s)\n", destination_pid, destination, exe);
                printf("details:\n >>> ");
                print_message(saved);
                printf(" <<< ");
                print_message(msg);
            }
            else
            {
                print_message(msg);
            }
        }
    }

    return DBUS_HANDLER_RESULT_HANDLED;
}