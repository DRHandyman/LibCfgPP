# LibCfg++
LibCfg++ is a convenient C++ library for working with configuration files.
# What can it do?
* Manipulate strings (Read and change string values).
* Create and read sections.
* Format the file in its own style.
* Scan the file for errors.
# Dependencies
### Linux:
* [G++](https://gcc.gnu.org/projects/cxx-status.html#cxx11) (GCC >= 4.8.1)
### Windows:
* [MinGW](https://sourceforge.net/projects/mingw/) or [Visual C++](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)
# Code example
```cpp
// main.cpp

#include <iostream>

#include "LibCfgPP.hpp"

using namespace std;

LibCfgPP::CfgFile cfg_file;

int main() {
    try {
        cfg_file.open("cfg_file.cfg");

        cout << cfg_file.read("string") << endl
             << cfg_file.read("section", "string") << endl;
    } catch (const LibCfgPP::exception &ex) {
        std::cerr << ex.what() << '\n';
    }

    cfg_file.close();

    return EXIT_SUCCESS;
}
```
```
# cfg_file.cfg

string = "value" # Test comment
 
[section]
    string = "value" # Test comment
```
# Notes
* You can view code examples in the [examples](https://github.com/DRHandyman/LibCfgPP/tree/main/examples) folder.
* The project is in beta testing, but at the moment it can already read string values.
