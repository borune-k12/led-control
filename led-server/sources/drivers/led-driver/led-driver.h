#ifndef LED_DRIVER_H
#define LED_DRIVER_H
#include <led/led-base.h>
#include <memory>

using namespace Led;

const Rate MIN_RATE = 0;
const Rate MAX_RATE = 5;

class LedDriver {
 private:
  std::shared_ptr<LedBase> led_;

 public:
  explicit LedDriver(const std::shared_ptr<LedBase>& led);
  LedDriver(const LedDriver& other) = delete;
  LedDriver(LedDriver&& other) = delete;
  LedDriver& operator=(const LedDriver& other) = delete;
  LedDriver& operator=(LedDriver&& other) = delete;

  virtual ~LedDriver() = default;

  bool set_state(const State state) noexcept;
  State get_state() const noexcept;

  bool set_color(const Color color) noexcept;
  Color get_color() const noexcept;

  bool set_rate(const Rate rate) noexcept;
  Rate get_rate() const noexcept;

 private:
  bool is_rate_valid(const Rate rate) noexcept;
};

#endif // LED_DRIVER_H
