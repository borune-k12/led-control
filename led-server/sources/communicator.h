#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
#include <memory>
#include <drivers/pipe-driver/pipe-driver-base.h>
#include <led/led-base.h>

#include <map>

using namespace PipeDriver;
using namespace Led;

static const std::map<State, std::string> LED_STATE2STRING = {
  {State::State_On, "on"},
  {State::State_Off, "off"},
};

static const std::map<Color, std::string> LED_COLOR2STRING = {
  {Color::Color_Red, "red"},
  {Color::Color_Green, "green"},
  {Color::Color_Blue, "blue"}
};

static const std::map<std::string, State> LED_STRING2STATE = {
  {"on", State::State_On},
  {"off", State::State_Off},
};

static const std::map<std::string, Color> LED_STRING2COLOR = {
  {"red", Color::Color_Red},
  {"green", Color::Color_Green},
  {"blue", Color::Color_Blue}
};

class LedDriver;

class CommunicatorBase
{
 protected:
  std::unique_ptr<PipeDriverBase> pipe_driver_;
  std::unique_ptr<LedDriver> led_driver_;

 protected:
  CommunicatorBase() = delete;
  explicit CommunicatorBase(const std::shared_ptr<LedBase>& led);

  void parse_message(const std::string& message);

 public:
  virtual ~CommunicatorBase();
  virtual bool start();
  virtual void stop();

  virtual bool is_working() const;
};

class Communicator : public CommunicatorBase
{
 public:
  Communicator() = delete;
  Communicator(const std::shared_ptr<LedBase>& led, const std::string& pipe_file);
};

#endif // COMMUNICATOR_H
