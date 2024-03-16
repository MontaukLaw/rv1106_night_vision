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

int main(int argc, char **argv)
{
    printf("********************************start***************************************\n");

    int cameraFd; /* 定义一个设备描述符 */
    cameraFd = open("/dev/video0", O_RDWR);
    if (cameraFd < 0)
    {
        perror("video设备打开失败\n");
        return -1;
    }
    else
    {
        printf("video设备打开成功\n");
    }
    printf("********************************1***************************************\n");
    struct v4l2_capability vcap;
    ioctl(cameraFd, VIDIOC_QUERYCAP, &vcap);
    if (!(V4L2_CAP_VIDEO_CAPTURE & vcap.capabilities))
    {
        perror("Error: No capture video device!\n");
        return -1;
    }
    printf("********************************2***************************************\n");
    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    printf("摄像头支持所有格式如下:\n");
    while (ioctl(cameraFd, VIDIOC_ENUM_FMT, &fmtdesc) == 0)
    {
        printf("v4l2_format%d:%s\n", fmtdesc.index, fmtdesc.description);
        fmtdesc.index++;
    }
    printf("********************************3***************************************\n");
    struct v4l2_frmsizeenum frmsize;
    frmsize.index = 0;
    frmsize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    printf("MJPEG格式支持所有分辨率如下:\n");
    frmsize.pixel_format = V4L2_PIX_FMT_MJPEG;
    while (ioctl(cameraFd, VIDIOC_ENUM_FRAMESIZES, &frmsize) == 0)
    {
        printf("frame_size<%d*%d>\n", frmsize.discrete.width, frmsize.discrete.height);
        frmsize.index++;
    }
    printf("YUYV422格式支持所有分辨率如下:\n");
    frmsize.pixel_format = V4L2_PIX_FMT_MJPEG;
    while (ioctl(cameraFd, VIDIOC_ENUM_FRAMESIZES, &frmsize) == 0)
    {
        printf("frame_size<%d*%d>\n", frmsize.discrete.width, frmsize.discrete.height);
        frmsize.index++;
    }
    printf("********************************4***************************************\n");
    // struct v4l2_frmivalenum frmival;
    // frmival.index = 0;
    // frmival.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    // frmival.pixel_format = V4L2_PIX_FMT_MJPEG;
    // frmival.width = 640;
    // frmival.height = 480;
    // while(ioctl(cameraFd,VIDIOC_ENUM_FRAMEINTERVALS,&frmival) == 0){
    //     printf("frame_interval under frame_size <%d*%d> support %dfps\n",frmival.width,frmival.height,frmival.discrete.denominator / frmival.discrete.numerator);
    //     frmival.index++;
    // }
    struct v4l2_frmivalenum frmival;
    frmival.index = 0;
    frmival.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    frmival.pixel_format = V4L2_PIX_FMT_MJPEG;
    frmival.width = 2592;
    frmival.height = 1944;
    while (ioctl(cameraFd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival) == 0)
    {
        printf("frame_interval under frame_size <%d*%d> support %dfps\n", frmival.width, frmival.height, frmival.discrete.denominator / frmival.discrete.numerator);
        frmival.index++;
    }
    printf("********************************5***************************************\n");

    // struct v4l2_format vfmt;
    // vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    // vfmt.fmt.pix.width = 640;
    // vfmt.fmt.pix.height = 480;
    // vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    // if(ioctl(cameraFd,VIDIOC_S_FMT,&vfmt) < 0){
    //     perror("设置格式失败\n");
    //     return -1;
    // }
    // // 检查设置参数是否生效
    // if(ioctl(cameraFd,VIDIOC_G_FMT,&vfmt) < 0){
    //     perror("获取设置格式失败\n");
    //     return -1;
    // }
    // else if(vfmt.fmt.pix.width == 640 && vfmt.fmt.pix.height == 480 && vfmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG){
    //     printf("设置格式生效,实际分辨率大小<%d * %d>,图像格式:Motion-JPEG\n",vfmt.fmt.pix.width,vfmt.fmt.pix.height);
    // }
    // else{
    //     printf("设置格式未生效\n");
    // }

    struct v4l2_format vfmt;
    vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vfmt.fmt.pix.width = 2592;
    vfmt.fmt.pix.height = 1944;
    vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    if (ioctl(cameraFd, VIDIOC_S_FMT, &vfmt) < 0)
    {
        perror("设置格式失败\n");
        return -1;
    }
    // 检查设置参数是否生效
    if (ioctl(cameraFd, VIDIOC_G_FMT, &vfmt) < 0)
    {
        perror("获取设置格式失败\n");
        return -1;
    }
    else if (vfmt.fmt.pix.width == 2592 && vfmt.fmt.pix.height == 1944 && vfmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG)
    {
        printf("设置格式生效,实际分辨率大小<%d * %d>,图像格式:Motion-JPEG\n", vfmt.fmt.pix.width, vfmt.fmt.pix.height);
    }
    else
    {
        printf("设置格式未生效\n");
    }

    printf("********************************6***************************************\n");
    struct v4l2_requestbuffers reqbuf;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.count = 3; // 3个帧缓冲
    reqbuf.memory = V4L2_MEMORY_MMAP;
    if (ioctl(cameraFd, VIDIOC_REQBUFS, &reqbuf) < 0)
    {
        perror("申请缓冲区失败\n");
        return -1;
    }
    printf("********************************7***************************************\n");
    // 将帧缓冲映射到进程地址空间
    void *frm_base[3]; // 映射后的用户空间的首地址
    unsigned int frm_size[3];

    struct v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    // 将每一帧对应的缓冲区的起始地址保存在frm_base数组中，读取采集数据时，只需直接读取映射区即可
    for (buf.index = 0; buf.index < 3; buf.index++)
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
    printf("********************************8***************************************\n");
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(cameraFd, VIDIOC_STREAMON, &type) < 0)
    {
        perror("开始采集失败\n");
        return -1;
    }
    printf("********************************9***************************************\n");
    struct v4l2_buffer readbuffer;
    readbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    readbuffer.memory = V4L2_MEMORY_MMAP;
    if (ioctl(cameraFd, VIDIOC_DQBUF, &readbuffer) < 0)
    {
        perror("读取帧失败\n");
    }

    // 保存这一帧，格式为jpg
    FILE *file = fopen("v4l2_cap.jpg", "w+");
    fwrite(frm_base[readbuffer.index], buf.length, 1, file);
    fclose(file);
    printf("********************************10***************************************\n");
    // 停止采集
    if (ioctl(cameraFd, VIDIOC_STREAMOFF, &type) < 0)
    {
        perror("停止采集失败\n");
        return -1;
    }

    // 释放映射
    for (int i = 0; i < 3; i++)
    {
        munmap(frm_base[i], frm_size[i]);
    }

    close(cameraFd);

    return 0;
}
