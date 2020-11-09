#include "rtmp_client.h"

#include "utils.h"

void RtmpClient::on_connection(const mn::TcpConnectionPtr& conn_ptr) {
  LOG_INFO << conn_ptr->localAddress().toIpPort() << " -> "
           << conn_ptr->peerAddress().toIpPort()
           << (conn_ptr->connected() ? " UP" : " DOWN");
  if (!conn_ptr->connected()) {
    _loop->quit();
  } else {
    mn::Buffer buffer;
    _rtmp_connection.process_message(conn_ptr, &buffer,
                                     muduo::Timestamp::now());
  }
}

void RtmpClient::on_message(const mn::TcpConnectionPtr& conn_ptr,
                            mn::Buffer* buffer, muduo::Timestamp rcv_time) {
  _rtmp_connection.process_message(conn_ptr, buffer, muduo::Timestamp::now());
}

void RtmpConnection::process_message(const mn::TcpConnectionPtr& conn_ptr,
                                     mn::Buffer* buffer,
                                     muduo::Timestamp rcv_time) {
  switch (_state) {
    case STATE_INIT: {
      std::string msg = create_c0c1();
      conn_ptr->send(msg);
      LOG_INFO << "send c0c1";
      ++_state;
      break;
    }

    case STATE_HANDSHAKE_COC1: {
      LOG_INFO << "process c0c1 back";
      std::string msg;
      Status rc = create_c2(buffer, &msg);
      if (rc.ok()) {
        conn_ptr->send(msg);
      }

      ++_state;
      break;
    }

    case STATE_HANDSHAKE_C2: {
      LOG_INFO << "handshark success";
      ++_state;
      break;
    }

    default:
      break;
  }
}

std::string RtmpConnection::create_c0c1() {
  srand(0);

  char data[1537] = {'\0'};
  data[0] = 0x03;

  time_t ts = time(nullptr);
  char* p = (char*)&ts;
  data[1] = p[3];
  data[2] = p[2];
  data[3] = p[1];
  data[4] = p[0];
  data[5] = 0x0;
  data[6] = 0x0;
  data[7] = 0x0;
  data[8] = 0x0;

  for (int i = 9; i < 1537; ++i) {
    data[i] = rand() % 256;
  }

  return std::string(data, 1537);
}

Status RtmpConnection::create_c2(mn::Buffer* buffer, std::string* msg) {
  int8_t version = buffer->readInt8();
  LOG_INFO << "version: " << version;

  time_t ts = 0;
  char* s1_data = (char*)&ts;

  s1_data[3] = *buffer->peek();
  buffer->retrieveInt8();

  s1_data[2] = *buffer->peek();
  buffer->retrieveInt8();

  s1_data[1] = *buffer->peek();
  buffer->retrieveInt8();

  s1_data[0] = *buffer->peek();
  buffer->retrieveInt8();

  LOG_INFO << "timestamp: " << ts;

  char c2[1536];
  {
    time_t now = time_t(nullptr);
    char* d = (char*)&now;
    c2[0] = d[3];
    c2[1] = d[2];
    c2[2] = d[1];
    c2[3] = d[0];
  }

  {
    c2[4] = s1_data[0];
    c2[5] = s1_data[1];
    c2[6] = s1_data[2];
    c2[7] = s1_data[3];
  }

  {
    for (int i = 8; i < 1536; ++i) {
      c2[i] = rand() % 256;
    }
  }

  msg->append(c2, sizeof(c2));
  return Status::OK();
}