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

    bool file_exists(const std::string &path) {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    template <typename T> void LCPP_ERROR(const T &message) {
        std::cout << "[ERROR][LibCfg++]: " << message << std::endl;

        if (TERMINATE_THE_PROGRAM)
            exit(EXIT_FAILURE);
    }

    std::string remove_whitespaces(std::string str) {
        str.erase(remove_if(str.begin(), str.end(),
                            [](char ch) {
                                return std::isspace<char>(
                                    ch, std::locale::classic());
                            }),
                  str.end());

        return str;
    }

    std::string line_trim_comment(const std::string &line) {
        if (line.find('#') != std::string::npos) {
            if (std::count(line.begin(), line.end(), '"') >= 2) {
                std::string output = line;

                for (uint32_t i = 0; i < line.length(); i++) {
                    if ((line[i] == '#' && i > line.find_last_of('"')) ||
                        (line[i] == '#' && i < line.find('"'))) {
                        output = line.substr(0, i);
                        break;
                    }
                }

                return output;
            } else
                return line.substr(0, line.find('#'));
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
                if (file_info.lines[i] == "" &&
                    file_info.lines[i + 1].empty()) {
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
                           !remove_whitespaces(file_info.lines[i - 1])
                                .empty()) {
                    file_info.lines.insert(file_info.lines.begin() + i, "");
                    i++;
                    continue;
                }
            }

            i++;
        }
    }

    // Removes trailing and leading spaces in a string
    std::string remove_tl_whitespaces(const std::string &str) {
        if (str.find_first_not_of(' ') != std::string::npos)
            return str.substr(str.find_first_not_of(' '),
                              str.find_last_not_of(' ') -
                                  str.find_first_not_of(' ') + 1);
        else
            return str;
    }

    std::string get_string_key(const std::string &str) {
        return remove_tl_whitespaces(str.substr(0, str.find('=')));
    }

    std::string get_string_value(const std::string &str) {
        return str.substr(str.find_first_of('"') + 1,
                          (str.find_last_of('"') - str.find_first_of('"')) - 1);
    }

    std::string format_the_line(const std::string &line) {
        if (line_is_string(line))
            return get_string_key(line) + " = \"" + get_string_value(line) +
                   '"';

        return line;
    }

    void format_the_file() {
        bool section_detected = false;

        _void0();

        for (std::string &line : file_info.lines) {
            if (line_is_section(line) && !section_detected)
                section_detected = true;

            if (section_detected && line_is_section(line))
                continue;

            if (section_detected)
                line = "    " + format_the_line(line);
            else
                line = format_the_line(line);
        }
    }

    void scan_the_line_for_type(const std::string &line) {
        if (!line_is_string(line) && !line_is_section(line) && line != "" &&
            line[0] != '#')
            LCPP_ERROR("");
    }

    void parse_file_lines() {
        std::ifstream is(file_info.path);

        std::string line;

        bool section_detected = false;

        while (std::getline(is, line)) {
            if (remove_whitespaces(line).empty())
                line = remove_whitespaces(line);

            line = remove_tl_whitespaces(line);

            scan_the_line_for_type(line);

            if (line_is_section(line) && !section_detected)
                section_detected = true;

            if (section_detected && !line_is_section(line) &&
                remove_whitespaces(line) != "")
                line = "    " + line;

            file_info.lines.push_back(line);
        }

        format_the_file();

        is.close();
    }

    CfgFile::CfgFile() {}

    CfgFile::CfgFile(const std::string &path) { open(path); }

    void CfgFile::open(const std::string &path) {
        if (!file_exists(path))
            LCPP_ERROR("The file called \"" + path +
                           "\" that you want to open does not exist in the "
                           "specified path.");

        if ((path.find('.') != std::string::npos &&
             file_types.find(path.substr(path.find_last_of('.'),
                                         path.length() - path.find('.'))) ==
                 file_types.end()) ||
            (path.find('.') == std::string::npos))
            LCPP_ERROR(
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
            LCPP_ERROR("It is not possible to update the file because it "
                           "is not open.");

        std::ofstream ofs(file_info.path);

        for (const std::string &line : file_info.lines)
            ofs << line << std::endl;

        ofs.close();
    }

    std::string CfgFile::read(const std::string &string_key) {}

    std::string CfgFile::read(const std::string &section_key,
                              const std::string &string_key) {}
} // namespace LibCfgPP