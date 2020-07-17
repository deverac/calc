`calc.zip` is a FreeDOS package containing `calc.exe` (and sources).

`calc.exe` - A simple command-line calculator.

`calc.exe` was created to run on (16-bit) FreeDOS, but can also be built to run on (64-bit) Linux. On Linux, a much more capable `calc` program already exists.

`calc.exe` uses [TinyExpr](https://github.com/codeplea/tinyexpras) as its calculation engine. `tinyexpr.zip` contains a subset of the TinyExpr source files that are needed to build `calc.exe`. The source code for tinyexpr has a separate license.

The build script requires the [difpat](https://github.com/deverac/difpat) package to be installed.

On FreeDOS:

    >build.bat clean   Remove generated files
    >build.bat calc    Build calc.exe
    >build.bat pkg     Build FreeDOS package calc.zip


On Linux:

    ./build.sh clean     Remove generated files
    ./build.sh hexdump   Build the hexd executable
    ./build.sh zip       Store files for FreeDOS in c.zip