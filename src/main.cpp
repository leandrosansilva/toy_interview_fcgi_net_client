#include <fcgicc.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <service.h>
#include <backend/network_client.h>

struct backend_mock: ifnc::backend::network_client
{
  MOCK_METHOD0(get_network_information, ifnc::dto::info());
};

int main(int, char **)
{
  backend_mock mock;

  ifnc::service service(mock);

  EXPECT_CALL(mock, get_network_information())
    .WillRepeatedly(testing::Return(
      ifnc::dto::info{"192.168.1.123", "192.168.1.0", "192.168.1.1"}));

  FastCGIServer server;

  server.request_handler(service, &ifnc::service::request_handler);
  server.data_handler(service, &ifnc::service::data_handler);
  server.complete_handler(service, &ifnc::service::complete_handler);
  
  server.listen("./socket");
  server.listen(8089);

  server.process_forever();
}
