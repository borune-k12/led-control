#include <gtest/gtest.h>
#include <communicator.h>
#include <drivers/led-driver/led-driver.h>

class CommunicatorForTest : public CommunicatorBase
{
 public:
  CommunicatorForTest(const std::shared_ptr<LedBase> led) :
  CommunicatorBase(led) {
    pipe_driver_.reset(new PipeDriverFake(std::bind(&CommunicatorForTest::parse_message, this, std::placeholders::_1)));
  }

  void write(const std::string& command) {
    auto pipe_driver_fake = dynamic_cast<PipeDriverFake*>(pipe_driver_.get());
    pipe_driver_fake->write_message(command);
  }

  std::string pop_last_output() {
    auto pipe_driver_fake = dynamic_cast<PipeDriverFake*>(pipe_driver_.get());
    const auto& front = pipe_driver_fake->output_.front();
    pipe_driver_fake->output_.pop();
    return front;
  }

  State get_led_state() const {
    return led_driver_->get_state();
  }

  Color get_led_color() const {
    return led_driver_->get_color();
  }

  Rate get_led_rate() const {
    return led_driver_->get_rate();
  }
};

class CommunicatorTest : public testing::Test
{
 public:
  std::unique_ptr<CommunicatorForTest> communicator_;
  std::shared_ptr<LedBase> led_;

  template<class T>
  void test_command(const std::string& what, const std::map<std::string, T>& values, const std::function<T()>& get_function, const bool has_error) {
    for (const auto& value : values) {
      communicator_->write("set-led-" + what + " " + value.first);
      ASSERT_EQ(get_function(), has_error ? static_cast<T>(UNDEFINED) : value.second);
      ASSERT_EQ(has_error ? "FAILED" : "OK", communicator_->pop_last_output());

      communicator_->write("get-led-" + what);
      ASSERT_EQ(has_error ? "FAILED" : "OK " + value.first, communicator_->pop_last_output());
    }
  }
};

TEST_F(CommunicatorTest, TestCommands) {
  led_ = std::make_shared<LedFake>();
  communicator_.reset(new CommunicatorForTest(led_));
  communicator_->start();

  std::map<std::string, Rate> string2rates;

  for (Rate rate = MIN_RATE; rate <= MAX_RATE; ++rate) {
    string2rates[std::to_string(rate)] = rate;
  }

  for (const auto& has_error : {true, false}) {
    led_->set_error(has_error);
    test_command<State>("state", LED_STRING2STATE, [this]() -> State {return communicator_->get_led_state();}, has_error);
    test_command<Color>("color", LED_STRING2COLOR, [this]() -> Color {return communicator_->get_led_color();}, has_error);
    test_command<Rate>("rate", string2rates, [this]() -> Rate {return communicator_->get_led_rate();}, has_error);
  }
}
