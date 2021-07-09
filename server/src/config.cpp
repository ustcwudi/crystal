#include "config.h"
#include <libconfig.h++>

Config::Config(string file)
{
    libconfig::Config cfg;
    cfg.readFile(file);
    cfg.lookupValue("port", port);
    cfg.lookupValue("ip", ip);
    cfg.lookupValue("username", username);
    cfg.lookupValue("password", password);
    cfg.lookupValue("mysql", mysql);
    cfg.lookupValue("mysql_username", mysql_username);
    cfg.lookupValue("mysql_password", mysql_password);
    cfg.lookupValue("redis", redis);
    cfg.lookupValue("redis_password", redis_password);
    cfg.lookupValue("fps", fps);
    cfg.lookupValue("camera_x", camera_x);
    cfg.lookupValue("camera_y", camera_y);
    cfg.lookupValue("qr_x", qr_x);
    cfg.lookupValue("qr_y", qr_y);
    cfg.lookupValue("cam_width", cam_width);
    cfg.lookupValue("cam_height", cam_height);
    cfg.lookupValue("camera_index", camera_index);
    cfg.lookupValue("show_camera", show_camera);
    cfg.lookupValue("show_qr", show_qr);
}

Config::~Config()
{
}