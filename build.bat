@echo off

if "%1" == "clean" goto %1
if "%1" == "calc"  goto %1
if "%1" == "pkg"   goto %1

echo.
echo   Targets:
echo      clean  Clean generated files
echo      calc   Build calc.exe
echo      pkg    Build FreeDOS package
echo.
goto end


:clean
    rem Clean
    if exist calc.exe del calc.exe > NUL
    if exist calc.zip del calc.zip > NUL
    if exist gen\NUL deltree /y gen > NUL
    if exist pkg\NUL deltree /y pkg > NUL
    if exist te-src\NUL deltree /y te-src > NUL
    goto end


:calc
    rem Build executable
    if exist te-src\NUL deltree /y te-src > NUL
    unzip tinyexpr.zip
    del te-src\LICENSE
    pat tinyexpr.pat te-src
    cd te-src
    wcl calc.c tinyexpr.c /fe=..\calc.exe
    cd ..
    goto end


:pkg
    rem Build FreeDOS package

    rem It would be more correct to set the GRP variable to 'progs', but doing
    rem so causes calc to be installed in the root directory. Setting GRP to
    rem 'devel' installs calc in '\devel\calc'
    rem 
    set PKG=pkg
    set GRP=devel
    set NAM=calc
    if exist %PKG%\NUL deltree /y %PKG% > NUL
    if exist %NAM%.zip del %NAM%.zip  > NUL

    mkdir %PKG%
    mkdir %PKG%\appinfo
    set LSM=%PKG%\appinfo\%NAM%.lsm
    echo Begin3> %LSM%
    echo Title:          %NAM%>> %LSM%
    echo Version:        0.1>> %LSM%
    echo Entered-date:   2020-07-10>> %LSM%
    echo Description:    A command-line calculator.>> %LSM%
    echo Keywords:       freedos %NAM%>> %LSM%
    echo Author:         >> %LSM%
    echo Maintained-by:  >> %LSM%
    echo Primary-site:   >> %LSM%
    echo Alternate-site: >> %LSM%
    echo Original-site:  >> %LSM%
    echo Platforms:      DOS>> %LSM%
    echo Copying-policy: GPL>> %LSM%
    echo End>> %LSM%

    mkdir %PKG%\%GRP%
    mkdir %PKG%\source
    mkdir %PKG%\source\%NAM%
    if not exist %PKG%\source\%NAM%\NUL goto err1pkg

    copy build.bat    %PKG%\source\%NAM%
    copy calc.txt     %PKG%\source\%NAM%
    copy LICENSE.txt  %PKG%\source\%NAM%
    copy tinyexpr.pat %PKG%\source\%NAM%
    copy tinyexpr.zip %PKG%\source\%NAM%
    if not exist %PKG%\source\%NAM%\tinyexpr.zip goto err2pkg

    rem copy te-src.zip   %PKG%\source\%NAM%

    mkdir %PKG%\%GRP%\%NAM%
    copy .\%NAM%.exe %PKG%\%GRP%\%NAM%
    if not exist %PKG%\%GRP%\%NAM%\%NAM%.exe goto err3pkg
    copy .\calc.txt %PKG%\%GRP%\%NAM%
    if not exist %PKG%\%GRP%\%NAM%\calc.txt goto err3pkg

    mkdir %PKG%\links
    echo %GRP%\%NAM%\%NAM%.exe > %PKG%\links\%NAM%.bat
    if not exist %PKG%\links\calc.bat goto err4pkg

    cd .\%PKG%
    rem  -9  Max compression
    rem  -r  Recurse into directories
    zip -9 -r ..\calc.zip *
    if not exist ..\calc.zip goto err5pkg
    cd ..

    echo.
    echo The calc.zip package has been created.
    echo.
    echo To install: fdnpkg install calc.zip
    echo         or: fdnpkg install-wsrc calc.zip
    echo  To remove: fdnpkg remove calc

    goto end


:err1pkg
    echo Error creating directory structure. Building package failed.
    goto end

:err2pkg
    echo Error copying source files. Building package failed. 
    goto end

:err3pkg
    echo Error copying executable files. Building package failed.
    goto end

:err4pkg
    echo Error building links. Building package failed. 
    goto end

:err5pkg
    echo Error creating zip file. Building package failed.
    goto end

:end
