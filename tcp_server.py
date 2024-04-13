import socket
import os
import time
from datetime import datetime

# 创建一个 TCP 套接字
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 绑定地址和端口
sock.bind(('', 8888))
# 开始监听传入连接请求
sock.listen(5)
print("TCP服务器启动，等待接收连接...")

def write_file(text_to_write, file_path):
    with open(file_path, "wb") as file:
        file.write(text_to_write)

def get_file_name_by_time():
    # 获取当前时间并格式化输出为YYYYMMDDHHMMSS
    current_time = datetime.now()
    formatted_time = current_time.strftime("%Y%m%d%H%M%S")
    return str(formatted_time) + ".jpg"

try:
    while True:
        # 接受一个新连接
        conn, addr = sock.accept()
        print(f"连接已建立: {addr}")

        rev_data = b''

        while True:
            # 接收数据
            data = conn.recv(1024)

            if not data:
                # 没有数据表示客户端已经关闭
                print("连接已关闭")
                conn.close()
                break

            print(f"从 {addr} 接收到的数据长度: {len(data)}")

            if data == b'START':
                rev_data = b''
                print("开始接收文件数据")

            elif data == b'END':
                print("传输完成, 写入jpg文件")
                write_file(rev_data, get_file_name_by_time())
                print("文件写入完成，继续等待数据...")

            else:
                rev_data += data

except KeyboardInterrupt:
    print("\nTCP服务器关闭。")

finally:
    # 关闭socket
    sock.close()
