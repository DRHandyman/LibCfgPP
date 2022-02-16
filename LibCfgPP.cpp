#include <algorithm>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "LibCfgPP.hpp"

namespace LibCfgPP {
    enum { LCPP_DEFAULT_ERROR = -1 };

    constexpr const char *file_types[3] = {".cfg", ".conf", ".config"};
    constexpr const char forbidden_characters[3] = {'[', ']', '"'};

    bool file_exists(const std::string &path) {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    void LCPP_ERROR(const std::string &message, const int &line_id) {
        if (line_id == LCPP_DEFAULT_ERROR)
            throw LCPPException("[\x1B[31mERROR\033[0m][LibCfg++]: " + message);
        else if (line_id > LCPP_DEFAULT_ERROR)
            throw LCPPException("[\x1B[31mERROR\033[0m][LibCfg++][" +
                                std::to_string(line_id) + "]: " + message);
    }

    std::string remove_whitespaces(std::string str) {
        str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

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
    void _void0(std::vector<std::string> &lines) {
        bool found_not_empty_line = false;

        uint32_t i = 0;

        while (i < lines.size()) {
            if (lines[0].empty()) {
                lines.erase(lines.begin());
                continue;
            }

            if (!lines[i].empty())
                found_not_empty_line = true;

            if (found_not_empty_line) {
                if (lines[i] == "" && lines[i + 1].empty()) {
                    lines.erase(lines.begin() + (i + 1));
                    continue;
                }
                if (lines.back().empty()) {
                    lines.erase(lines.end());
                    continue;
                }
                if (line_is_section(lines[i]) && lines[i + 1] == "" &&
                    i + 1 < lines.size()) {
                    lines.erase(lines.begin() + (i + 1));
                    continue;
                } else if (line_is_section(lines[i]) && (int64_t)i - 1 >= 0 &&
                           !remove_whitespaces(lines[i - 1]).empty()) {
                    lines.insert(lines.begin() + i, "");
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

    std::string get_line_comment(const std::string &line) {
        if (line.find('#') != std::string::npos) {
            if (std::count(line.begin(), line.end(), '#') >= 2) {
                for (uint32_t i = 0; i < line.length(); i++) {
                    if ((line[i] == '#' && i > line.find_last_of('#')) ||
                        (line[i] == '#' && i < line.find('"')))
                        return line.substr(i, line.length() - i);
                }
            } else
                return line.substr(line.find('#'),
                                   line.length() - line.find('#'));
        }

        return "";
    }

    std::string get_section_key(const std::string &section) {
        return section.substr(section.find('[') + 1,
                              (section.find_last_of(']') - section.find('[')) -
                                  1);
    }

    std::string format_the_line(const std::string &line) {
        if (line_is_string(line))
            return get_string_key(line) + " = \"" + get_string_value(line) +
                   '"';
        else if (line_is_section(line))
            return '[' + get_section_key(line) + "] " + get_line_comment(line);

        return line;
    }

    void format_the_file(std::vector<std::string> &lines) {
        bool section_detected = false;

        _void0(lines);

        for (std::string &line : lines) {
            if (line_is_section(line) && !section_detected)
                section_detected = true;

            if (section_detected && line_is_section(line)) {
                line = format_the_line(line);
                continue;
            }

            if (section_detected)
                line = "    " + format_the_line(line) + ' ' +
                       (line[0] != '#' ? get_line_comment(line) : "");
            else
                line = format_the_line(line) + ' ' +
                       (line[0] != '#' ? get_line_comment(line) : "");
        }
    }

    void scan_the_line_for_type(const std::string &line,
                                const uint32_t &line_id) {
        if (!line_is_string(line) && !line_is_section(line) && line != "" &&
            line[0] != '#')
            LCPP_ERROR("Unidentified line type.", line_id);
    }

    void parse_file_lines(const std::string &file_path,
                          std::vector<std::string> &lines) {
        uint32_t i = 0;

        std::ifstream is(file_path);

        std::string line;

        bool section_detected = false;

        while (std::getline(is, line)) {
            if (remove_whitespaces(line).empty())
                line = remove_whitespaces(line);

            i++;

            line = remove_tl_whitespaces(line);

            scan_the_line_for_type(line, i);

            if (line_is_section(line) && !section_detected)
                section_detected = true;

            if (section_detected && !line_is_section(line) &&
                remove_whitespaces(line) != "")
                line = "    " + line;

            lines.push_back(line);
        }

        format_the_file(lines);

        is.close();
    }

    CfgFile::CfgFile(const std::string &path) { open(path); }

    void CfgFile::open(const std::string &path) {
        if (!file_exists(path))
            LCPP_ERROR("The file called \"" + path +
                           "\" that you want to open does not exist in the "
                           "specified path.",
                       LCPP_DEFAULT_ERROR);

        bool _bool = std::find(std::begin(file_types), std::end(file_types),
                               path.substr(path.find(
                                   '.', path.length() - path.find('.') - 1))) !=
                     std::end(file_types);

        if ((path.find('.') != std::string::npos && !_bool) ||
            (path.find('.') == std::string::npos))
            LCPP_ERROR("The type of file you are trying to open should be one "
                       "of the following: \".cfg\", \".conf\" or \".config\".",
                       LCPP_DEFAULT_ERROR);

        this->path = path;
        _is_open = true;

        parse_file_lines(path, lines);

        update();
    }

    void CfgFile::close() {
        lines.clear();
        path.clear();
    }

    bool CfgFile::is_open() { return _is_open; }

    void CfgFile::update() {
        if (!is_open())
            LCPP_ERROR("It is not possible to update the file because it "
                       "is not open.",
                       LCPP_DEFAULT_ERROR);

        std::ofstream ofs(path);

        for (const std::string &line : lines)
            ofs << line << std::endl;

        ofs.close();
    }

    std::string CfgFile::read(const std::string &string_key) {
        bool found_string = false;

        std::string output;

        for (const std::string &line : lines) {
            if (line_is_section(line))
                break;
            if (line_is_string(line) && get_string_key(line) == string_key) {
                output = get_string_value(line);
                found_string = true;
            }
        }

        if (!found_string)
            LCPP_ERROR("It is impossible to find the string under the key \"" +
                           string_key + "\" in the file.",
                       LCPP_DEFAULT_ERROR);

        return output;
    }

    std::string CfgFile::read(const std::string &section_key,
                              const std::string &string_key) {
        bool section_detected = false, found_string = false,
             found_section = false;

        std::string output;

        for (const std::string &line : lines) {
            if (line_is_section(line) && get_section_key(line) == section_key) {
                section_detected = true;
                found_section = true;
                continue;
            } else if (line_is_section(line) &&
                       get_section_key(line) != section_key)
                section_detected = false;

            if (section_detected && line_is_string(line) &&
                get_string_key(line) == string_key) {
                output = get_string_value(line);
                found_string = true;
            }
        }

        if (!found_section)
            LCPP_ERROR("It is impossible to find the section under the key \"" +
                           section_key + "\" in the file.",
                       LCPP_DEFAULT_ERROR);
        else if (!found_string)
            LCPP_ERROR("It is impossible to find a string under the key \"" +
                           string_key + "\" in the \"" + section_key +
                           "\" section",
                       LCPP_DEFAULT_ERROR);

        return output;
    }

    void CfgFile::change_the_value(const std::string &string_key,
                                   const std::string &value) {
        int i, id = -1;

        for (auto it = lines.begin(); it != lines.end(); it++) {
            i = it - lines.begin();

            if (line_is_section(lines[i]))
                break;
            if (line_is_string(lines[i]) &&
                get_string_key(lines[i]) == string_key)
                id = i;
        }

        if (id != -1) {
            lines[id] = get_string_key(lines[id]) + " = \"" + value + "\" " +
                        get_line_comment(lines[id]);
            printf("%s\n", lines[id].c_str());
        } else
            LCPP_ERROR("It is not possible to change the value of the string, "
                       "because the string under the key \"" +
                           string_key + "\" does not exist in the file.",
                       LCPP_DEFAULT_ERROR);
    }

    void CfgFile::change_the_value(const std::string &section_key,
                                   const std::string &string_key,
                                   const std::string &value) {
        int section_id = -1, string_id = -1;

        for (uint32_t i = 0; i < lines.size(); i++) {
            if (line_is_section(lines[i]) &&
                get_section_key(lines[i]) == section_key)
                section_id = i;
        }

        if (section_id == -1)
            LCPP_ERROR("It was not possible to change the value of the string "
                       "because the section under the key \"" +
                           section_key + "\" was not found in the file.",
                       LCPP_DEFAULT_ERROR);

        for (uint32_t i = section_id + 1; i < lines.size(); i++) {
            if (line_is_section(lines[i]))
                break;
            if (line_is_string(lines[i]) &&
                get_string_key(lines[i]) == string_key)
                string_id = i;
        }

        if (string_id == -1)
            LCPP_ERROR("It is impossible to change the value of the string, "
                       "because the string under the key \"" +
                           string_key +
                           "\" was not found in the section under the key \"" +
                           section_key + "\".",
                       LCPP_DEFAULT_ERROR);

        lines[string_id] = "    " + string_key + " = \"" + value + "\" " +
                           get_line_comment(lines[string_id]);
    }

    void CfgFile::create_string(const std::string &string_key,
                                const std::string &value) {
        size_t id = 0;

        for (size_t i = 0; i < lines.size(); i++) {
            if (line_is_section(lines[i]))
                break;

            id = i;
        }

        if (!lines.empty() && lines[id] != "")
            lines.insert(lines.begin() + id, "");

        if (id + 1 < lines.size() && line_is_section(lines[id + 1]))
            lines.insert(lines.begin() + id, "");

        lines.empty() ? lines.push_back(string_key + " = \"" + value + '"')
                      : (void)(lines[id] = string_key + " = \"" + value + '"');
    }
} // namespace LibCfgPP