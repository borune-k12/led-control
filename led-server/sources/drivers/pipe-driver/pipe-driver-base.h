#ifndef PIPE_DRIVER_BASE_H
#define PIPE_DRIVER_BASE_H
#include <string>
#include <functional>
#include <atomic>
#include <queue>

namespace PipeDriver {

using callback_function = std::function<void(const std::string&)>;

class PipeDriverBase
{
 protected:
  std::atomic_bool working_;
  callback_function callback_;

 public:
  PipeDriverBase() = delete;

  explicit PipeDriverBase(callback_function callback) :
  working_(false),
  callback_(callback) {
  }

  virtual ~PipeDriverBase() = default;

  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void write(const std::string& message) const = 0;

  inline virtual bool is_working() const {
    return working_;
  }
};

class PipeDriverFake : public PipeDriverBase {

 public:
  mutable std::queue<std::string> output_;

 public:
  PipeDriverFake() = delete;

  explicit PipeDriverFake(callback_function callback) :
    PipeDriverBase(callback) {
    stop();
  }

  virtual ~PipeDriverFake() = default;

  inline void start() final {
    working_ = true;
  }

  inline void stop() final {
    working_ = false;
  }

  inline void write(const std::string& message) const final {
    output_.emplace(message);
  }

  inline void write_message(const std::string& message) const {
    if (working_) {
      callback_(message);
    }
  }

  inline std::queue<std::string> get_output() const {
    return output_;
  }
};
}  // namespace PipeDriver

#endif // PIPE_DRIVER_BASE_H
