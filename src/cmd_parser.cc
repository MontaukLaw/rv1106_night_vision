#include "comm.h"
#define CMD "cmd"
#define DETECT_START "start"

static bool if_detecting = false;
// 解析接收到的命令
void parse_cmd(char *cmd, int cmd_len)
{
    printf("Received data len %d: %s\n", cmd_len, cmd);
    char *cmdType = strtok(cmd, ":");
    char *cmdValue = NULL;

    // 使用冒号分隔命令
    if (cmdType != NULL)
    {
        // 使用分号分隔命令
        cmdValue = strtok(NULL, ";");
        if (cmdValue != NULL)
        {
            printf("%s : %s\n", cmdType, cmdValue);

            if (strcmp(cmdType, CMD) == 0 && strcmp(cmdValue, DETECT_START) == 0)
            {
                // 开启检测
                printf("start detect\n");

                if (if_detecting == false)
                {
                    if_detecting = true;
                    start_detect("yolov5s-640-640.rknn", "/tftpboot/input.jpg");
                    if_detecting = false;
                }
            }
        }
    }
}
