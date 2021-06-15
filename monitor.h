#ifndef MONITOR_H
#define MONITOR_H

#include <stdint.h>
#include <stddef.h>
#include <dbus/dbus.h>

/***********************************************************************************
 * @brief 申请成为监视器
 * @param[in] conn DBus连接
 * @param[in] filter 过滤规则
 * @param[in] len filter的长度
 * @param[out] err DBus错误消息
 * *********************************************************************************/
void become_monitor(DBusConnection* conn, char* filter[], size_t len, DBusError* err);

#endif // DBUS_MONITOR_H