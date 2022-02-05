#include "LibCfgPP.hpp"

using namespace LibCfgPP;

enum { TERMINATE_THE_PROGRAM = 1 };

void print_an_error(const std::string &message) {
    if (TERMINATE_THE_PROGRAM)
        exit(EXIT_FAILURE);
}

CfgFile::CfgFile() {}

CfgFile::CfgFile(const std::string &path) {}

void CfgFile::open(const std::string &path) {}

void CfgFile::close() {}