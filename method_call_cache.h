#ifndef CACHE_H
#define CACHE_H

#include <stddef.h>
#include <dbus/dbus.h>

typedef struct MethodCallCache
{
    DBusMessage** messages;
    size_t length;
}MethodCallCache;

/***********************************************************************************
 * @brief 创建一个缓冲区
 * @param[in] element_size 一个元素的大小
 * @param[in] length 元素的数量
 * @return 缓冲期
 * *********************************************************************************/
MethodCallCache* new_method_call_cache(size_t length);

/***********************************************************************************
 * @brief 释放一个缓冲区
 * @param[in] cache 要释放的缓冲区
 * *********************************************************************************/
void free_method_call_cache(MethodCallCache* cache);

/***********************************************************************************
 * @brief 从缓冲区中读取一个数据
 * @param[in] cache 缓冲区
 * @param[in] serial 调用的序列号
 * @return DBus调用的消息
 * *********************************************************************************/
DBusMessage* get_method_call_cache(MethodCallCache* cache, size_t serial);

/***********************************************************************************
 * @brief 向缓冲区中写入一个数据
 * @param[in] cache 缓冲区
 * @param[in] message DBus调用的消息
 * *********************************************************************************/
void set_method_call_cache(MethodCallCache* cache, DBusMessage* message);


#endif // CACHE_H