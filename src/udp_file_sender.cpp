#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <unistd.h>

#define SERVER_IP "192.168.1.88"
#define SERVER_PORT 8888

int send_file_through_udp(char *file_name)
{

    // 准备发送的数据
    char *start_buffer = "START";
    char *end_buffer = "END";

    std::cout << "sending " << file_name << std::endl;

    // 读取文件
    std::ifstream file(file_name, std::ios::binary );
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file" << std::endl;
        return -1;
    }

    // // 获取文件大小并读取内容
    // std::streamsize size = file.tellg();
    // file.seekg(0, std::ios::beg);
    // std::vector<char> buffer(size);
    // if (!file.read(buffer.data(), size))
    // {
    //     std::cerr << "Error: Cannot read file" << std::endl;
    //     return -1;
    // }

    // 创建UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error: Cannot open socket" << std::endl;
        return -1;
    }

    // 目标地址和端口配置
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);          // 目标端口号
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); // 目标IP地址

    // 发送启动数据包
    ssize_t sentBytes = sendto(sockfd, start_buffer, 5, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (sentBytes < 0)
    {
        std::cerr << "Error: Cannot send data start" << std::endl;
        return -1;
    }

    // 定义每次发送的数据块大小
    const size_t bufferSize = 1024;
    char buffer[bufferSize];

    // 循环读取文件内容并发送
    while (!file.eof())
    {
        file.read(buffer, bufferSize);
        std::streamsize bytes_read = file.gcount(); // 实际读取的字节数

        // 发送读取的数据块
        ssize_t sentBytes = sendto(sockfd, buffer, bytes_read, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        if (sentBytes < 0)
        {
            std::cerr << "Error: Cannot send data" << std::endl;
            break;
        }

        // 等待10ms
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // // 发送文件内容
    // sentBytes = sendto(sockfd, buffer.data(), buffer.size(), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    // if (sentBytes < 0)
    // {
    //     std::cerr << "Error: Cannot send data" << std::endl;
    // }

    // 发送结束数据包
    sentBytes = sendto(sockfd, end_buffer, 3, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (sentBytes < 0)
    {
        std::cerr << "Error: Cannot send data end" << std::endl;
        return -1;
    }

    // std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // 关闭文件和socket
    file.close();
    close(sockfd);

    return 0;
}
