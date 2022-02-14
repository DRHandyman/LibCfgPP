#include <iostream>

#include "../../LibCfgPP.hpp"

using namespace std;

LibCfgPP::CfgFile cfg_file;

int main() {
    try {
        cfg_file.open("cfg_file.cfg");

        cfg_file.change_the_value("string", "another value");

        cfg_file.change_the_value("section", "string", "another value");

        cfg_file.update_file();
    } catch (const LibCfgPP::exception &e) {
        cout << e.what() << endl;
    }

    cfg_file.close();
}