#include "comm.h"

using namespace cv;

// 把原来640x480的图像, 转成640x640, 上下填充灰色
void letterbox(cv::Mat origin_img, cv::Mat letterbox_img)
{
    int origin_width = origin_img.cols;
    int origin_height = origin_img.rows;
    int letterbox_width = letterbox_img.cols;
    int letterbox_height = letterbox_img.rows;

    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;

    if (origin_width < letterbox_width)
    {
        left = (letterbox_width - origin_width) / 2;
        right = letterbox_width - origin_width - left;
    }

    if (origin_height < letterbox_height)
    {
        top = (letterbox_height - origin_height) / 2;
        bottom = letterbox_height - origin_height - top;
    }

    copyMakeBorder(origin_img, letterbox_img, top, bottom, left, right, BORDER_CONSTANT, Scalar(128, 128, 128));
}