#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

#include <unistd.h>

#include "posix_wrapper.h"

void posix_wrapper::execvp(const std::string &file, const std::vector<std::string> &argv) {
    std::size_t vec_size = argv.size() + 1;
    char **cstring_argv = (char **)std::malloc(sizeof(char **) * vec_size);
    for (int i = 0; i < vec_size - 1; ++i) {
        auto arg = argv.at(i);
        std::size_t str_size = arg.length() + 1;
        cstring_argv[i] = (char *)std::malloc(sizeof(char) * str_size);
        std::strncpy(cstring_argv[i], arg.c_str(), str_size);
    }
    cstring_argv[vec_size - 1] = NULL; // must be NULL-terminated !! 

    //for (int i = 0; i < argv.size(); ++i) 
    //    std::cout << i << " " << cstring_argv[i] << std::endl;
    
   
    //if (::execvp(file.c_str(), cstring_argv) == -1) throw std::runtime_error("couldn't run it :(");

    if (::execvp(file.c_str(), cstring_argv) == -1) std::cerr << "couldn't run it :(\n";
    
    for (int i = 0; i < vec_size - 1; ++i) // the last NULL pointer doesn't have to be free'd
        std::free(cstring_argv[i]);
    std::free(cstring_argv);
}

