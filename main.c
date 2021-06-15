
#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include "monitor.h"
#include "message.h"
#include "method_call_cache.h"

static DBusHandlerResult log_message(DBusConnection* conn, DBusMessage* msg, void* userdata);
static void free_userdata(void* userdata);

int main()
{
    //  建立连接
    DBusError err;
    dbus_error_init(&err);
    DBusConnection* conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if(conn == NULL)
    {
        fprintf(stderr, "%s:%s\n", err.name, err.message);
        return EXIT_FAILURE;
    }

    // 成为监视器
    become_monitor(conn, NULL, 0, &err);
    if(dbus_error_is_set(&err))
    {
        fprintf(stderr, "%s:%s\n", err.name, err.message);
        return EXIT_FAILURE;
    }

    // 创建函数调用的缓存
    MethodCallCache* cache = new_method_call_cache(64);

    // 设置监视回调函数
    dbus_connection_add_filter(conn, log_message, cache, (void(*)(void*))free_method_call_cache);
    
    while(dbus_connection_read_write_dispatch(conn, -1));
}

/***********************************************************************************
 * @brief DBus filter 回调函数
 * @param[in] conn DBus连接
 * @param[in] msg DBus消息
 * @param[in] userdata 用户数据
 * *********************************************************************************/
static DBusHandlerResult log_message(DBusConnection* conn, DBusMessage* msg, void* userdata)
{
    MethodCallCache* cache = userdata;

    if(dbus_message_is_signal(msg, DBUS_INTERFACE_LOCAL, "Disconnected"))
    {
        free_method_call_cache(cache);
        exit (0);
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
            set_method_call_cache(cache, copy);
            break;
        }

    case DBUS_MESSAGE_TYPE_ERROR:
        {
            // 查找缓存
            dbus_uint32_t serial = dbus_message_get_reply_serial(msg);
            DBusMessage* saved = get_method_call_cache(cache, serial);
            if(saved != NULL)
            {
                print_message(saved);
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