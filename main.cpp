#include <iostream>

#include "LibCfgPP.hpp"

using namespace std;

LibCfgPP::CfgFile cfg_file;

int main(){
    try{    
        cfg_file.open("cfg_file.cfg");

        for(uint32_t i=0;i<100000;i++)
            cout << cfg_file.read("string") << endl;

        cfg_file.close();
    }catch(const LibCfgPP::exception &ex){
        cout << ex.what() << endl;
    }
}