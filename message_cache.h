#ifndef CACHE_H
#define CACHE_H

#include <stddef.h>
#include <dbus/dbus.h>

typedef struct message_cache_t
{
    DBusMessage** messages;
    size_t length;
}message_cache_t;

/***********************************************************************************
 * @brief 创建一个缓冲区
 * @param[in] element_size 一个元素的大小
 * @param[in] length 元素的数量
 * @return 缓冲期
 * *********************************************************************************/
message_cache_t* message_cache_new(size_t length);

/***********************************************************************************
 * @brief 释放一个缓冲区
 * @param[in] cache 要释放的缓冲区
 * *********************************************************************************/
void message_cache_free(message_cache_t* cache);

/***********************************************************************************
 * @brief 从缓冲区中删除一个数据
 * @param[in] cache 缓冲区
 * @param[in] serial 调用的序列号
 * *********************************************************************************/
void message_cache_remove(message_cache_t* cache, size_t serial);

/***********************************************************************************
 * @brief 从缓冲区中读取一个数据
 * @param[in] cache 缓冲区
 * @param[in] serial 调用的序列号
 * @return DBus调用的消息
 * *********************************************************************************/
DBusMessage* message_cache_get(message_cache_t* cache, size_t serial);

/***********************************************************************************
 * @brief 向缓冲区中写入一个数据
 * @param[in] cache 缓冲区
 * @param[in] message DBus调用的消息
 * *********************************************************************************/
void message_cache_set(message_cache_t* cache, DBusMessage* message);


#endif // CACHE_H