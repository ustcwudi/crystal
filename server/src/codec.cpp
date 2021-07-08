#include "codec.h"
#include <string.h>

void convert(char *yuyv, char *yuv422, int width, int height)
{
    for (int i = 0; i < width * height; i++)
    {
        yuv422[i] = yuyv[i * 2];
    }
    for (int i = 0; i < width * height / 2; i++)
    {
        yuv422[width * height + i] = yuyv[i * 4 + 1];
    }
    for (int i = 0; i < width * height / 2; i++)
    {
        yuv422[width * height * 3 / 2 + i] = yuyv[i * 4 + 3];
    }
}

Codec::Codec(Config *config)
{
    this->config = config;
    x264_param_default_preset(pParam, "veryfast", "zerolatency");
    pParam->i_width = config->cam_width;
    pParam->i_height = config->cam_height;
    pParam->i_log_level = X264_LOG_DEBUG;
    pParam->i_threads = X264_SYNC_LOOKAHEAD_AUTO;
    pParam->i_frame_total = 0;
    pParam->i_keyint_max = X264_KEYINT_MAX_INFINITE;
    pParam->i_bframe = 0;
    pParam->b_open_gop = 0;
    pParam->i_bframe_pyramid = 0;
    pParam->rc.i_qp_constant = 0;
    pParam->rc.i_qp_max = 0;
    pParam->rc.i_qp_min = 0;
    pParam->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
    pParam->i_fps_den = 1;
    pParam->i_fps_num = config->fps;
    pParam->i_timebase_den = pParam->i_fps_num;
    pParam->i_timebase_num = pParam->i_fps_den;
    pParam->i_csp = X264_CSP_I422;
    x264_param_apply_profile(pParam, x264_profile_names[4]);

    pHandle = x264_encoder_open(pParam);
    x264_picture_init(pPic_out);
    x264_picture_alloc(pPic_in, X264_CSP_I422, pParam->i_width, pParam->i_height);
    buffer = new char[config->cam_width * config->cam_height * 2];
    // ret = x264_encoder_headers(pHandle, &pNals, &iNal);
}

int Codec::Encode(char *data, char **output, int *length)
{
    size_t size = config->cam_width * config->cam_height;
    convert(data, buffer, config->cam_width, config->cam_height);
    memcpy(pPic_in->img.plane[0], buffer, size);
    memcpy(pPic_in->img.plane[1], buffer + size, size / 2);
    memcpy(pPic_in->img.plane[2], buffer + size * 3 / 2, size / 2);
    pPic_in->i_pts = num++;

    int iNal = 0;
    x264_nal_t *pNals = NULL;
    int ret = x264_encoder_encode(pHandle, &pNals, &iNal, pPic_in, pPic_out);
    if (ret > 0)
    {
        int count = 0;
        for (int j = 0; j < iNal; ++j)
        {
            memcpy(buffer + count, pNals[j].p_payload, pNals[j].i_payload);
            count += pNals[j].i_payload;
        }
        *output = buffer;
        *length = count;
    }
    return ret;
}

int Codec::Encode(char **output, int *length)
{
    int iNal = 0;
    x264_nal_t *pNals = NULL;
    int ret = x264_encoder_encode(pHandle, &pNals, &iNal, NULL, pPic_out);
    if (ret > 0)
    {
        printf("flush 1 frame.\n");
        int count = 0;
        for (int j = 0; j < iNal; ++j)
        {
            memcpy(buffer + count, pNals[j].p_payload, pNals[j].i_payload);
            count += pNals[j].i_payload;
        }
        *output = buffer;
        *length = count;
    }
    return ret;
}

Codec::~Codec()
{
    x264_picture_clean(pPic_in);
    x264_encoder_close(pHandle);

    free(pPic_in);
    free(pPic_out);
    free(pParam);

    delete[] buffer;
}
