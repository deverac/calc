#!/bin/sh

# On Linux, the executable is named 'calc'. When building c.zip, we wish to
# use a directory named 'calc'. The 'calc' executable and the 'calc' directory
# cannot exist at the same time. When building c.zip, the 'calc' executable
# will be deleted, if it exists. 

if [ "$1" = "calc" ]; then

    rm -rf calc
    cd src
    gcc calc.c tinyexpr.c -lm -o ../calc
    cd ..

elif [ "$1" = "clean" ]; then

    rm -f c.zip
    rm -f calc


elif [ "$1" = "zip" ]; then

    if [ -e calc ]; then
        printf "\nDeleting calc\n"
        rm -rf calc
    fi

    dirnam=calc
    mkdir -p $dirnam
    for fil in LICENSE.txt build.bat src ./support/calc.txt ./support/tinyexpr.zip; do
        cp -r $fil $dirnam
    done

    cd $dirnam
    unzip -q tinyexpr.zip
    rm te-src/LICENSE
    diff --new-file --recursive te-src src > tinyexpr.pat
    cd ..
    rm -rf $dirnam/te-src
    rm -rf $dirnam/src

    rm -rf c.zip
    7z a c.zip $dirnam
    rm -rf $dirnam

else

    printf "\n"
    printf "  Targets:\n"
    printf "      clean    Remove generated files\n"
    printf "      calc     Build calc executable\n"
    printf "      zip      Store files for FreeDOS in c.zip\n"
    printf "\n"

fi
