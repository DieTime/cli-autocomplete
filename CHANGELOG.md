# Changelog

All notable changes to this project will be documented in this file.

## [2.0.1] - 2020-12-30 [[xxxxxxx]](https://github.com/DieTime/CLI-Autocomplete/commit/xxxxxxx)

### Fixed

- MacOS preprocessor directive. `version 2.0.0 did not compile on macOS`


## [2.0.0] - 2020-12-29 [[62f1244]](https://github.com/DieTime/CLI-Autocomplete/commit/62f1244)

### Fixed

- Bug with incorrect autocompletion. `The rules are now stored in a tree`.
  Until then, the program relied only on the last two words.
- No memory leak `worked with fsanitize`

### Changed

- Language changed from C++17 to C99
- Config file stored like a tree after parsing

### Added

- Increased runtime error handling
- Increased performance


## [1.0.3] - 2020-08-16 [[3f28a14]](https://github.com/DieTime/CLI-Autocomplete/commit/3f28a14)

### Fixed 

- Unannotated fallthrough between switch labels


## [1.0.2] - 2020-08-16 [[a395e76]](https://github.com/DieTime/CLI-Autocomplete/commit/a395e76)

### Added

- `single header` version of the library
- `static` and `dynamic` versions of the library


## [1.0.1] - 2020-08-07 [[b27ed19]](https://github.com/DieTime/CLI-Autocomplete/commit/b27ed19)

### Changed

- Behavior of the tab key if there are no predictions.

### Fixed

- Detecting arrow key presses on some Windows terminals.


## [1.0.0] - 2020-08-04 [[a51d09f]](https://github.com/DieTime/CLI-Autocomplete/commit/a51d09f)

### Added

- The first working version of the library.
- README file with base description.
- MIT License file.

### Changed 

- CMake file for identical build on different platforms.

### Fixed

- Error displaying colors on UNIX systems.