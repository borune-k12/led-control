#include "led-driver.h"

LedDriver::LedDriver(const std::shared_ptr<LedBase>& led) :
  led_(led) {

}

bool LedDriver::set_state(const State state) noexcept {
  bool success = false;
  if (led_) {
    try {
      led_->set_state(state);
      success = true;
    } catch (const std::exception&) {

    }
  }
  return success;
}

State LedDriver::get_state() const noexcept {
  State state = State::State_Undefined;
  if (led_) {
    try {
      state = led_->get_state();
    } catch (const std::exception&) {

    }
  }
  return state;
}

bool LedDriver::set_color(const Color color) noexcept {
  bool success = false;
  if (led_) {
    try {
      led_->set_color(color);
      success = true;
    } catch (const std::exception&) {

    }
  }
  return success;
}

Color LedDriver::get_color() const noexcept {
  Color color = Color::Color_Undefined;
  if (led_) {
    try {
      color = led_->get_color();
    } catch (const std::exception&) {

    }
  }
  return color;
}

bool LedDriver::set_rate(const Rate rate) noexcept {
  bool success = false;
  if (led_ && is_rate_valid(rate)) {
    try {
      led_->set_rate(rate);
      success = true;
    } catch (const std::exception&) {

    }
  }
  return success;
}

Rate LedDriver::get_rate() const noexcept {
  Rate rate = RATE_UNDEFINED;
  if (led_) {
    try {
      rate = led_->get_rate();
    } catch (const std::exception&) {

    }
  }
  return rate;
}

bool LedDriver::is_rate_valid(const Rate rate) noexcept {
  return (rate >= MIN_RATE && rate <= MAX_RATE);
}
