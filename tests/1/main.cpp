// A test for the time it takes to read the value of a string a million times.

#include <ctime>
#include <iostream>

#include "../../LibCfgPP.hpp"

using namespace std;

LibCfgPP::CfgFile cfg_file;

int main() {
    clock_t start, end;

    start = clock();

    try {
        cfg_file.open("cfg_file.cfg");

        for (int i = 0; i < 1000000; i++)
            cfg_file.read("string");

        cfg_file.close();
    } catch (const LibCfgPP::exception &ex) {
        cout << ex.what() << endl;
    }

    end = clock();

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    cout << time_taken << endl;

    return EXIT_SUCCESS;
}