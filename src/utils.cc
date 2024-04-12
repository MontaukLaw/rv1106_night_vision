#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include "comm.h"

void get_file_name_by_time(char *file_name_buf, uint8_t file_type)
{
    time_t timep;
    struct tm *pLocalTime;
    time(&timep);

    pLocalTime = localtime(&timep);

    // 20230802201900.mp4
    long long time_long = (long long)(pLocalTime->tm_year + 1900) * 10000000000 +
                          (long long)(pLocalTime->tm_mon + 1) * 100000000 +
                          (long long)(pLocalTime->tm_mday) * 1000000 +
                          (long long)(pLocalTime->tm_hour) * 10000 +
                          (long long)(pLocalTime->tm_min) * 100 +
                          (long long)(pLocalTime->tm_sec);
    switch (file_type)
    {

    case IMAGE_TYPE_ORIGIN:
        sprintf(file_name_buf, "%s%lld", ORIGIN_IMAGE_FOLDER, time_long);
        break;

    case IMAGE_TYPE_TARGET:
        sprintf(file_name_buf, "%s%lld", TARGET_IMAGE_FOLDER, time_long);
        break;
    }

    printf("new file name = %s\n", file_name_buf);
}