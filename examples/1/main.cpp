#include <iostream>

#include "../../LibCfgPP.hpp"

using namespace std;

LibCfgPP::CfgFile cfg_file;

string section_key = "section", string_key = "string";

int main() {
    try {
        cfg_file.open("cfg_file.cfg");

        cout << string_key + " = \"" + cfg_file.read("string") + "\"\n" << endl;
        cout << '[' + section_key + ']' << endl
             << "   " + string_key + " = \"" +
                    cfg_file.read("section", "string") + '"'
             << endl;

        cfg_file.close();
    } catch (const LibCfgPP::exception &ex) {
        cout << ex.what() << endl;
    }
}