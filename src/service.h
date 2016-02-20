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
inline int to_response(const T &t, const char *name, FastCGIRequest &request)
{
  request.out.append("Content-Type: application/json\r\n\r\n");

  std::ostringstream ss;
  {
    cereal::JSONOutputArchive archive(ss);
    archive(cereal::make_nvp(name, t));
  }

  request.out.append(ss.str());

  return 0;
}

struct service
{
  ifnc::backend::network_client &_client;

  service(ifnc::backend::network_client &client):
  _client(client)
  {
  }

  ifnc::dto::info get_information() const
  {
    return _client.get_network_information();
  }

  int request_handler(FastCGIRequest &request)
  {
    std::cout << "request: '" << request.params[std::string("REQUEST_URI")] << "'" << std::endl;
    return 0;
  }

  int data_handler(FastCGIRequest &request)
  {
    return 0;
  }

  int complete_handler(FastCGIRequest &request)
  {
    try {
      return to_response(get_information(), "information", request);
    } catch (const std::exception &e) {
      return to_response(std::string(e.what()), "error", request);
    } catch (...) {
      return to_response(std::string("Unknown error"), "error", request);
    }
  }

  void register_to_server(FastCGIServer &server)
  {
    server.request_handler(*this, &service::request_handler);
    server.data_handler(*this, &service::data_handler);
    server.complete_handler(*this, &service::complete_handler);
  }
};

}
