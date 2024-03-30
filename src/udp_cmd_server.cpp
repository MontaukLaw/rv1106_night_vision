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

int main()
{
    // 创建一个新线程来运行UDP服务器
    std::thread serverThread(runUDPServer);

    std::cout << "UDP Server is running in a separate thread." << std::endl;

    // 主线程的其它逻辑...
    // 例如，可以等待用户输入，以决定何时停止服务器
    std::string command;

    while (true)
    {
    }

    // 等待服务器线程结束
    if (serverThread.joinable())
    {
        serverThread.join();
    }

    return 0;
}
