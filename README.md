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
# Usage
* Clone this repository to any folder you like, using this command ```git clone https://github.com/DRHandyman/LibCfgPP```. If you do not have GIT installed, then here is a [link](https://github.com/git-guides/install-git) to the documentation for installing it.
* Drag and drop files [LibCfgPP.cpp](https://github.com/DRHandyman/LibCfgPP/blob/main/LibCfgPP.cpp) and [LibCfgPP.hpp](https://github.com/DRHandyman/LibCfgPP/blob/main/LibCfgPP.hpp) to the folder with your project.
* Compile the source file LibCfgPP.cpp along with the rest of the source files.
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