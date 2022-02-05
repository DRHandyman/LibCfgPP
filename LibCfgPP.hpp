#ifndef _LIBCFGPP_HPP
#define _LIBCFGPP_HPP

namespace LibCfgPP {
    const char **fileTypes = [ ".cfg", ".conf", ".config" ];

    class CfgFile {
        public:
            CfgFile();

            void open(const char *path);

            void close();
    }
} // namespace LibCfgPP

#endif // _LIBCFGPP_HPP