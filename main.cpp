#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include "posix_wrapper.h"

namespace pw = posix_wrapper;

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
    std::string prompt{};
    std::string line{};

    std::string user{};
    std::string homedir{};
    try {
        user = pw::getenv("USER");
        homedir = "/home/" + user;
    } catch (std::runtime_error re) {
        std::cerr << re.what() << std::endl;
    }

    std::string host{};
    try {
        host = pw::gethostname();
    } catch (std::runtime_error re) {
        std::cerr << re.what() << std::endl;
    }

    for (;;) {
        std::string cwd{};
        try {
            cwd = posix_wrapper::getcwd();
        } catch (std::runtime_error re) {
            std::cerr << re.what() << std::endl;
        }

        // replace /home/$USER with a tilde if found in cwd path
        if (homedir == cwd.substr(0, homedir.length()))
            cwd = "~" + cwd.substr(homedir.length());

        prompt = user + "@" + host + ":" + cwd + " $ ";
        std::cout << prompt;

        // read a line from user and break on EOF
        if (!getline(std::cin, line)) break;
        auto tokens = Tokenize(line);

        // No tokens, no action
        if (tokens.size() == 0) continue;

        // exit has to be implemented as a built-in command
        // This is a dead simple implementation. If the first token received from
        // the user is literally "exit", the shell will break out of the loop and
        // terminate as if it received EOF.
        if (tokens.at(0) == "exit") break;

        // cd has to be implemented as a built-in command
        // It is fairly simple to implement so I won't create a separate function for it as it can be done here.
        if (tokens.at(0) == "cd") {
            if (tokens.size() > 2) std::cerr << "cd: too many arguments\n";
            else if (tokens.size() == 1) chdir(std::string("/home/" + user).c_str());
            else chdir(tokens.at(1).c_str());//posix_wrapper::chdir(tokens.at(1)); 
            continue;
        }

        // Create a child process, and call execvp inside it to execute the
        // (non-built-in) command/file previously received from the user
        // Inside it, try to execute the command and print error message and exit the child
        // process on failure
        if (fork() == 0)
            if (posix_wrapper::execvp(tokens.at(0), tokens) == -1) {
                if (tokens.at(0).find('/') == std::string::npos)
                    std::cerr << tokens.at(0) << ": command not found\n";
                else std::cerr << tokens.at(0) << ": no such file or directory\n";
                exit(EXIT_FAILURE);
            }

        // The shell must wait for the child process to exit, otherwise some unwanted behavior happens
        wait(NULL);
    }

    return 0;
}
