#ifndef _LIBCFGPP_HPP
#define _LIBCFGPP_HPP

#include <string>

typedef int32_t LCPP_INT32;
typedef int64_t LCPP_INT64;

typedef uint32_t LCPP_UINT32;
typedef uint64_t LCPP_UINT64;

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