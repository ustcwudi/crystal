#include "camera.h"
#include "v4l2/V4l2Device.h"

Camera::Camera(Config *config)
{
    this->config = config;
    char dev[20];
    sprintf(dev, "/dev/video%d", config->camera_index);
    V4L2DeviceParameters param(dev, V4L2_PIX_FMT_YUYV, config->cam_width, config->cam_height, config->fps, IOTYPE_MMAP, 1);
    capture = V4l2Capture::create(param);
    buffer = new char[config->cam_width * config->cam_height * 2];
}

bool Camera::Grab(char **data)
{
    timeval timeout;
    bool readable = (capture->isReadable(&timeout) == 1);
    if (readable)
    {
        size_t size = config->cam_width * config->cam_height * 2;
        size_t length = capture->read(buffer, size);
        if (length == size)
        {
            *data = buffer;
        }
        else
        {
            printf("read camera data error: %zu\n", length);
            return false;
        }
    }
    return readable;
}

Camera::~Camera()
{
    delete capture;
    delete[] buffer;
}