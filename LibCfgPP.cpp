#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sys/stat.h>
#include <vector>

#include "LibCfgPP.hpp"

namespace LibCfgPP {
    struct file_info {
        std::string path;
        std::vector<std::string> lines;
        bool is_open = false;
    } file_info;

    enum { TERMINATE_THE_PROGRAM = 1 };

    const std::set<std::string> file_types = {".cfg", ".conf", ".config"};

    template <typename T> void LCPP_LOG(const T &message) {
        std::cout << "LCPP_LOG: " << message << std::endl;
    }

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

    std::string line_trim_comment(const std::string &line) {
        if (std::count(line.begin(), line.end(), '"') >= 2) {
            if (line.find('#') != std::string::npos) {
                std::string output = line;

                for (uint32_t i = 0; i < line.length(); i++) {
                    if ((line[i] == '#' && i > line.find_last_of('"')) ||
                        (line[i] == '#' && i < line.find('"'))) {
                        output = line.substr(0, i);
                        break;
                    }
                }

                return output;
            }
        }

        return line;
    }

    bool line_is_string(std::string line) {
        line = remove_whitespaces(line_trim_comment(line));

        if (line.find('=') != std::string::npos &&
            line.find('"') != std::string::npos &&
            count(line.begin(), line.end(), '"') >= 2 &&
            line.find('=') < line.find('"') &&
            line[line.find('"') - 1] == '=' &&
            line.find_last_of('"') + 1 > line.length() - 1)
            return true;
        else
            return false;

        return false;
    }

    bool line_is_section(std::string str) {
        str = remove_whitespaces(line_trim_comment(str));

        return str[0] == '[' && str.back() == ']';
    }

    // Adds or deletes empty lines in the file.
    void _void0() {
        bool found_not_empty_line = false;

        uint32_t i = 0;

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
                if (line_is_section(file_info.lines[i]) &&
                    file_info.lines[i + 1] == "" &&
                    i + 1 < file_info.lines.size()) {
                    file_info.lines.erase(file_info.lines.begin() + (i + 1));
                    continue;
                } else if (line_is_section(file_info.lines[i]) &&
                           (int64_t)i - 1 >= 0 &&
                           file_info.lines[i - 1] != "") {
                    file_info.lines.insert(file_info.lines.begin() + i, "");
                    i++;
                    continue;
                }
            }

            i++;
        }
    }

    void format_the_file() { _void0(); }

    void parse_file_lines() {
        std::ifstream is(file_info.path);

        std::string line;

        while (std::getline(is, line)) {
            if (remove_whitespaces(line) == "")
                line = remove_whitespaces(line);

            file_info.lines.push_back(line);
        }

        format_the_file();

        is.close();
    }

    CfgFile::CfgFile() {}

    CfgFile::CfgFile(const std::string &path) { open(path); }

    void CfgFile::open(const std::string &path) {
        if (!file_exists(path))
            print_an_error("The file called \"" + path +
                           "\" that you want to open does not exist in the "
                           "specified path.");

        if ((path.find('.') != std::string::npos &&
             file_types.find(path.substr(path.find_last_of('.'),
                                         path.length() - path.find('.'))) ==
                 file_types.end()) ||
            (path.find('.') == std::string::npos))
            print_an_error(
                "The type of file you are trying to open should be one "
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

    bool CfgFile::is_open() { return file_info.is_open; }

    void CfgFile::update_file() {
        if (!is_open())
            print_an_error("It is not possible to update the file because it "
                           "is not open.");

        std::ofstream ofs(file_info.path);

        for (const std::string &line : file_info.lines)
            ofs << line << std::endl;

        ofs.close();
    }
} // namespace LibCfgPP