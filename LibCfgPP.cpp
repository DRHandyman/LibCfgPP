#include <algorithm>
#include <fstream>
#include <set>
#include <sys/stat.h>
#include <vector>

#include "LibCfgPP.hpp"

using namespace LibCfgPP;

struct file_info {
    LCPP_STRING path;
    std::vector<std::string> lines;
    LCPP_BOOL is_open = false;
} file_info;

enum { TERMINATE_THE_PROGRAM = 1 };

const std::set<std::string> file_types = {".cfg", ".conf", ".config"};

LCPP_BOOL file_exists(const LCPP_STRING &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

void print_an_error(const LCPP_STRING &message) {
    printf("[ERROR][LibCfg++]: %s\n", message.c_str());

    if (TERMINATE_THE_PROGRAM)
        exit(EXIT_FAILURE);
}

LCPP_STRING remove_whitespaces(LCPP_STRING str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

    return str;
}

void remove_extra_empty_lines_in_the_file() {
    LCPP_BOOL found_not_empty_line = false;

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

    LCPP_STRING line;

    while (std::getline(is, line)) {
        if (remove_whitespaces(line) == "")
            line = remove_whitespaces(line);

        file_info.lines.push_back(line);
    }

    format_the_file();

    is.close();
}

CfgFile::CfgFile() {}

CfgFile::CfgFile(const LCPP_STRING &path) { open(path); }

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
    file_info.is_open = true;

    parse_file_lines();

    update_file();
}

void CfgFile::close() {
    file_info.path.clear();
    file_info.is_open = false;
}

LCPP_BOOL CfgFile::is_open() { return file_info.is_open; }

void CfgFile::update_file() {
    if (!is_open())
        print_an_error(
            "It is not possible to update the file because it is not open.");

    std::ofstream ofs(file_info.path);

    for (const std::string &line : file_info.lines)
        ofs << line << std::endl;

    ofs.close();
}