#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdbool.h>
#include <dbus/dbus.h>

/***********************************************************************************
 * @brief 打印DBus消息
 * @param[in] message DBus消息
 * *********************************************************************************/
void print_message(DBusMessage* message);

/***********************************************************************************
 * @brief 格式化字符串
 * @param[out] str 保存结果的字符串
 * @param[inout] max str的可用空间长度(不含结尾的0),如果不够，则返回扩展后的长度(不含结尾的0)
 * @param[in] fmt 格式化字符串
 * @return is success
 * *********************************************************************************/
bool snprintf_alloc(char** str, size_t* max, const char* fmt, ...);

/***********************************************************************************
 * @brief 解析DBus消息参数
 * @param[in] iter DBus消息参数的第一个迭代器
 * @param[in] depth 递归深度
 * @return 参数字符串，需要使用free释放
 * *********************************************************************************/
char* decode_message_iter(DBusMessageIter* iter, int depth);

/***********************************************************************************
 * @brief 解析DBus消息
 * @param[in] message DBus消息
 * @return 消息字符串，需要使用free释放
 * *********************************************************************************/
char* decode_message(DBusMessage* message);

#endif // MESSAGE_H