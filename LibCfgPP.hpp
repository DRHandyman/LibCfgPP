#ifndef _LIBCFGPP_HPP
#define _LIBCFGPP_HPP

#include <string>

typedef int32_t LCPP_INT32;
typedef int64_t LCPP_INT64;

typedef uint32_t LCPP_UINT32;
typedef uint64_t LCPP_UINT64;

typedef bool LCPP_BOOL;

namespace LibCfgPP {
    class CfgFile {
      public:
        CfgFile();

        CfgFile(const std::string &path);

        void open(const std::string &path);

        void close();

        LCPP_BOOL is_open();

        void update_file();
    };
} // namespace LibCfgPP

#endif // _LIBCFGPP_HPP