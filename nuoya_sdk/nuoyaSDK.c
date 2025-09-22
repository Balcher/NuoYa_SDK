/*
 * @Description: 无锡诺亚自动化滚轴的控制
 * @Author: BiChunkai 321521004@qq.com
 * @Date: 2025-09-19 15:52:37
 * @LastEditTime: 2025-09-22 13:22:47
 * @FilePath: /NuoYa_SDK/nuoya_sdk/nuoyaSDK.c
 *
 * Copyright (c) 2025 by BiChunkai 321521004@qq.com, All Rights Reserved.
 */

#include "nuoyaSDK.h"
#include "rs232.h"
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>   // printf
#include <string.h>

int NuoyaSDK_Init(const char *port)
{
    comEnumerate();
    int port_index = comFindPort(port);
    if (port_index == -1)
    {
        return -1;
    }
    return port_index;
}

bool NuoyaSDK_Connect(int portIndex, int baudRate)
{
    if (!comOpen(portIndex, baudRate))
    {
        return false;
    }
    return true;
}

bool NuoyaSDK_Disconnect(int port)
{
    comClose(port);
    comTerminate();
    printf("Port %d disconnected\n", port);
    return true;
}

/**
 * @brief 读取完整一帧数据，超时返回已读取字节数
 * 
 * @param portIndex 端口索引
 * @param buf 接收数据缓存
 * @param len 接收数据长度
 * @param timeout_ms 超时时间（毫秒）
 * @return int 实际读取到的字节数
 */
int read_frame(int portIndex, unsigned char *buf, int len, int timeout_ms)
{
    int total = 0;
    struct timeval start, now;
    gettimeofday(&start, NULL);

    while (total < len)
    {
        int n = comRead(portIndex, (char*)buf + total, len - total);
        if (n > 0)
        {
            total += n;
        }
        else if (n == 0)
        {
            // 串口没有数据，等一小段时间再读
        }
        else
        {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
                return -1; // 读错误
        }

        // 超时检查
        gettimeofday(&now, NULL);
        long elapsed = (now.tv_sec - start.tv_sec) * 1000 +
                       (now.tv_usec - start.tv_usec) / 1000;
        if (elapsed >= timeout_ms)
            break;

        usleep(1000); // 避免 CPU 空转, 1ms 检查一次
    }

    return total;
}

DriverError NuoyaSDK_stop(int portIndex, int timeout_ms)
{
    unsigned char send_data[8] = {0x85, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    // printf("Sending stop: ");
    // for(int i=0;i<8;i++) printf("%02X ", send_data[i]);
    // printf("\n");

    comWrite(portIndex, (const char*)send_data, sizeof(send_data));

    unsigned char recv_data[4] = {0};
    int n = read_frame(portIndex, recv_data, sizeof(recv_data), timeout_ms);

    if (n == sizeof(recv_data))
    {
        // printf("Received frame: ");
        // for(int i=0;i<4;i++) printf("%02X ", recv_data[i]);
        // printf("\n");
        return (DriverError)recv_data[2]; // 第3字节错误码
    }
    else if (n == 0)
    {
        // printf("Connection closed by peer\n");
        return ERROR_CONNECTION_CLOSED;
    }
    else if (n < 0)
    {
        printf("Read error: %s\n", strerror(errno));
        return ERROR_READ_FAILED;
    }
    else
    {
        // printf("Timeout: only received %d bytes\n", n);
        return ERROR_TIMEOUT;
    }
}

DriverError NuoyaSDK_rotate(int portIndex, int dir, int speed, int timeout_ms)
{
    if(speed < 0) speed = 0;
    if(speed > 127) speed = 127;

    unsigned char send_data[8] = {0};
    send_data[0] = 0x85;
    send_data[1] = ((dir & 0x01) << 6) | 0x01;
    send_data[2] = speed & 0x7F;
    send_data[3] = 0x00;
    send_data[4] = send_data[5] = send_data[6] = 0x00;
    send_data[7] = send_data[1] ^ send_data[2];

    // printf("Sending rotate: ");
    // for(int i=0;i<8;i++) printf("%02X ", send_data[i]);
    // printf("\n");

    comWrite(portIndex, (const char*)send_data, sizeof(send_data));

    unsigned char recv_data[4] = {0};
    int n = read_frame(portIndex, recv_data, sizeof(recv_data), timeout_ms);

    if(n == sizeof(recv_data))
    {
        // printf("Received frame: ");
        // for(int i=0;i<4;i++) printf("%02X ", recv_data[i]);
        // printf("\n");
        return (DriverError)recv_data[2]; // 第3字节错误码
    }
    else if(n == 0)
    {
        // printf("Connection closed by peer\n");
        return ERROR_CONNECTION_CLOSED;
    }
    else if(n < 0)
    {
        printf("Read error: %s\n", strerror(errno));
        return ERROR_READ_FAILED;
    }
    else
    {
        // printf("Timeout: only received %d bytes\n", n);
        return ERROR_TIMEOUT;
    }
}

void NuoyaSDK_runControl(int portIndex)
{
    unsigned char send_data[8] = {0x8A, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

    comWrite(portIndex, (const char*)send_data, sizeof(send_data)); 
}

///////////////////////////////////////////////////////////////////
                        //  第一版用循环查看接受情况
///////////////////////////////////////////////////////////////////
// bool Nuoya_move(int portIndex, int x, int y)
// {
//     char cmd[100];
//     snprintf(cmd, sizeof(cmd), "MOVE %d %d", x, y);
//     return comWrite(portIndex, cmd, strlen(cmd));
// }

// DriverError NuoyaSDK_stop(int portIndex)
// {
//     unsigned char send_data[8] = {0x85, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
//     comWrite(portIndex, (const char*)send_data, sizeof(send_data));

//     unsigned char recv_data[8];
//     memset(recv_data, 0, sizeof(recv_data));

//     int bytes_read = 0;
//     for (int attempt = 0; attempt < 10; attempt++) {    // 利用循环，容易在循环期间读不到
//         int n = comRead(portIndex, (char*)recv_data, sizeof(recv_data));
//         if (n > 0) {
//             bytes_read = n;
//             break;
//         }
//     }

//     if (bytes_read > 0) {
//         printf("Received %d bytes: ", bytes_read);
//         for (int i = 0; i < bytes_read; i++) {
//             printf("%02X ", recv_data[i]);
//         }
//         printf("\n");

//         // 直接返回 byte3 错误码（可能叠加多个错误位）
//         return (DriverError)recv_data[3];
//     } else {
//         printf("No data received\n");
//         return ERROR_NONE;
//     }
// }

// DriverError NuoyaSDK_rotate(int portIndex, int dir, int speed)
// {
//     if (speed < 0)
//         speed = 0;
//     if (speed > 127)
//         speed = 127; // 协议低7位最大127

//     unsigned char send_data[8] = {0};

//     // 1. 起始符号
//     send_data[0] = 0x85;

//     // 2. 方向/地址，地址固定为1
//     send_data[1] = ((dir & 0x01) << 6) | 0x01; // B6=方向，B5-B0=地址低6位=1

//     // 3. 运行速度低7位
//     send_data[2] = speed & 0x7F;

//     // 4. 运行速度高1位
//     send_data[3] = 0x00; // 本例忽略高位

//     // 5-7 预留/扩展
//     send_data[4] = 0x00;
//     send_data[5] = 0x00;
//     send_data[6] = 0x00;

//     // 8. 校验符 = byte2 XOR byte3
//     send_data[7] = send_data[1] ^ send_data[2];

//     // 发送命令
//     comWrite(portIndex, (const char *)send_data, sizeof(send_data));

//     // 接收返回
//     unsigned char recv_data[8];
//     memset(recv_data, 0, sizeof(recv_data));

//     int bytes_read = 0;
//     for (int attempt = 0; attempt < 100; attempt++)
//     {
//         int n = comRead(portIndex, (char *)recv_data, sizeof(recv_data));
//         if (n > 0)
//         {
//             bytes_read = n;
//             break;
//         }
//     }

//     if (bytes_read > 0)
//     {
//         printf("Received %d bytes: ", bytes_read);
//         for (int i = 0; i < bytes_read; i++)
//         {
//             printf("%02X ", recv_data[i]);
//         }
//         printf("\n");

//         return (DriverError)recv_data[2]; // byte3 错误码
//     }
//     else
//     {
//         printf("No data received\n");
//         return ERROR_NONE;
//     }
// }