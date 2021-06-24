#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message_cache.h"

/***********************************************************************************
 * @brief 创建一个缓冲区
 * @param[in] element_size 一个元素的大小
 * @param[in] length 元素的数量
 * @return 缓冲期
 * *********************************************************************************/
message_cache_t* message_cache_new(size_t length)
{
    message_cache_t* cache = malloc(sizeof(message_cache_t));
    if(cache == NULL)
    {
        fprintf(stderr, "bad alloc\n");
        return NULL;
    }

    cache->length = length;
    cache->messages = malloc(sizeof(DBusMessage*) * length);
    if(cache->messages != NULL)
    {
        memset(cache->messages, 0, sizeof(DBusMessage*) * length);
    }

    return cache;
}

/***********************************************************************************
 * @brief 释放一个缓冲区
 * @param[in] cache 要释放的缓冲区
 * *********************************************************************************/
void message_cache_free(message_cache_t* cache)
{
    if(cache != NULL && cache->messages != NULL)
    {
        for(size_t i = 0; i < cache->length; i++)
        {
            DBusMessage* msg = cache->messages[i];
            if(msg != NULL)
            {
                dbus_message_unref(msg);
                cache->messages[i] = NULL;
            }
        }
        free(cache->messages);
        cache->messages = NULL;
    }

    if(cache != NULL)
        free(cache);
}

/***********************************************************************************
 * @brief 从缓冲区中删除一个数据
 * @param[in] cache 缓冲区
 * @param[in] serial 调用的序列号
 * *********************************************************************************/
void message_cache_remove(message_cache_t* cache, size_t serial)
{
    if(cache == NULL)
        return;

    size_t index = serial % cache->length;
    DBusMessage* saved = cache->messages[index];
    if(saved != NULL)
    {
        dbus_message_unref(saved);
        cache->messages[index] = NULL;
    }
}

/***********************************************************************************
 * @brief 从缓冲区中读取一个数据
 * @param[in] cache 缓冲区
 * @param[in] serial 调用的序列号
 * @return DBus调用的消息
 * *********************************************************************************/
DBusMessage* message_cache_get(message_cache_t* cache, size_t serial)
{
    if(cache == NULL)
        return NULL;

    size_t index = serial % cache->length;
    return cache->messages[index];
}

/***********************************************************************************
 * @brief 向缓冲区中写入一个数据
 * @param[in] cache 缓冲区
 * @param[in] message DBus调用的消息
 * *********************************************************************************/
void message_cache_set(message_cache_t* cache, DBusMessage* message)
{
    if(cache == NULL)
        return;

    size_t serial = dbus_message_get_serial(message);
    size_t index = serial % (cache->length);
    DBusMessage* saved = cache->messages[index];
    if(saved != NULL)
    {
        dbus_message_unref(saved);
    }
    cache->messages[index] = message;
}