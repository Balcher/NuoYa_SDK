# 控制无锡诺亚自动化的滚轴设备通信软件工具包
兼容C/C++，针对V2-20HC3-7版本的驱动卡软件

## 功能描述
1. 初始化串口并搜索设备
2. 打开/关闭串口
3. 启动滚轴正转或反转并设置速度
4. 停止滚轴
5. 获取返回错误码，支持多种错误解析
*. 未提供设置驱动卡地址功能，默认地址为0x01

## 主要接口功能
- 初始化端口：`int NuoyaSDK_Init(const char * port);` 【通信波特率38400】
- 连接端口：`bool NuoyaSDK_Connect(int portIndex, int baudRate);`
- 断开端口：`bool NuoyaSDK_Disconnect(int portIndex);`
- 停止滚轴：`DriverError NuoyaSDK_stop(int portIndex, int timeout_ms);`
- 滚轴旋转：`DriverError NuoyaSDK_rotate(int portIndex, int dir, int speed, int timeout_ms);`

注：
1. 发送控制指令后，驱动卡有应答报文，返回错误码为0x00时表示成功，其他值表示错误，错误码详细说明见下方错误码章节。超时时间表示接收报文的超时。
2. 滚轴的速度变量计算方法为：速度值*10*2/辊筒减速比rpm

## 错误码
允许存在多个错误码，每个错误码对应一个位
```C
typedef enum {
    ERROR_NONE               = 0x00, // 无错误
    ERROR_MOTOR_FAIL         = 1 << 5, // B5 电机运作失败
    ERROR_PARAM_NO_ACTION    = 1 << 4, // B4 参数之前无动作
    ERROR_CMD_NO_PARAM       = 1 << 3, // B3 动作指令前无参数
    ERROR_HALL               = 1 << 2, // B2 霍尔错误
    ERROR_OVERCURRENT        = 1 << 1, // B1 过流保护

    ERROR_COMMUNICATION      = 1 << 7, // B7 通信错误
    ERROR_TIMEOUT            = 1 << 6, // B6 超时错误
    ERROR_INVALID_RESPONSE   = 1 << 0, // B0 无效响应
    ERROR_READ_FAILED        = ERROR_COMMUNICATION, // 读取失败
    ERROR_CONNECTION_CLOSED  = ERROR_COMMUNICATION  // 连接关闭
} DriverError;

```

