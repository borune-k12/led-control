#include "pipe-driver.h"
#include <sys/stat.h>
#include <cstring>

static const std::string EXIT_COMMAND = "exit";

PipeDriverReal::PipeDriverReal(callback_function callback, const std::string &pipe_file) :
  PipeDriverBase(callback),
  pipe_file_(pipe_file),
  need_remove_pipe_(false),
  read_thread_(nullptr) {

}

PipeDriverReal::~PipeDriverReal() {
  if (working_) {
    stop();
  }
}

void PipeDriverReal::start() {
  if (working_)
    return;

  struct stat buffer;
  if (stat(pipe_file_.c_str(), &buffer) != 0) {
    if (mkfifo(pipe_file_.c_str(), 0664) != 0) {
      throw std::runtime_error("Cannot create pipe. Error: " + std::string(strerror(errno)));
    }
    need_remove_pipe_ = true;
  }

  working_ = true;
  read_thread_.reset(new std::thread(&PipeDriverReal::thread_func, this));
}

void PipeDriverReal::stop() {
  if (read_thread_) {
    write(EXIT_COMMAND);
    read_thread_->join();
  }

  if (need_remove_pipe_) {
    remove(pipe_file_.c_str());
  }

  working_ = false;
}

void PipeDriverReal::write(const std::string& message) const {
  auto pipe = std::ofstream(pipe_file_.c_str(), std::ios::out);
  pipe << message << std::endl;
  pipe.close();
}

void PipeDriverReal::thread_func() {
  while (working_) {
    auto pipe = std::ifstream(pipe_file_.c_str(), std::ios::in);
    std::string message;
    if (std::getline(pipe, message)) {
      pipe.close();
      if (message == EXIT_COMMAND) {
        break;
      }
      callback_(message);
    }
  }
}
