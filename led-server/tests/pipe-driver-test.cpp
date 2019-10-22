#include <gtest/gtest.h>
#include <drivers/pipe-driver/pipe-driver.h>
#include <memory>
#include <future>

static bool file_exists(const std::string& filename) {
  struct stat buffer;
  return stat(filename.c_str(), &buffer) == 0;
}

static const std::string pipe_path = "/tmp/test.pipe";
static const std::string invalid_pipe_path = "/test.pipe";
static const std::string test_message = "test message";

class PipeDriverTest : public ::testing::Test
{
 public:
  std::unique_ptr<PipeDriverReal> pipe_driver_;
  std::promise<const std::string&> promise_;

 public:
  void handle_message(const std::string& message) {
    promise_.set_value(std::move(message));
  }
};

TEST_F(PipeDriverTest, TestReadWrite) {
  pipe_driver_.reset(new PipeDriverReal(std::bind(&PipeDriverTest::handle_message, this, std::placeholders::_1), pipe_path));
  pipe_driver_->start();

  pipe_driver_->write(test_message);

  auto future = promise_.get_future();
  ASSERT_EQ(future.wait_for(std::chrono::seconds(1)), std::future_status::ready);

  const auto& message = future.get();
  ASSERT_EQ(message, test_message);
}

TEST_F(PipeDriverTest, TestPipeRemoved) {
  remove(pipe_path.c_str());
  ASSERT_FALSE(file_exists(pipe_path));

  pipe_driver_.reset(new PipeDriverReal([](const std::string&){}, pipe_path));
  pipe_driver_->start();
  ASSERT_TRUE(file_exists(pipe_path));

  pipe_driver_->stop();
  ASSERT_FALSE(file_exists(pipe_path));
}

TEST_F(PipeDriverTest, TestExistingPipe) {
  if (mkfifo(pipe_path.c_str(), 0664) == 0) {
    ASSERT_TRUE(file_exists(pipe_path));

    pipe_driver_.reset(new PipeDriverReal([](const std::string&){}, pipe_path));
    pipe_driver_->start();
    pipe_driver_->stop();
    ASSERT_TRUE(file_exists(pipe_path));

    remove(pipe_path.c_str());
  }
}

TEST_F(PipeDriverTest, TestInvalidPipePath) {
  pipe_driver_.reset(new PipeDriverReal([](const std::string&){}, invalid_pipe_path));
  ASSERT_THROW(pipe_driver_->start(), std::runtime_error);
}
