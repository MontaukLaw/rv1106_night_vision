#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


void parse_cmd(char* cmd, int cmd_len);

int start_detect(char *model_path, char *input_path);

#endif