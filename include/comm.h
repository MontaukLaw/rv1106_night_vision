#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <linux/videodev2.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

void parse_cmd(char *cmd, int cmd_len);

int start_detect(char *model_path, char *input_path, char *file_prefix, cv::Mat letterbox_img);

void get_file_name_by_time(char *file_name_buf);

void letterbox(cv::Mat origin_img, cv::Mat letterbox_img);

int send_file_through_udp(const char *file_name);

int get_gpip_status();

void runUDPServer();

#define IMAGE_FOLDER "/mnt/sdcard/imgs/"

#endif