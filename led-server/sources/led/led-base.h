#ifndef LED_BASE_H
#define LED_BASE_H
#include <stdexcept>

namespace Led {

static const int UNDEFINED = -1;

enum class State {
  State_Undefined = UNDEFINED,
  State_On,
  State_Off
};

enum class Color {
  Color_Undefined = UNDEFINED,
  Color_Red,
  Color_Green,
  Color_Blue
};

using Rate = int;

static const Rate RATE_UNDEFINED = UNDEFINED;

class LedBase
{
 protected:
  State state_;
  Color color_;
  Rate rate_;

  bool has_error_;

 public:
  LedBase();
  virtual ~LedBase() = default;

  virtual void set_state(const State state) = 0;
  virtual State get_state() const = 0;

  virtual void set_color(const Color color) = 0;
  virtual Color get_color() const = 0;

  virtual void set_rate(const Rate rate) = 0;
  virtual Rate get_rate() const = 0;

  virtual void set_error(const bool error);

 protected:
  inline void check_has_error() const {
    if (has_error_) {
      throw std::runtime_error("An error has occurred during operation");
    }
  }
};

class LedFake : public LedBase
{
 public:
  void set_state(const State state) override;
  State get_state() const override;

  void set_color(const Color color) override;
  Color get_color() const override;

  void set_rate(const Rate rate) override;
  Rate get_rate() const override;
};
}  // namespace Led

#endif // LED_BASE_H
