#ifndef _LIBCFGPP_HPP
#define _LIBCFGPP_HPP

#include <string>

namespace LibCfgPP {
    class CfgFile {
      public:
        CfgFile();

        CfgFile(const std::string &path);

        void open(const std::string &path);

        void close();

        bool is_open();

        std::string read(const std::string &string_key),
            read(const std::string &section_key, const std::string &string_key);

        void update_file();
    };
} // namespace LibCfgPP

#endif // _LIBCFGPP_HPP