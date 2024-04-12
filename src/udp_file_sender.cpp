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
#include <dirent.h>
#include <sys/stat.h>
#include "comm.h"
#include <iostream>
#include <string>

#define SERVER_IP "192.168.1.33"
#define SERVER_PORT 8888
#define CMD_SERVER_PORT 7777

int send_file_through_udp(const char *file_name)
{

    // 准备发送的数据
    char *start_buffer = "START";
    char *end_buffer = "END";

    std::cout << "sending " << file_name << std::endl;

    // 读取文件
    std::ifstream file(file_name, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file" << std::endl;
        return -1;
    }

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

void searchAndPrintFiles(const std::string &directory)
{
    DIR *dir = opendir(directory.c_str());
    if (dir == nullptr)
    {
        std::cerr << "Error opening directory: " << directory << std::endl;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        // 忽略当前目录"."和上一级目录".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // 构建完整的文件路径
        std::string filePath = directory + std::string(entry->d_name);
        const char *filePathCStr = filePath.c_str();
        // 检查是否为普通文件
        struct stat path_stat;
        stat(filePath.c_str(), &path_stat);
        if (S_ISREG(path_stat.st_mode))
        {
            std::cout << "Sending file: " << filePath << std::endl;

            send_file_through_udp(filePathCStr);
        }
    }

    closedir(dir);
}

void runUDPServer()
{
    // 创建UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error: Cannot open socket" << std::endl;
        return;
    }

    // 服务器地址配置
    struct sockaddr_in serverAddr, clientAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;      // 监听任何来源的地址
    serverAddr.sin_port = htons(CMD_SERVER_PORT); // 绑定到端口7777

    // 绑定socket到服务器地址
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error: Cannot bind to port " << CMD_SERVER_PORT << std::endl;
        close(sockfd);
        return;
    }

    std::cout << "UDP Server listening on port " << CMD_SERVER_PORT << std::endl;

    // 循环接收数据
    while (true)
    {
        char buffer[1024];
        socklen_t len = sizeof(clientAddr);
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &len);
        if (n < 0)
        {
            std::cerr << "Error: Cannot receive data" << std::endl;
            continue;
        }

        // 确保字符串正确终结
        buffer[n] = '\0';
        std::string receivedString(buffer);

        // 判断接收到的数据
        if (receivedString == "START_TRANSFORM_ORIGIN")
        {
            std::cout << "Received 'START_TRANSFORM_ORIGIN' command" << std::endl;

            // 遍历/mnt/sdcard目录后发送
            searchAndPrintFiles(ORIGIN_IMAGE_FOLDER);
        }
        else if (receivedString == "START_TRANSFORM_TARGET")
        {
            std::cout << "Received 'START_TRANSFORM_TARGET' command" << std::endl;

            // 遍历/mnt/sdcard目录后发送
            searchAndPrintFiles(TARGET_IMAGE_FOLDER);
        }
        else
        {
            std::cout << "Received different command: " << receivedString << std::endl;
        }
    }

    // 关闭socket
    close(sockfd);
}
