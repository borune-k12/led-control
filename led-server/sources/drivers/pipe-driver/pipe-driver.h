#ifndef PIPE_DRIVER_H
#define PIPE_DRIVER_H
#include "pipe-driver-base.h"
#include <fstream>
#include <thread>

using namespace PipeDriver;

class PipeDriverReal : public PipeDriverBase
{
 private:
  const std::string pipe_file_;
  bool need_remove_pipe_;

  std::unique_ptr<std::thread> read_thread_;

 public:
  PipeDriverReal() = delete;
  PipeDriverReal(const PipeDriverReal&) = delete;
  PipeDriverReal(PipeDriverReal&& other) = delete;
  PipeDriverReal& operator=(const PipeDriverReal& other) = delete;
  PipeDriverReal& operator=(PipeDriverReal&& other) = delete;

  PipeDriverReal(callback_function callback, const std::string& pipe_file);
  virtual ~PipeDriverReal();

  void start() final;
  void stop() final;

  void write(const std::string& message) const final;

 private:
  void thread_func();
};

#endif // PIPE_DRIVER_H
