#include "led-base.h"

namespace Led  {

LedBase::LedBase() :
  state_(State::State_Undefined),
  color_(Color::Color_Undefined),
  rate_(RATE_UNDEFINED),
  has_error_(false) {

}

void LedBase::set_error(const bool error) {
  has_error_ = error;
}

void LedFake::set_state(const State state) {
  check_has_error();
  state_ = state;
}

State LedFake::get_state() const {
  check_has_error();
  return state_;
}

void LedFake::set_color(const Color color) {
  check_has_error();
  color_ = color;
}

Color LedFake::get_color() const {
  check_has_error();
  return color_;
}

void LedFake::set_rate(const Rate rate) {
  check_has_error();
  rate_ = rate;
}

Rate LedFake::get_rate() const {
  check_has_error();
  return rate_;
}
}  // namespace Led
