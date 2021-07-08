#pragma once
#include "v4l2/V4l2Capture.h"
#include "config.h"
#include <stdio.h>

class Camera
{
public:
    Camera(Config *config);
    ~Camera();
    bool Grab(char **data);

private:
    Config *config;
    char *buffer = NULL;
    V4l2Capture *capture;
};