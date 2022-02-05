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
    };
} // namespace LibCfgPP

#endif // _LIBCFGPP_HPP