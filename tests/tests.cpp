#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <backend/network_client.h>
#include <service.h>

using namespace testing;

struct backend_mock: ifnc::backend::network_client
{
  MOCK_METHOD0(get_network_information, ifnc::dto::info());
};

ACTION(ThrowStdException)
{
  throw std::runtime_error("Some random Error");
}

ACTION(ThrowUnknownException)
{
  throw 1;
}

TEST(Service, Requests)
{
  backend_mock mock;

  ifnc::service service(mock);

  ifnc::dto::info mocked_information_no_network {};

  ifnc::dto::info mocked_information_two_interfaces {
  	ifnc::dto::interface_information {
  		"wlan0",
  		ifnc::dto::interface_information::ip_info {
  			std::vector<std::string>{"192.168.1.19/24"},
  			"192.168.1.1"
  		},
  		ifnc::dto::interface_information::ip_info {
  			std::vector<std::string>{"fe80::9248:9aff:feed:d70f/64"},
  			""
  		}
  	},	
  	ifnc::dto::interface_information {
  		"docker0",
  		ifnc::dto::interface_information::ip_info {
  			std::vector<std::string>{"172.17.0.1/16"},
  			""
  		},
  		ifnc::dto::interface_information::ip_info {
  			std::vector<std::string>{"fe80::42:9eff:fe92:b38a/64"},
  			""
  		}
  	}	
  };

  EXPECT_CALL(mock, get_network_information())
    .WillOnce(Return(mocked_information_no_network))
  	.WillOnce(ThrowStdException())
  	.WillOnce(ThrowUnknownException())
    .WillOnce(Return(mocked_information_two_interfaces));

  // no active interface
  {
  	const char *expected_response = "Content-Type: application/json\r\n\r\n{\n    \"information\": []\n}";
  	std::string actual_response;
  	int r = service.handle_response(actual_response);
  	EXPECT_EQ(r, 0);
  	EXPECT_EQ(actual_response, expected_response);
  }

  // dbus error, probably
  {
  	const char *expected_response = "Content-Type: application/json\r\n\r\n{\n    \"error\": \"Some random Error\"\n}";
  	std::string actual_response;
  	int r = service.handle_response(actual_response);
  	EXPECT_EQ(r, 1);
  	EXPECT_EQ(actual_response, expected_response);
  }

  // unknown error
  {
  	const char *expected_response = "Content-Type: application/json\r\n\r\n{\n    \"error\": \"Unknown error\"\n}";
  	std::string actual_response;
  	int r = service.handle_response(actual_response);
  	EXPECT_EQ(r, 1);
  	EXPECT_EQ(actual_response, expected_response);
  }

  // two active interfaces
  {
  	const char *expected_response = "Content-Type: application/json\r\n\r\n{\n    \"information\": [\n        {\n            \"name\": \"wlan0\",\n            \"ipv4\": {\n                \"addresses\": [\n                    \"192.168.1.19/24\"\n                ],\n                \"default_gateway\": \"192.168.1.1\"\n            },\n            \"ipv6\": {\n                \"addresses\": [\n                    \"fe80::9248:9aff:feed:d70f/64\"\n                ],\n                \"default_gateway\": \"\"\n            }\n        },\n        {\n            \"name\": \"docker0\",\n            \"ipv4\": {\n                \"addresses\": [\n                    \"172.17.0.1/16\"\n                ],\n                \"default_gateway\": \"\"\n            },\n            \"ipv6\": {\n                \"addresses\": [\n                    \"fe80::42:9eff:fe92:b38a/64\"\n                ],\n                \"default_gateway\": \"\"\n            }\n        }\n    ]\n}";
  	std::string actual_response;
  	int r = service.handle_response(actual_response);
  	EXPECT_EQ(r, 0);
  	EXPECT_EQ(actual_response, expected_response);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
