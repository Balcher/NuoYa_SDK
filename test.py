import serial

# 打开串口
ser = serial.Serial(
    port='/dev/ttyUSB0',   # 串口设备
    baudrate=38400,        # 波特率
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=1              # 读超时时间1秒
)

if ser.is_open:
    print("串口已打开")

# 要发送的数据（16进制）
data_to_send = bytes([0x85, 0x41, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x3E])

# 发送数据
ser.write(data_to_send)
print(f"已发送: {data_to_send.hex()}")

# 接收数据
received = ser.read(8)  # 根据设备返回长度调整
print(f"接收到: {received.hex()}")

# 关闭串口
ser.close()
