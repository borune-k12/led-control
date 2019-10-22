#include "communicator.h"
#include <vector>
#include <sstream>
#include <iterator>
#include <drivers/led-driver/led-driver.h>
#include <drivers/pipe-driver/pipe-driver.h>

enum class MessageType
{
  MessageType_SetState,
  MessageType_GetState,
  MessageType_SetColor,
  MessageType_GetColor,
  MessageType_SetRate,
  MessageType_GetRate
};

static const std::map<std::string, MessageType> MESSAGES = {
  {"set-led-state", MessageType::MessageType_SetState},
  {"get-led-state", MessageType::MessageType_GetState},
  {"set-led-color", MessageType::MessageType_SetColor},
  {"get-led-color", MessageType::MessageType_GetColor},
  {"set-led-rate", MessageType::MessageType_SetRate},
  {"get-led-rate", MessageType::MessageType_GetRate}
};

static std::vector<std::string> split(const std::string& input) {
  std::istringstream iss(input);
  std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                   std::istream_iterator<std::string>());
  return results;
}

CommunicatorBase::CommunicatorBase(const std::shared_ptr<LedBase>& led) :
  led_driver_(new LedDriver(led)) {

}

CommunicatorBase::~CommunicatorBase() {
  stop();
}

bool CommunicatorBase::start() {
  try {
    pipe_driver_->start();
  } catch (const std::exception&) {

  }
  return is_working();
}

void CommunicatorBase::stop() {
  pipe_driver_->stop();
}

bool CommunicatorBase::is_working() const {
  return pipe_driver_->is_working();
}

void CommunicatorBase::parse_message(const std::string& message) {
  const std::vector<std::string>& tokens = split(message);

  const std::string& command = tokens.at(0);

  const auto& it = MESSAGES.find(command);
  if (it == MESSAGES.end()) {
    printf("invalid command: '%s'\n", command.c_str());
    return;
  }

  switch (it->second) {
  case MessageType::MessageType_SetState: {
    if (tokens.size() == 2) {
      const auto& it1 = LED_STRING2STATE.find(tokens.at(1));
      if (it1 != LED_STRING2STATE.end()) {
        const bool success = led_driver_->set_state(it1->second);
        pipe_driver_->write(success ? "OK" : "FAILED");
      } else {
        pipe_driver_->write("FAILED");
      }
    } else {
      printf("invalid command format\n");
    }
    break;
  }
  case MessageType::MessageType_GetState: {
    const State& state = led_driver_->get_state();
    const auto& it1 = LED_STATE2STRING.find(state);
    pipe_driver_->write(it1 != LED_STATE2STRING.end() ? "OK " + it1->second : "FAILED");
    break;
  }
  case MessageType::MessageType_SetColor: {
    if (tokens.size() == 2) {
      const auto& it1 = LED_STRING2COLOR.find(tokens.at(1));
      if (it1 != LED_STRING2COLOR.end()) {
        const bool success = led_driver_->set_color(it1->second);
        pipe_driver_->write(success ? "OK" : "FAILED");
      } else {
        pipe_driver_->write("FAILED");
      }
    } else {
      printf("invalid command format\n");
    }
    break;
  }
  case MessageType::MessageType_GetColor: {
    const Color& color = led_driver_->get_color();
    const auto& it1 = LED_COLOR2STRING.find(color);
    pipe_driver_->write(it1 != LED_COLOR2STRING.end() ? "OK " + it1->second : "FAILED");
    break;
  }
  case MessageType::MessageType_SetRate: {
    if (tokens.size() == 2) {
        const bool success = led_driver_->set_rate(std::stoi(tokens.at(1)));
        pipe_driver_->write(success ? "OK" : "FAILED");
    } else {
      printf("invalid command format\n");
    }
    break;
  }
  case MessageType::MessageType_GetRate: {
    const Rate rate = led_driver_->get_rate();
    pipe_driver_->write(rate != RATE_UNDEFINED ? "OK " + std::to_string(rate) : "FAILED");
    break;
  }
  }
}

Communicator::Communicator(const std::shared_ptr<LedBase>& led, const std::string& pipe_file) :
CommunicatorBase(led) {
  pipe_driver_.reset(new PipeDriverReal(std::bind(&Communicator::parse_message, this, std::placeholders::_1), pipe_file));
}
