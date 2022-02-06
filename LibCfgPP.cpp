#include <algorithm>
#include <fstream>
#include <set>
#include <sys/stat.h>
#include <vector>

#include "LibCfgPP.hpp"

using namespace LibCfgPP;

struct file_info {
    std::string path;
    std::vector<std::string> lines;
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

std::string remove_whitespaces(std::string str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

    return str;
}

void remove_extra_empty_lines_in_the_file() {
    bool found_not_empty_line = false;

    LCPP_UINT32 i = 0;

    while (i < file_info.lines.size()) {
        if (file_info.lines[0].empty()) {
            file_info.lines.erase(file_info.lines.begin());
            continue;
        }

        if (!file_info.lines[i].empty())
            found_not_empty_line = true;

        if (found_not_empty_line) {
            if (file_info.lines[i] == "" && file_info.lines[i - 1] != "" &&
                file_info.lines[i + 1] == "") {
                file_info.lines.erase(file_info.lines.begin() + (i + 1));
                continue;
            }
            if (file_info.lines.back().empty()) {
                file_info.lines.erase(file_info.lines.end());
                continue;
            }
        }

        i++;
    }
}

void format_the_file() { remove_extra_empty_lines_in_the_file(); }

void parse_file_lines() {
    std::ifstream is(file_info.path);

    std::string line;

    while (std::getline(is, line)) {
        if (remove_whitespaces(line) == "")
            line = remove_whitespaces(line);

        file_info.lines.push_back(line);
    }

    format_the_file();

    for (const std::string &str : file_info.lines)
        printf("%s\n", str.c_str());

    is.close();
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

    file_info.path = path;

    parse_file_lines();
}

void CfgFile::close() { file_info.path.empty(); }