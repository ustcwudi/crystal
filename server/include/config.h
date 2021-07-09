#pragma once

#include <string>

using namespace std;

class Config
{
private:
    /* data */
public:
    int port;
    string ip;
    string username;
    string password;
    string mysql;
    string mysql_username;
    string mysql_password;
    string redis;
    string redis_password;
    int fps;
    int camera_x;
    int camera_y;
    int qr_x;
    int qr_y;
    int cam_width;
    int cam_height;
    int camera_index;
    int show_camera;
    int show_qr;
    Config(string file);
    ~Config();
};
