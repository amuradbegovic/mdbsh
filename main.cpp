#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <iomanip>

#include <unistd.h>
#include <sys/wait.h>

#include "posix_wrapper.h"

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

    while (std::cout << prompt && getline(std::cin, line)) {
        auto tokens = Tokenize(line);
        if (tokens.size() == 0) continue;
        if (tokens.at(0) == "exit")
            break;
        
        if (fork() == 0)
            posix_wrapper::execvp(tokens.at(0), tokens);
        wait(NULL);
        //posix_wrapper::execvp(tokens.at(0), std::vector<std::string>(tokens.begin() + 1, tokens.end()));
        /*std::cout << "{ ";
        for (auto token : tokens) std::cout << std::quoted(token) << " ";
        std::cout << "}\n";*/
    }

    return 0;
}
