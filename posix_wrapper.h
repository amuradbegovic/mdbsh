/* 
 * These are C++ wrapper functions for some POSIX API functions and syscallshthat deal with strings
 * What they essentially do is convert C++ strings and vectors to classic C null-terminated strings and "vectors" organized as pointers to pointers or similar so that they can be passed to POSIX API functions.
 * I am not going to write wrappers for all POSIX functions. I am only going to implement wrappers for functions that deal with data types which are nicer to work with in modern C++ than ANSI C. 
*/

#include <vector>
#include <string>

namespace posix_wrapper {
    int execvp(const std::string &file, const std::vector<std::string> &argv);
    std::string getcwd(); 
}
