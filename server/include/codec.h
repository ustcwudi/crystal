#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include "config.h"

extern "C"
{
#include "x264.h"
}

class Codec
{
private:
    int num = 0;
    Config *config;
    char *buffer;

    x264_t *pHandle = NULL;
    x264_param_t *pParam = (x264_param_t *)malloc(sizeof(x264_param_t));
    x264_picture_t *pPic_in = (x264_picture_t *)malloc(sizeof(x264_picture_t));
    x264_picture_t *pPic_out = (x264_picture_t *)malloc(sizeof(x264_picture_t));

public:
    Codec(Config *config);
    int Encode(char *data, char **output, int *length);
    int Encode(char **output, int *length);
    ~Codec();
};
