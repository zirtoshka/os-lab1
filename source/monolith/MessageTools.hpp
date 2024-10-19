//
// Created by zirtoshka on 19.10.2024.
//
#include <iostream>
#include <string>

namespace monolith {

enum class Color { RED, YELLOW, GREEN, BLUE, MAGENTA, CYAN, RESET };

std::string GetColorCode(Color color);

void PrintColoredMessage(const std::string& message, Color color);

void PrintShellHi(const std::string& message);
void PrintInfo(const std::string& message);
void PrintError(const std::string& message);
}  // namespace monolith