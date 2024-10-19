//
// Created by zirtoshka on 18.10.2024.
//
#include "Shell.hpp"

namespace monolith {

std::vector<std::string> ParseInput(const std::string& input) {
  std::vector<std::string> tokens;
  std::string item;
  std::stringstream ss(input);
  while (ss >> item) {
    tokens.push_back(item);
  }
  return tokens;
}

void ExecuteCommand(const std::vector<std::string>& args) {
  if (args.empty()) {
    PrintError("Emptiness is not a command");
    return;
  }

  char* argv[args.size() + 1];
  for (size_t i = 0; i < args.size(); ++i) {
    argv[i] = const_cast<char*>(args[i].c_str());
  }
  argv[args.size()] = nullptr;  // the last element of argv must be a NULL pointer

  std::chrono::time_point start = std::chrono::high_resolution_clock::now();

  pid_t pid = fork();
  if (pid < -1) {
    PrintError("Fork failed(( Couldn't create a new process");
    return;
  }
  if (pid == 0) {
    // дочерний процесс
    execvp(argv[0], argv);  // cmd, list of args to cmd
    PrintError("Execution failed((");
    exit(EXIT_FAILURE);
  } else {
    // parent process
    int stat_loc;
    waitpid(pid, &stat_loc, 0);

    std::chrono::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    PrintInfo("Program executed in " + std::to_string(elapsed.count()) + " seconds.");
  }
}

int ShellRun() {
  std::string input;

  while (true) {
    PrintShellHi("pupa_shell> ");

    std::getline(std::cin, input);

    if (input == "exit") {
      break;
    }

    // ввод на аргументы
    std::vector<std::string> args = ParseInput(input);

    ExecuteCommand(args);
  }
  return 0;
}

}  // namespace monolith