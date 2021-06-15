#include "message.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <string.h>


/***********************************************************************************
 * @brief 打印DBus消息
 * @param[in] message DBus消息
 * *********************************************************************************/
void print_message(DBusMessage* message)
{

    char* str = decode_message(message);
    printf("%s\n", str);
    free(str);
}

/***********************************************************************************
 * @brief 内存复制
 * @param[in] src 源数据
 * @param[in] len 复制的字节数
 * @return 复制的数据，需要free释放
 * *********************************************************************************/
static char* copy(const void* src, size_t len)
{
    if(src == NULL || len == 0)
        return NULL;

    char* dst = malloc(len);
    memcpy(dst, src, len);
    return dst;
}

/***********************************************************************************
 * @brief 缩进
 * @param[out] str 返回生成的字符串,必须是NULL或者是可以free的内存块
 * @param[inout] max str的可用空间长度(含结尾的0),如果不够，则返回扩展后的长度(含结尾的0)
 * @param[in] depth 缩进深度
 * @return 复制的数据，需要free释放
 * *********************************************************************************/
static char* indent(char** str, size_t* max, int depth)
{
    // tab
    for(int i = 0 ; i < depth; i++)
    {
        snprintf_alloc(str, max, "%s  ", *str);
    }
}

/***********************************************************************************
 * @brief 格式化字符串
 * @param[out] str 返回生成的字符串,必须是NULL或者是可以free的内存块
 * @param[inout] max str的可用空间长度(含结尾的0),如果不够，则返回扩展后的长度(含结尾的0)
 * @param[in] fmt 格式化字符串
 * @return 是否成功
 * *********************************************************************************/
bool snprintf_alloc(char** str, size_t* max, const char* fmt, ...)
{
    size_t n = (*str == NULL || max == NULL) ? 0 : *max;
    char* dst = copy(*str, n);
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(dst, n, fmt, args);
    if(len > n)
    {
        char* m = realloc(dst, len + 1);
        if(m == NULL)
        {
            fprintf(stderr, "bad alloc\n");
            false;
        }
        dst = m;
        *max = len + 1;

        va_start(args, fmt);
        vsnprintf(dst, *max, fmt, args);
        va_end(args);
    }

    if(*str != NULL)
        free(*str);
    *str = dst;

    return true;
}

/***********************************************************************************
 * @brief 解析DBus消息
 * @param[in] message DBus消息
 * @return 消息字符串，需要使用free释放
 * *********************************************************************************/
char* decode_message(DBusMessage* message)
{
    int message_type = dbus_message_get_type (message);
    const char* sender = dbus_message_get_sender (message);
    const char* destination = dbus_message_get_destination (message);
    
    size_t len = 1024; 
    char* str = malloc(len);
    if(str == NULL)
    {
        fprintf(stderr, "bad alloc\n");
        return NULL;
    }

    snprintf_alloc(&str, &len, "%s time=%lu sender=%s -> destination=%s",
            dbus_message_type_to_string(message_type), 
            time(NULL), sender, destination);
    
    switch (message_type)
    {
    case DBUS_MESSAGE_TYPE_METHOD_CALL:
    case DBUS_MESSAGE_TYPE_SIGNAL:
        snprintf_alloc(&str, &len, "%s serial=%u path=%s interface=%s member=%s\n",
                str,
                dbus_message_get_serial (message),
                dbus_message_get_path (message),
                dbus_message_get_interface (message),
                dbus_message_get_member (message));
        break;
      
    case DBUS_MESSAGE_TYPE_METHOD_RETURN:
        snprintf_alloc(&str, &len, "%s serial=%u reply_serial=%u\n",
                str,
                dbus_message_get_serial (message),
                dbus_message_get_reply_serial (message));
        break;

    case DBUS_MESSAGE_TYPE_ERROR:
        snprintf_alloc(&str, &len, "%s error_name=%s reply_serial=%u\n",
                str,
                dbus_message_get_error_name (message),
                dbus_message_get_reply_serial (message));
        break;

    default:
        snprintf_alloc(&str, &len, "%s\n", str);
        break;
    }

    DBusMessageIter iter;
    dbus_message_iter_init(message, &iter);
    char* args = decode_message_iter(&iter, 0);
    snprintf_alloc(&str, &len, "%s%s", str, args);
    free(args);
    return str;
}

/***********************************************************************************
 * @brief 解析DBus消息参数
 * @param[in] iter DBus消息参数的第一个迭代器
 * @return 参数字符串，需要使用free释放
 * *********************************************************************************/
char* decode_message_iter(DBusMessageIter* iter, int depth)
{
    size_t len = 1024;
    char* str = malloc(len);
    str[0] = 0;
    if(str == NULL)
    {
        fprintf(stderr, "bac alloc\n");
        return NULL;
    }
    do
    {
        size_t arg_len = 64;
        char* arg = malloc(arg_len);
        arg[0] = 0;
        if(arg == NULL)
        {
            break;
        }
        int t = dbus_message_iter_get_arg_type(iter);

        switch(t)
        {
            case DBUS_TYPE_STRING:
            {
                char *val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sstring: \"%s\"\n", arg, val);
                break;
            }
            case DBUS_TYPE_SIGNATURE:
            {
                char *val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%ssignature: \"%s\"\n", arg, val);
                break;
            }
            case DBUS_TYPE_OBJECT_PATH:
            {
                char *val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sobject_path: \"%s\"\n", arg, val);
                break;
            }
            case DBUS_TYPE_INT16:
            {
                dbus_int16_t val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sint16: %d\n", arg, val);
                break;
            }
            case DBUS_TYPE_UINT16:
            {
                dbus_uint16_t val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%suint16: %u\n", arg, val);
                break;
            }
            case DBUS_TYPE_INT32:
            {
                dbus_int32_t val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sint32: %d\n", arg, val);
                break;
            }
            case DBUS_TYPE_UINT32:
            {
                dbus_uint32_t val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%suint32: %u\n", arg, val);
                break;
            }
            case DBUS_TYPE_INT64:
            {
                dbus_int64_t val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sint64: %ld\n", arg, val);
                break;
            }
            case DBUS_TYPE_UINT64:
            {
                dbus_uint64_t val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%suint64: %lu\n", arg, val);
                break;
            }
            case DBUS_TYPE_DOUBLE:
            {
                double val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sdouble: %lf\n", arg, val);
                break;
            }
            case DBUS_TYPE_BYTE:
            {
                uint8_t val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sbyte: %u\n", arg, val);
                break;
            }
            case DBUS_TYPE_BOOLEAN:
            {
                dbus_bool_t val;
                dbus_message_iter_get_basic(iter, &val);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sboolean: %s\n", arg, val ? "true" : "false");
                break;
            }
            case DBUS_TYPE_VARIANT:
            {
                DBusMessageIter subiter;
                dbus_message_iter_recurse(iter, &subiter);
                indent(&arg, &arg_len, depth);
                char* substr = decode_message_iter(&subiter, depth+1);
                snprintf_alloc(&arg, &arg_len, "%svartian: %s", arg, substr);
                free(substr);
                break;
            }
            case DBUS_TYPE_STRUCT:
            {
                DBusMessageIter subiter;
                dbus_message_iter_recurse (iter, &subiter);
                char* substr = decode_message_iter(&subiter, depth+1);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sstruct: {\n%s", arg, substr);
                free(substr);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%s}\n", arg);
                break;
            }
            case DBUS_TYPE_ARRAY:
            {
                DBusMessageIter subiter;
                dbus_message_iter_recurse (iter, &subiter);
                char* substr = decode_message_iter(&subiter, depth+1);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sarray: [\n%s", arg, substr);
                free(substr);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%s]\n", arg);
                break;
            }
            case DBUS_TYPE_DICT_ENTRY:
            {
                DBusMessageIter subiter;
                dbus_message_iter_recurse (iter, &subiter);
                char* substr = decode_message_iter(&subiter, depth+1);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%sdict_entry: (\n%s", arg, substr);
                free(substr);
                indent(&arg, &arg_len, depth);
                snprintf_alloc(&arg, &arg_len, "%s)\n", arg);
                break;
            }
            
        }

        snprintf_alloc(&str, &len, "%s%s", str, arg);
        free(arg);
    }while(dbus_message_iter_next(iter));

    return str;
}