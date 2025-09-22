/*
 * @Description: 无锡诺亚自动化滚轴的控制
 * @Author: BiChunkai 321521004@qq.com
 * @node: 兼容C、C++，linux版本
 * @Date: 2025-09-19 15:52:37
 * @LastEditTime: 2025-09-22 13:22:31
 * @FilePath: /NuoYa_SDK/nuoya_sdk/nuoyaSDK.h
 * 
 * Copyright (c) 2025 by BiChunkai 321521004@qq.com, All Rights Reserved. 
 */

#ifndef NUOYASDK_H
#define NUOYASDK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    ERROR_NONE               = 0x00, // B7-B6 = 0, B0 = 0
    ERROR_MOTOR_FAIL         = 1 << 5, // B5 电机运作失败
    ERROR_PARAM_NO_ACTION    = 1 << 4, // B4 参数之前无动作
    ERROR_CMD_NO_PARAM       = 1 << 3, // B3 动作指令前无参数
    ERROR_HALL               = 1 << 2, // B2 霍尔错误
    ERROR_OVERCURRENT        = 1 << 1,  // B1 过流保护

    // 在B7-B6保留为 0基础上，利用空闲 添加通信相关的错误码
    ERROR_COMMUNICATION      = 1 << 7, // B7 通信错误
    ERROR_TIMEOUT            = 1 << 6, // B6 超时错误
    ERROR_INVALID_RESPONSE   = 1 << 0, // B0 无效响应（如果B0确实可用）
    ERROR_READ_FAILED        = ERROR_COMMUNICATION,
    ERROR_CONNECTION_CLOSED  = ERROR_COMMUNICATION
} DriverError;

/**
 * @brief port initialization and search the port
 * 
 * @param port port name
 * @return port index, -1 fail
 */
int NuoyaSDK_Init(const char * port);

/**
 * @brief connect port
 * 
 * @param portIndex port index
 * @param baudRate baud rate
 * @return true connect success
 * @return false fail connect
 */
bool NuoyaSDK_Connect(int portIndex, int baudRate);

/**
 * @brief disconnect port
 * 
 * @param portIndex port index
 * @return true disconnect success
 * @return false fail disconnect
 */
bool NuoyaSDK_Disconnect(int portIndex);

/**
 * @brief stop the robot
 * 
 * @param portIndex port index
 * @param timeout timeout in ms
 * @return DriverError error code
 */
DriverError NuoyaSDK_stop(int portIndex, int timeout_ms);

/**
 * @brief rotate the robot
 * 
 * @param portIndex port index
 * @param dir direction, 0: 正转, 1: 反转【从接线地方参考，正转逆时针，反转顺时针】
 * @param speed speed, 速度值*10*2/辊筒减速比rpm
 * @return DriverError error code
 */
DriverError NuoyaSDK_rotate(int portIndex, int dir, int speed, int timeout_ms);

/**
 * @brief send control message
 * 
 * @param portIndex 
 */
void NuoyaSDK_runControl(int portIndex);


#ifdef __cplusplus
}
#endif
#endif