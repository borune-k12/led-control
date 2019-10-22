#include <gtest/gtest.h>
#include <memory>
#include <drivers/led-driver/led-driver.h>
#include <led/led-base.h>

class LedDriverTest : public ::testing::Test
{
 public:
  std::unique_ptr<LedDriver> led_driver_;
  std::shared_ptr<LedBase> led_;

  void SetUp() override {
    led_ = std::make_shared<LedFake>();
    led_driver_.reset(new LedDriver(led_));
  }

  void TearDown() override {

  }
};

TEST_F(LedDriverTest, TestSetGet) {
  for (const auto& state : {State::State_On, State::State_Off}) {
    ASSERT_TRUE(led_driver_->set_state(state));
    ASSERT_EQ(led_driver_->get_state(), led_->get_state());
    ASSERT_EQ(state, led_->get_state());
  }

  for (const auto& color : {Color::Color_Red, Color::Color_Blue, Color::Color_Green}) {
    ASSERT_TRUE(led_driver_->set_color(color));
    ASSERT_EQ(led_driver_->get_color(), led_->get_color());
    ASSERT_EQ(color, led_->get_color());
  }

  for (const auto& rate : {0, 1, 2, 3, 4, 5}) {
    ASSERT_TRUE(led_driver_->set_rate(rate));
    ASSERT_EQ(led_driver_->get_rate(), led_->get_rate());
    ASSERT_EQ(rate, led_->get_rate());
  }
}

TEST_F(LedDriverTest, TestErrors) {
  led_->set_error(true);
  ASSERT_FALSE(led_driver_->set_state(State::State_On));
  ASSERT_EQ(State::State_Undefined, led_driver_->get_state());

  ASSERT_FALSE(led_driver_->set_color(Color::Color_Red));
  ASSERT_EQ(Color::Color_Undefined, led_driver_->get_color());

  ASSERT_FALSE(led_driver_->set_rate(0));
  ASSERT_EQ(-1, led_driver_->get_rate());
}
