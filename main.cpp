#include <csignal>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include "posix_wrapper.h"

// TODO: implement proper tokenizer. This is crucial to support pipes and
// redirection later on
std::vector<std::string> Tokenize(const std::string &line) {
  std::vector<std::string> tokens;
  std::istringstream line_stream(line);
  std::string token;
  while (line_stream >> token)
    tokens.push_back(token);

  return tokens;
}

int main() {
  std::string prompt = "$ ";
  std::string line;

  while (getline(std::cin, line)) {
    try {
      prompt = posix_wrapper::getcwd() + " $ ";
    } catch (std::runtime_error re) {
      std::cerr << "error: couldn't get current working directory\n";
    }
    std::cout << prompt;

    auto tokens = Tokenize(line);

    // No tokens, no action
    if (tokens.size() == 0)
      continue;

    // exit has to be implemented as a built-in command
    // This is a dead simple implementation. If the first token received from
    // the user is literally "exit", the shell will break out of the loop and
    // terminate as if it received EOF.
    if (tokens.at(0) == "exit")
      break;

    // Create a child process, and call execvp inside it to execute the
    // (non-built-in) command/file previously received from the user
    if (fork() == 0)
      // Try to execute the command and print error message and exit the child
      // process on failure
      if (posix_wrapper::execvp(tokens.at(0), tokens) == -1) {
        if (tokens.at(0).find('/') == std::string::npos)
          std::cerr << tokens.at(0) << ": command not found\n";
        else
          std::cerr << tokens.at(0) << ": no such file or directory\n";
        exit(EXIT_FAILURE);
      }

    // The shell must wait for the child process to exit, otherwise some
    // unwanted behavior happens
    wait(NULL);
  }

  return 0;
}
