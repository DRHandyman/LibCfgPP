#include <iostream>

#include "../../LibCfgPP.hpp"

using namespace std;

LibCfgPP::CfgFile cfg_file;

int main() {
    try {
        cfg_file.open("cfg_file.cfg");

        cout << cfg_file.read("string") << endl;
    } catch (const LibCfgPP::exception &ex) {
        cout << ex.what() << endl;
    }

    cfg_file.close();
}