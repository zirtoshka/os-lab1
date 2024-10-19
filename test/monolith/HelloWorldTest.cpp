#include <gtest/gtest.h>

#include "monolith/Shell.hpp"

namespace monolith {


std::string CaptureOutput(const std::string& command) {
  std::ostringstream oss;
  std::ostringstream err_oss;

  std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
  std::streambuf* old_cerr = std::cerr.rdbuf(err_oss.rdbuf());

  std::istringstream input_stream(command + "\nexit\n");
  std::streambuf* old_cin = std::cin.rdbuf(input_stream.rdbuf());

  ShellRun();

  std::cin.rdbuf(old_cin);
  std::cout.rdbuf(old_cout);
  std::cerr.rdbuf(old_cerr);

  return err_oss.str() + oss.str();
}

TEST(ShellTest, EmptyCommandError) {
  std::string input = "";
  std::string output = CaptureOutput(input);
  ASSERT_NE(output.find("Emptiness is not a command"), std::string::npos);
}

TEST(ShellTest, ExecutionTimeMeasurement) {
  std::string output = CaptureOutput("sleep 1");

  ASSERT_NE(output.find("Program executed in"), std::string::npos);
}

}  // namespace monolith
