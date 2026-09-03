@echo off
setlocal enabledelayedexpansion

:: Usage: makeWinDeps.bat
:: Builds VTK 9.6.2 against Qt6 and installs into DEPS\

set "QT=C:\Qt6\6.10.2\msvc2022_64"
set "PREFIX=%CD%\DEPS"

:: ---- Clone VTK if not present ----

if not exist VTK-9.6.2 (
    echo.
    echo --- Cloning VTK 9.6.2 repository ---
    echo.
    git clone --branch v9.6.2 --depth 1 https://github.com/Kitware/VTK.git VTK-9.6.2
    if errorlevel 1 (
        echo.
        echo FAIL: git clone failed.
        echo.
        exit /b 1
    )
)

:: ---- Create install prefix ----

if not exist "%PREFIX%" mkdir "%PREFIX%"

:: ---- Normalize paths to forward slashes for CMake ----

set "QT_FWD=%QT:\=/%"
set "PREFIX_FWD=%PREFIX:\=/%"

:: ---- Configure ----

echo.
echo Configuring VTK...
echo.

cmake -B VTK-9.6.2\build -S VTK-9.6.2 ^
    -DCMAKE_INSTALL_PREFIX="%PREFIX_FWD%" ^
    -DCMAKE_PREFIX_PATH="%QT_FWD%" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_SHARED_LIBS=ON ^
    -DVTK_GROUP_ENABLE_StandAlone=WANT ^
    -DVTK_GROUP_ENABLE_Rendering=WANT ^
    -DVTK_GROUP_ENABLE_Qt=WANT ^
    -DVTK_QT_VERSION=6 ^
    -DVTK_GROUP_ENABLE_MPI=DONT_WANT

if errorlevel 1 (
    echo.
    echo FAIL: CMake configure failed.
    echo.
    exit /b 1
)

:: ---- Build ----

echo.
echo Building VTK...
echo.

cmake --build VTK-9.6.2\build --config Release
if errorlevel 1 (
    echo.
    echo FAIL: CMake build failed.
    echo.
    exit /b 1
)

:: ---- Install ----

echo.
echo Installing VTK to %PREFIX%...
echo.

cmake --install VTK-9.6.2\build --config Release
if errorlevel 1 (
    echo.
    echo FAIL: CMake install failed.
    echo.
    exit /b 1
)

echo.
echo Build complete!
echo.
