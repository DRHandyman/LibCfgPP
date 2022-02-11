#ifndef _LIBCFGPP_HPP
#define _LIBCFGPP_HPP

#include <string>

namespace LibCfgPP {
    class exception : std::exception {
        std::string _msg;

      public:
        exception(const std::string &msg) : _msg(msg) {}
        
        virtual const char *what() const noexcept override {
            return _msg.c_str();
        }
    };

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

        void change_the_value(const std::string &string_key,
                              const std::string &value),
            change_the_value(const std::string &section,
                             const std::string &string_key,
                             const std::string &value);
    };
} // namespace LibCfgPP

#endif // _LIBCFGPP_HPP