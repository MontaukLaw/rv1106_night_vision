#include "comm.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#define VIDEO_IN_WIDTH 640
#define VIDEO_IN_HEIGHT 480
#define BUFFER_FRAME_NUMBER 3

int get_image(char *file_name)
{
    printf("********start capture image********\n");

    int cameraFd; /* 定义一个设备描述符 */
    cameraFd = open("/dev/video0", O_RDWR);
    if (cameraFd < 0)
    {
        perror("video设备打开失败\n");
        return -1;
    }
    else
    {
        printf("open video device success\n");
    }
    printf("step 1\n");
    struct v4l2_capability vcap;
    ioctl(cameraFd, VIDIOC_QUERYCAP, &vcap);
    if (!(V4L2_CAP_VIDEO_CAPTURE & vcap.capabilities))
    {
        perror("Error: No capture video device!\n");
        return -1;
    }
    printf("step 2\n");

    struct v4l2_format vfmt;
    vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vfmt.fmt.pix.width = VIDEO_IN_WIDTH;
    vfmt.fmt.pix.height = VIDEO_IN_HEIGHT;
    vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    if (ioctl(cameraFd, VIDIOC_S_FMT, &vfmt) < 0)
    {
        perror("设置格式失败\n");
        return -1;
    }

    // 检查设置参数是否生效
    if (ioctl(cameraFd, VIDIOC_G_FMT, &vfmt) < 0)
    {
        perror("Set video format MJPEG success\n");
        return -1;
    }

    else if (vfmt.fmt.pix.width == VIDEO_IN_WIDTH && vfmt.fmt.pix.height == VIDEO_IN_HEIGHT && vfmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG)
    {
        printf("MJPEG: %dx%d\n", vfmt.fmt.pix.width, vfmt.fmt.pix.height);
    }
    else
    {
        printf("设置格式未生效\n");
    }

    printf("step 3\n");
    struct v4l2_requestbuffers reqbuf;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.count = 3; // 3个帧缓冲
    reqbuf.memory = V4L2_MEMORY_MMAP;
    if (ioctl(cameraFd, VIDIOC_REQBUFS, &reqbuf) < 0)
    {
        perror("申请缓冲区失败\n");
        return -1;
    }

    printf("step 4\n");
    // 将帧缓冲映射到进程地址空间
    void *frm_base[BUFFER_FRAME_NUMBER]; // 映射后的用户空间的首地址
    unsigned int frm_size[BUFFER_FRAME_NUMBER];

    struct v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    // 将每一帧对应的缓冲区的起始地址保存在frm_base数组中，
    // 读取采集数据时，只需直接读取映射区即可

    // 一个缓冲区被用于当前的视频帧捕获：这意味着在任何给定时间点，一个缓冲区都在被摄像头填充新的视频帧数据。
    // 一个缓冲区在队列中等待：当当前填充的缓冲区被处理（例如，读取数据、保存到文件）后，另一个缓冲区就已经准备好被填充，这样就不会因为处理时间而错过下一帧。
    // 一个缓冲区用于数据处理：这包括从缓冲区读取数据、处理数据（例如，编码、保存到文件）等操作。
    for (buf.index = 0; buf.index < BUFFER_FRAME_NUMBER; buf.index++)
    {
        ioctl(cameraFd, VIDIOC_QUERYBUF, &buf);
        frm_base[buf.index] = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, cameraFd, buf.m.offset);
        frm_size[buf.index] = buf.length;

        if (frm_base[buf.index] == MAP_FAILED)
        {
            perror("mmap failed\n");
            return -1;
        }

        // 入队操作
        if (ioctl(cameraFd, VIDIOC_QBUF, &buf) < 0)
        {
            perror("入队失败\n");
            return -1;
        }
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(cameraFd, VIDIOC_STREAMON, &type) < 0)
    {
        perror("开始采集失败\n");
        return -1;
    }
    struct v4l2_buffer readbuffer;
    readbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    readbuffer.memory = V4L2_MEMORY_MMAP;
    if (ioctl(cameraFd, VIDIOC_DQBUF, &readbuffer) < 0)
    {
        perror("读取帧失败\n");
    }

    // 保存这一帧，格式为jpg
    char jpeg_file_name[50];
    get_file_name_by_time(file_name);
    sprintf(jpeg_file_name, "%s.jpg", file_name);

    FILE *file = fopen(jpeg_file_name, "w+");
    printf("len:%d index:%d \n", buf.length, readbuffer.index);
    fwrite(frm_base[readbuffer.index], buf.length, 1, file);
    fclose(file);
    // 停止采集
    if (ioctl(cameraFd, VIDIOC_STREAMOFF, &type) < 0)
    {
        perror("停止采集失败\n");
        return -1;
    }

    // 释放映射
    for (int i = 0; i < BUFFER_FRAME_NUMBER; i++)
    {
        munmap(frm_base[i], frm_size[i]);
    }

    close(cameraFd);

    return 0;
}

int main(int argc, char **argv)
{
    char file_name[100];
    get_image(file_name);

    printf("File name :%s\r\n", file_name);

    char jpeg_file_name[100];
    sprintf(jpeg_file_name, "%s.jpg", file_name);

    char letter_box_jpeg[100];
    sprintf(letter_box_jpeg, "%s_letterbox.jpg", file_name);

    // 使用opencv打开jpeg
    cv::Mat img = cv::imread(jpeg_file_name);
    if (img.empty())
    {
        printf("read image failed\n");
        return -1;
    }

    // for test
    // imwrite("test.jpg", img);

    cv::Mat letterbox_img;
    
    // letterbox尺寸为640x640
    letterbox_img = cv::Mat(640, 640, CV_8UC3);

    letterbox(img, letterbox_img);

    cv::imwrite(letter_box_jpeg, letterbox_img);

    start_detect("yolov5s-640-640.rknn", letter_box_jpeg, file_name, letterbox_img);

    return 0;
}
