#include "comm.h"

#define PORT 12345 // 服务器端口号

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    // 创建UDP套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // 设置服务器地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 任意IP地址
    server_addr.sin_port = htons(PORT);

    // 绑定服务器套接字到指定端口
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d...\n", PORT);

    while (1)
    {
        // 接收UDP数据包
        ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);

        if (recv_len < 0)
        {
            perror("Error in receiving data");
            exit(EXIT_FAILURE);
        }

        buffer[recv_len] = '\0'; // 添加字符串结束符

        // 打印接收到的数据和客户端信息
        parse_cmd(buffer, recv_len);
    }

    close(sockfd);

    return 0;
}
