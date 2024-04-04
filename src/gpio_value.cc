#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// 我们要访问的GPIO编号
#define GPIO_NUMBER "55"

// GPIO文件路径
#define GPIO_PATH "/sys/class/gpio/"

void exportGPIO()
{
    int fileDescriptor;
    char buffer[64];

    fileDescriptor = open(GPIO_PATH "export", O_WRONLY);
    if (fileDescriptor == -1)
    {
        perror("Unable to open /sys/class/gpio/export");
        exit(1);
    }

    ssize_t bytes_written = write(fileDescriptor, GPIO_NUMBER, strlen(GPIO_NUMBER));
    if (bytes_written == -1)
    {
        perror("Error writing to /sys/class/gpio/export");
        exit(1);
    }

    close(fileDescriptor);
}

void setDirectionInput()
{
    int fileDescriptor;
    char buffer[64];

    snprintf(buffer, sizeof(buffer), GPIO_PATH "gpio%s/direction", GPIO_NUMBER);

    fileDescriptor = open(buffer, O_WRONLY);
    if (fileDescriptor == -1)
    {
        perror("Unable to open GPIO direction file");
        exit(1);
    }

    ssize_t bytes_written = write(fileDescriptor, "in", 2);
    if (bytes_written == -1)
    {
        perror("Error writing to GPIO direction file");
        exit(1);
    }

    close(fileDescriptor);
}

int readGPIO()
{
    int fileDescriptor;
    char buffer[64];
    char value[4];
    char value_byte[1];

    snprintf(buffer, sizeof(buffer), GPIO_PATH "gpio%s/value", GPIO_NUMBER);

    // fileDescriptor = open(buffer, O_RDONLY);
    // /sys/class/gpio/gpio55
    fileDescriptor = open("/sys/class/gpio/gpio55/value", O_RDONLY);
    if (fileDescriptor == -1)
    {
        perror("Unable to open GPIO value file");
        return 0;
    }

    if (read(fileDescriptor, value, 1) == -1)
    {
        perror("Error reading GPIO value");
        return 0;
    }

    // printf("The value of GPIO%s is: %s\n", GPIO_NUMBER, value);
    // printf("The value is %s\n", value);

    memcpy(value_byte, value, 1);
    int gpio_status = atoi(value_byte);
    if (gpio_status)
    {
        printf("High lever");
    }
    else
    {
        printf("Low lever");
    }

    close(fileDescriptor);

    return gpio_status;
}

int get_gpip_status()
{
    // 导出GPIO，使其可用
    // exportGPIO();
    // sleep(1); // 等待GPIO文件被创建

    // 设置GPIO方向为输入
    // setDirectionInput();

    // 读取GPIO值
    return readGPIO();
}
