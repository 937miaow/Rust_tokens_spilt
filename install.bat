@echo off
echo 正在安装Rust词法分析器所需环境...

REM 检查Python是否已安装
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python未安装，请先安装Python 3.6或更高版本
    echo 下载地址：https://www.python.org/downloads/
    pause
    exit /b 1
)

REM 检查CMake是否已安装
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo CMake未安装，请先安装CMake 3.15或更高版本
    echo 下载地址：https://cmake.org/download/
    pause
    exit /b 1
)

REM 检查Visual Studio是否已安装
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo Visual Studio未安装，请先安装Visual Studio 2019或更高版本
    echo 下载地址：https://visualstudio.microsoft.com/
    pause
    exit /b 1
)

REM 安装PyQt5
echo 正在安装PyQt5...
pip install PyQt5==5.15.9
if %errorlevel% neq 0 (
    echo PyQt5安装失败
    pause
    exit /b 1
)

REM 安装pyinstaller
echo 正在安装pyinstaller...
pip install pyinstaller
if %errorlevel% neq 0 (
    echo pyinstaller安装失败
    pause
    exit /b 1
)

echo 环境安装完成！
pause 