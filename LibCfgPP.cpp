#include <fstream>
#include <set>
#include <sys/stat.h>

#include "LibCfgPP.hpp"

using namespace LibCfgPP;

struct file_info {
    std::string path;
} file_info;

enum { TERMINATE_THE_PROGRAM = 1 };

const std::set<std::string> file_types = {".cfg", ".conf", ".config"};

bool file_exists(const std::string &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

void print_an_error(const std::string &message) {
    printf("[ERROR][LibCfg++]: %s\n", message.c_str());

    if (TERMINATE_THE_PROGRAM)
        exit(EXIT_FAILURE);
}

CfgFile::CfgFile() {}

CfgFile::CfgFile(const std::string &path) { open(path); }

void CfgFile::open(const std::string &path) {
    if (!file_exists(path))
        print_an_error(
            "The file called \"" + path +
            "\" that you want to open does not exist in the specified path.");
    if ((path.find('.') != std::string::npos &&
         file_types.find(
             path.substr(path.find('.'), path.length() - path.find('.'))) ==
             file_types.end()) ||
        (path.find('.') == std::string::npos))
        print_an_error("The type of file you are trying to open should be one "
                       "of the following: \".cfg\", \".conf\" or \".config\".");
}

void CfgFile::close() {}