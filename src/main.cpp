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

  FastCGIServer server;

  ifnc::service service(mock);

  ifnc::dto::info mocked_info {
    ifnc::dto::interface_information{"eth0", "192.168.1.123", "192.168.1.0", "192.168.1.1"},
    ifnc::dto::interface_information{"wlan0", "192.168.2.123", "192.168.2.0", "192.168.2.1"}
  };

  EXPECT_CALL(mock, get_network_information())
    .WillRepeatedly(testing::Return(mocked_info));

  service.register_to_server(server);
  server.listen("./socket");
  server.listen(8089);

  server.process_forever();
}
