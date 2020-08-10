<p align="center">
  <img src="https://i.ibb.co/41pL50L/Group-1.png" width="400" alt="logo">
</p>
<p align="center">
  <img src="https://img.shields.io/badge/version-1.0.0-b.svg" alt="version">
</p>

### About
Cross-platform flexible autocomplete library for your CLI applications.
  
<img src="https://s7.gifyu.com/images/example3316d0de48f02022.gif" alt="example gif" width="500">

### Features
- Cross-platform: `macOS`, `Linux`, `Windows`.
- Write your own autocomplete rules.
- Setup your own highlight colors.
- Required `C++17`.

### Config Example
1. After `git` may follow: `config`, `init`, `clone`.
2. After `config` may follow: `--global`, `user.name`, `user.email`.
3. After `--global` may follow: `user.name`, `user.email`.
4. After `user.name` may follow optional value: `"[name]"`.
5. ...
```
git
    config
        --global
            user.name
                "[name]"
            user.email
                "[email]"
        user.name
            "[name]"
        user.email
            "[email]"
    init
        [repository name]
    clone
        [url]
```

### Simple Example
> More complex example with: `color settings`, `handling optional values` and `line title configuration` [you will find here.](example/main.cpp)
```cpp
#include <iostream>
#include <string>

#include "../include/autocomplete.h"

int main() {
    std::string config_file_path = "../config.txt";
  
    auto [dict, status, message] = parse_config_file(config_file_path);

    if (status) {
        while (true) {
            std::cerr << "Attention! Please run the executable file only" << std::endl
                      << "through the command line!\n\n";
            
            std::cerr << "- To switch the prompts press UP or DOWN arrow." << std::endl;
            std::cerr << "- To move cursor press LEFT or RIGHT arrow." << std::endl;
            std::cerr << "- To edit input press DELETE or BACKSPACE key." << std::endl;
            std::cerr << "- To apply current prompt press TAB key.\n\n";

            std::string command = input(dict);
            std::cout << std::endl << command << std::endl << std::endl;
        }
    }
    else {
        std::cerr << message << std::endl;
    }

    return 0;
}
```

### How to start
```bash
git clone https://github.com/DieTime/CLI-AutoComplete.git
cd CLI-AutoComplete/

cmake -DCMAKE_BUILD_TYPE=Release -S . -B ./cmake-build 
cmake --build ./cmake-build --config Release

----------------------- RUN EXAMPLE ---------------------

cd example/Release/
./example            # Posix
example.exe          # Windows

---------------------- OR RUN TESTS ---------------------

cd tests/Release/
./tests              # Posix
tests.exe            # Windows
```

### About changes
> A detailed description of the changes can be found in [CHANGELOG.md](CHANGELOG.md)

### Linking a dynamic library [[Releases]](https://github.com/DieTime/CLI-Autocomplete/releases/tag/v1.0.0-alpha)
#### Posix
```bash
g++ -std=c++17 -o <executable> <paths/of/source/files> -L<path/to/shared/lib> -I<path/to/include> -lcliac -Wl,-rpath,<path/to/shared/lib>
```

#### Windows MSVC from VS Command Prompt
```cmd
cl /EHsc /std:c++17 <paths/of/source/files> /Fe<executable>.exe /I <path/to/include> /link <path/to/shared/lib>
```

#### Windows MinGW
```cmd
g++ -std=c++17 -o <executable>.exe <paths/of/source/files> -L<path/to/shared/lib> -I<path/to/include> -lcliac
```