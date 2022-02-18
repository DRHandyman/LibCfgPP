#include <iostream>

#include "../../LibCfgPP.hpp"

using namespace std;

LibCfgPP::CfgFile cfg_file;

int main() {
    try {
        cfg_file.open("cfg_file.cfg");

        cfg_file.change("string", "another value");

        cfg_file.change("section", "string", "another value");

        cfg_file.update();
    } catch (const LibCfgPP::LCPPException &e) {
        cout << e.what() << endl;
    }

    cfg_file.close();
}