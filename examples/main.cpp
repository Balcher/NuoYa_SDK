/*
 * @Description: 诺亚自动化sdk测试
 * @Author: BiChunkai 321521004@qq.com
 * @Date: 2025-09-19 15:06:22
 * @LastEditTime: 2025-09-22 10:26:44
 * @FilePath: /NuoYa_SDK/examples/main.cpp
 * 
 * Copyright (c) 2025 by BiChunkai 321521004@qq.com, All Rights Reserved. 
 */
#include <iostream>
#include <cstring>      // for memset
#include "nuoyaSDK.h"
#include <stdio.h>   // printf
#include <string.h>
#include <unistd.h>

int main() {
    const char* port_name = "ttyUSB0"; // 直接指定端口
    int baudrate = 38400;

    // 1. 初始化端口
    int port_index = NuoyaSDK_Init(port_name);
    if (port_index == -1) {
        std::cout << "Failed to init port " << port_name << std::endl;
        return -1;
    }

    sleep(1); // 延迟1秒


    // 2. 连接端口
    if (!NuoyaSDK_Connect(port_index, baudrate)) {
        std::cout << "Failed to connect port " << port_name << std::endl;
        return -1;
    }
    std::cout << "Port " << port_name << " opened at " << baudrate << " baud" << std::endl;

    sleep(1); // 延迟1秒


    // 3. 发送运动指令
    DriverError RotateErr = NuoyaSDK_rotate(port_index, 0, 100, 2000); // 正转，速度100
    if (RotateErr == ERROR_NONE) {
        std::cout << "Rotate successful, no errors." << std::endl;
} else {
    std::cout << "Stop failed with errors:" << std::endl;

    if (RotateErr & ERROR_MOTOR_FAIL)       std::cout << "- Motor operation failed" << std::endl;
    if (RotateErr & ERROR_PARAM_NO_ACTION)  std::cout << "- Previous parameter had no action" << std::endl;
    if (RotateErr & ERROR_CMD_NO_PARAM)     std::cout << "- Command received without parameter" << std::endl;
    if (RotateErr & ERROR_HALL)             std::cout << "- Hall signal error" << std::endl;
    if (RotateErr & ERROR_OVERCURRENT)      std::cout << "- Phase overcurrent" << std::endl;
    if (RotateErr & ERROR_COMMUNICATION)    std::cout << "- Communication error" << std::endl;
    if (RotateErr & ERROR_TIMEOUT)          std::cout << "- Timeout error" << std::endl;
    if (RotateErr & ERROR_INVALID_RESPONSE) std::cout << "- Invalid response" << std::endl;
    if (RotateErr == ERROR_READ_FAILED)       std::cout << "- Read failed" << std::endl;
    if (RotateErr == ERROR_CONNECTION_CLOSED) std::cout << "- Connection closed" << std::endl;
}


    sleep(1); // 延迟1秒

    // 4. 发送停止指令
    DriverError err = NuoyaSDK_stop(port_index, 2000);
    if (err == ERROR_NONE) {
        std::cout << "Stop successful, no errors." << std::endl;
} else {
    std::cout << "Stop failed with errors:" << std::endl;

    if (err & ERROR_MOTOR_FAIL)       std::cout << "- Motor operation failed" << std::endl;
    if (err & ERROR_PARAM_NO_ACTION)  std::cout << "- Previous parameter had no action" << std::endl;
    if (err & ERROR_CMD_NO_PARAM)     std::cout << "- Command received without parameter" << std::endl;
    if (err & ERROR_HALL)             std::cout << "- Hall signal error" << std::endl;
    if (err & ERROR_OVERCURRENT)      std::cout << "- Phase overcurrent" << std::endl;
    if (err & ERROR_COMMUNICATION)    std::cout << "- Communication error" << std::endl;
    if (err & ERROR_TIMEOUT)          std::cout << "- Timeout error" << std::endl;
    if (err & ERROR_INVALID_RESPONSE) std::cout << "- Invalid response" << std::endl;
    if (err == ERROR_READ_FAILED)       std::cout << "- Read failed" << std::endl;
    if (err == ERROR_CONNECTION_CLOSED) std::cout << "- Connection closed" << std::endl;
}

    sleep(1); // 延迟1秒


    // 5. 关闭端口
    if (!NuoyaSDK_Disconnect(port_index)) {
        std::cout << "Failed to disconnect port " << port_name << std::endl;
        return -1;
    }

    return 0;
}