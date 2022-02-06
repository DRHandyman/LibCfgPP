#include <algorithm>
#include <cctype>
#include <iostream>

#include "LibCfgPP.hpp"

using namespace std;

LibCfgPP::CfgFile cfg_file;

int main() {
    cfg_file.open("cfg_file.cfg");

    cfg_file.close();
}