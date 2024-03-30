import socket
import os
import time

from datetime import datetime


# 创建一个socket对象
# socket.AF_INET 指定使用IPv4地址
# socket.SOCK_DGRAM 指定使用UDP协议
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 绑定地址和端口
# '' 表示绑定到所有可用的接口
# 88888 是指定的端口号
sock.bind(('', 8888))


def write_file(text_to_write, file_path):
    with open(file_path, "wb") as file:
        # 写入字符串到文件
        file.write(text_to_write)


# 通过时间戳产生一个文件名
def get_file_name_by_time():
    # 获取当前时间
    current_time = datetime.now()
    # 格式化输出为YYYYMMDDHHMMSS
    formatted_time = current_time.strftime("%Y%m%d%H%M%S")

    return str(formatted_time) + ".jpg"


print("UDP服务器启动，等待接收数据...")

rev_data = b''

try:
    while True:
        # 接收数据
        # 1024 是接收数据的最大字节数
        data, addr = sock.recvfrom(1024)

        print(type(data))
        print(len(data))

        if data == b'START':
            rev_data = b''
            print("Start")

        elif data == b'END':
            print("传输完成, 写入jpg文件")
            write_file(rev_data, get_file_name_by_time())
            print("End")

        else:
            rev_data += data
            # print("Data: ", data)

        # 使用16进制打印接收到的数据
        # print("从 {} 接收到的数据: {}".format(addr, data.hex()))

        # 打印接收到的数据和发送方的地址
        # print("从 {} 接收到的数据: {}".format(addr, data.decode()))

except KeyboardInterrupt:
    print("\nUDP服务器关闭。")

finally:
    # 关闭socket
    sock.close()
