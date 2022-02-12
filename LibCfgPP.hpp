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
        /**
         * @brief Main constructor
         */
        CfgFile();

        CfgFile(const std::string &path);

        /**
         * @brief Opens the configuration file at the specified path.
         * @param path - The path to a specific configuration file.
         */
        void open(const std::string &path);

        // Closes the file.
        void close();

        // Returns true if the file is open, false if closed.
        bool is_open();

        /**
         * @brief Returns the value of a specific string.
         * @param string_key - String key.
         */
        std::string read(const std::string &string_key),
            /**
             * @brief Returns the value of a specific string from a specific
             * section.
             * @param section_key - Section Key.
             * @param string_key - String key.
             */
            read(const std::string &section_key, const std::string &string_key);

        /**
         * @details Let's say you have made changes in the file, for example,
         *          changed the value of a string. In order for the changes to
         * take effect, you will need to call this function.
         */
        void update_file();

        /**
         * @brief Changes the value of a specific string to the one you
         * specified.
         * @param string_key - String key.
         * @param value - The value to assign to the string.
         */
        void change_the_value(const std::string &string_key,
                              const std::string &value),
            /**
             * @brief Changes the value of a specific string in a specific
             * section.
             * @param section_key - Section Key.
             * @param string_key - String key.
             * @param value - The value to assign to the string.
             */
            change_the_value(const std::string &section_key,
                             const std::string &string_key,
                             const std::string &value);
    };
} // namespace LibCfgPP

#endif // _LIBCFGPP_HPP