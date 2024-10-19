#pragma once
#include "MessageTools.hpp"

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <vector>
#include <cstring>

namespace monolith {

std::string HelloWorld();
void ExecuteCommand (const std::vector<std::string>& args);
int ShellRun();
std::vector<std::string> ParseInput(const std::string& input);

}
