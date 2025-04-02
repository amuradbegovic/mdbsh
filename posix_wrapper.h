// C++ wrappers for some POSIX API functions and syscalls that deal with strings

#include <vector>
#include <string>
#include <stdexcept>

namespace posix_wrapper {
    void execvp(const std::string &file, const std::vector<std::string> &argv);
    std::string getcwd();
}
