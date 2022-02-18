#ifndef _LIBCFGPP_HPP
#define _LIBCFGPP_HPP

#include <string>
#include <vector>

namespace LibCfgPP {
    class LCPPException : std::exception {
        std::string _msg;

      public:
        LCPPException(const std::string &msg) : _msg(msg){};

        virtual const char *what() const noexcept override {
            return _msg.c_str();
        }
    };

    class CfgFile {
      public:
        /// Default constructor.
        CfgFile() = default;

        /**
         * @details Creates a new CfgFile object and opens the configuration
         *          file at the specified path.
         *
         * @param path The path to the configuration file.
         */
        CfgFile(const std::string &path);

        /**
         * @brief Opens the configuration file at the specified path.
         *
         * @param path The path to the configuration file.
         */
        void open(const std::string &path);

        /// Closes the file.
        void close();

        /**
         * @brief Returns true if the file is open, false if closed.
         *
         * @return true
         * @return false
         */
        bool is_open();

        /**
         * @details Finds a string by the specified key in the file and reads
         *          its value.
         *
         * @param string_key Key to the string.
         *
         * @return The value of the found string.
         */
        std::string read(const std::string &string_key),
            /**
             * @brief Reads the value of a string in a specific section.
             *
             * @param section_key Key to the section.
             * @param string_key Key to the string.
             *
             * @return The value of the found string.
             */
            read(const std::string &section_key, const std::string &string_key);

        /// Updates all lines of the file.
        void update();

        /**
         * @details Finds a string under the specified key and changes its value
         *          to the one you specified.
         *
         * @param string_key The key to the string.
         * @param value New value for the string.
         */
        void change(const std::string &string_key, const std::string &value),
            /**
             * @details Finds a string by the specified key, in a specific
             *          section and changes its value to the one you specified.
             *
             * @param section_key The key to the section.
             * @param string_key The key to the string.
             * @param value New value for the string.
             */
            change(const std::string &section_key,
                   const std::string &string_key, const std::string &value);

        /**
         * @brief Creates a new string in the file.
         *
         * @param string_key The key of the string.
         * @param value The value that should be assigned to the string.
         */
        void create_string(const std::string &string_key,
                           const std::string &value);

      private:
        std::string path;
        std::vector<std::string> lines;
        bool _is_open = false;
    };
} // namespace LibCfgPP

#endif // _LIBCFGPP_HPP