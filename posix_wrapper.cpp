#include <iostream>
#include <string>

#include <unistd.h>

#include "posix_wrapper.h"

int posix_wrapper::execvp(const std::string &file, const std::vector<std::string> &argv) {
    // "longer" than the original vector by one, since there has to be space for
    // // the terminating null pointer
    std::size_t vec_size = argv.size() + 1;
    char **cstring_argv = new char *[vec_size];

    for (int i = 0; i < vec_size - 1; ++i) {
        auto arg = argv.at(i);
        std::size_t str_size = arg.length() + 1;
        cstring_argv[i] = new char[str_size];
        std::copy(arg.begin(), arg.end(), cstring_argv[i]);
    }
    // the string vector that we send to execvp must be terminated by a null pointer!
    cstring_argv[vec_size - 1] = nullptr;
    // these are for debugging purposes
    // // for (int i = 0; i < argv.size(); ++i)
    // // std::cout << i << " " << cstring_argv[i] << std::endl;
    ::execvp(file.c_str(), cstring_argv);

    // On success, a whole new program is loaded into the process and the shell is
    // replaced. Thus, there's no need to check for return value of execvp here.
    // We may immediately proceeed with cleanup and reutrning -1 from our wrapper
    // function since that part of the code won't even be executed if the user
    // command is successfully ran. There is also no need to attempt to clean up
    // if execvp succeeded because everything that our shell allocated will in
    // that case automatically get free'd.

    for (int i = 0; i < vec_size - 1; ++i) // the last NULL pointer doesn't have to be free'd
        delete[] cstring_argv[i];
    delete[] cstring_argv;

    return -1;
}

std::string posix_wrapper::getcwd() {
    constexpr size_t path_max = 4096; // on Linux
    char buf[path_max];
    if (::getcwd(buf, sizeof(buf)) != nullptr) 
        return std::string(buf);
    throw std::runtime_error("couldn't retrieve current working directory");
}

std::string posix_wrapper::getenv(const std::string &name) {
    if (char *buf = ::getenv(name.c_str()); buf != nullptr) 
        return std::string(buf);
    throw std::runtime_error("error retrieving an environment variable");
}

std::string posix_wrapper::gethostname() {
    constexpr size_t host_name_max = 64; // on Linux
    char buf[host_name_max];
    if (::gethostname(buf, host_name_max) == 0)
        return std::string(buf);
    throw std::runtime_error("couldn't retrieve hostname");
}

/*int posix_wrapper::chdir(const std::string &path) {
    return chdir(path.c_str());
}*/
