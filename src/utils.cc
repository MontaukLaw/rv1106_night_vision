#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include "comm.h"

void get_file_name_by_time(char *file_name_buf)
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

    sprintf(file_name_buf, "%s%lld", IMAGE_FOLDER, time_long);

    printf("new file name = %s\n", file_name_buf);

}