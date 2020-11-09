#include <iostream>
#include <string>
#include "rtmp_client.h"

int main() {
    std::cout << "Hello World" << std::endl;
    muduo::net::EventLoop loop;
    muduo::net::InetAddress server_addr("10.138.33.16", 8935);
    RtmpClient client(&loop, server_addr);
    client.connect();
    loop.loop();
    return 0;
}