#pragma once

#include <iostream>
#include <fcgicc.h>
#include <cereal/archives/json.hpp>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

#include <backend/network_client.h>
#include <serialization/info.h>

namespace ifnc {

template<typename T>
inline void to_response(const T &t, const char *name, std::string &out)
{
  std::ostringstream ss;

  ss << "Content-Type: application/json\r\n\r\n";

  {
    cereal::JSONOutputArchive archive(ss);
    archive(cereal::make_nvp(name, t));
  }

  out.append(ss.str());
}

struct service
{
  ifnc::backend::network_client &_client;

  service(ifnc::backend::network_client &client):
  _client(client)
  {
  }

  int request_handler(FastCGIRequest &request)
  {
    std::cout << request.in << std::endl;

    return 0;
  }

  int data_handler(FastCGIRequest &request)
  {
    return 0;
  }

  int handle_response(std::string &response)
  {
    try {
      to_response(_client.get_network_information(), "information", response);
      return 0;
    } catch (const std::exception &e) {
      to_response(std::string(e.what()), "error", response);
      return 1;
    } catch (...) {
    }

    to_response(std::string("Unknown error"), "error", response);
    return 1;
  }

  int complete_handler(FastCGIRequest &request)
  {
    return handle_response(request.out);
  }

  void register_to_server(FastCGIServer &server)
  {
    server.request_handler(*this, &service::request_handler);
    server.data_handler(*this, &service::data_handler);
    server.complete_handler(*this, &service::complete_handler);
  }
};

}
