#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <backend/network_client.h>

using namespace testing;

struct backend_mock: ifnc::backend::network_client
{
  MOCK_METHOD0(get_network_information, ifnc::dto::info());
};

TEST(Service, EmptyMessage)
{
  backend_mock mock;

  EXPECT_CALL(mock, get_network_information())
    .WillOnce(Return(
      ifnc::dto::info{"192.168.1.123", "192.168.1.0", "192.168.1.1"}));

  EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
