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

  ifnc::dto::info mocked_information {};

  EXPECT_CALL(mock, get_network_information())
    .WillOnce(Return(mocked_information));

  EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
