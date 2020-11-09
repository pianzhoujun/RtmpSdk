#pragma once

#include <string>
#include <utility>

#include "muduo/base/Logging.h"
#include "muduo/base/Timestamp.h"
#include "muduo/net/Endian.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpClient.h"

#include "status.h"

namespace mn = muduo::net;

class RtmpClient;

class RtmpConnection {
 public:
  enum {
    STATE_INIT,
    STATE_HANDSHAKE_COC1,
    STATE_HANDSHAKE_C2,
    STATE_MSG
  };

 public:
  RtmpConnection(RtmpClient* rtmp_client)
      : _rtmp_client(rtmp_client), _state(STATE_INIT) {}

  ~RtmpConnection() {}

  void process_message(const mn::TcpConnectionPtr& conn_ptr, mn::Buffer* buffer,
                       muduo::Timestamp rcv_time);

 private:
  std::string create_c0c1();
  Status create_c2(mn::Buffer* buffer, std::string* msg);

 private:
  RtmpClient* _rtmp_client;
  int _state;
};


class RtmpClient {
 public:
  RtmpClient(mn::EventLoop* loop, const mn::InetAddress& server_addr)
      : _loop(loop),
        _client(loop, server_addr, "RtmpClient"),
        _rtmp_connection(this) {
    _client.setConnectionCallback(
        std::bind(&RtmpClient::on_connection, this, std::placeholders::_1));
    _client.setMessageCallback(
        std::bind(&RtmpClient::on_message, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3));
  }

  inline void connect() { _client.connect(); }

  ~RtmpClient() {}

 private:
  void on_connection(const mn::TcpConnectionPtr& conn_ptr);

  void on_message(const mn::TcpConnectionPtr& conn_ptr, mn::Buffer* buffer,
                  muduo::Timestamp rcv_time);

 private:
  mn::TcpClient _client;
  mn::EventLoop* _loop;
  RtmpConnection _rtmp_connection;
};

