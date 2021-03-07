<p align="center">
  <img src="https://i.ibb.co/41pL50L/Group-1.png" width="400" alt="logo">
</p>
<p align="center">
  <img src="https://img.shields.io/badge/version-2.0.1-b.svg" alt="version">
</p>

### About
Cross-platform flexible autocomplete library for your CLI applications
  
<img src="https://s7.gifyu.com/images/example3316d0de48f02022.gif" alt="example gif" width="500">

### Features
- Cross-platform: `MacOS`, `Linux`, `Windows`
- Write your own autocomplete rules
- Setup your own highlight colors
- `single header` version of the library
- `static` and `dynamic` versions of the library

### Config Example
1. After `git` may follow: `config`, `init`, `clone`
2. After `config` may follow: `--global`, `user.name`, `user.email`
3. After `--global` may follow: `user.name`, `user.email`
4. After `user.name` may follow optional value: `"[name]"`
5. `...`
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
        [repository_name]
    clone
        [url]
```

### Simple Example
> More complex example with: `color settings`, `handling optional values` and `line title configuration` [you will find here](examples/custom_input.c)
```cpp
#include <stdio.h>

#include "../include/autocomplete.h"

int main() {
    // Parsing the configuration file
    Tree* rules = tree_create("../example.config");

    fprintf(
        stderr,
        "Attention! Please run the executable file only\n"
        "           through the command line!\n\n"

        "- To switch the prompts press UP or DOWN arrow.\n"
        "- To move cursor press LEFT or RIGHT arrow.\n"
        "- To edit input press DELETE or BACKSPACE key.\n"
        "- To apply current prompt press TAB key.\n\n"
    );

    // Listening process
    while (1) {
        // Get user input
        char* str = input(rules);
        printf("\n%s\n", str);

        // Stop listening if user need
        if (strcmp(str, "") == 0) {
            free(str);
            break;
        }

        // Free user input string
        free(str);
    }

    // Free rules
    tree_free(rules);

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

# Unix
cd builds/examples/unix/Release
./custom_example

# MinGW
cd builds/examples/mingw/Release
custom_example.exe

# MSVC
cd builds/examples/msvc/Release
custom_example.exe
```

### Linking a shared library [[Releases]](https://github.com/DieTime/CLI-Autocomplete/releases/tag/v2.0.0-shared)

##### Unix
```bash
gcc -o <executable> <paths/of/source/files> -L<path/to/shared/lib/folder> -I<path/to/include/folder> -lcliac -Wl,-rpath,<path/to/shared/lib/folder>
```

##### Windows MSVC from VS Command Prompt
```cmd
cl /EHsc <paths/of/source/files> /Fe<executable>.exe /I<path/to/include/folder> /link <path/to/shared/lib/file(.lib)>
```

##### Windows MinGW
```cmd
gcc -o <executable>.exe <paths/of/source/files> -L<path/to/shared/lib/folder> -I<path/to/include/folder> -lcliac
```

### Linking a static library [[Releases]](https://github.com/DieTime/CLI-Autocomplete/releases/tag/v2.0.0-static)

##### Unix
```bash
gcc -o <executable> <paths/of/source/files> -L<path/to/static/lib/folder> -I<path/to/include/folder> -lcliac
```

##### Windows MSVC from VS Command Prompt
```cmd
cl /EHsc <paths/of/source/files> /Fe<executable>.exe /I<path/to/include/folder> /link <path/to/static/lib/file(.lib)>
```

##### Windows MinGW
```cmd
gcc -o <executable>.exe <paths/of/source/files> -L<path/to/static/lib/folder> -I<path/to/include/folder> -lcliac
```

### About changes
> A detailed description of the changes can be found in [CHANGELOG.md](CHANGELOG.md)

### Support the author
You can support me so that there will be more good open source projects in the future
<p align="center" style="padding: 10px 0 20px 0">
  <a href="https://www.buymeacoffee.com/roniemartinez" target="_blank">
    <img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="50" width="200">
  </a>
</p>
