# Rust词法分析器

这是一个基于C++和Python的Rust词法分析器，可以将Rust代码分解成各种词法单元。
只是一个课程作业。

## 项目结构

```
Rust_tokens_spilt/
├── src/                # C++后端代码
│   ├── tokens.cpp      # 词法分析器实现
│   ├── tokens.h        # 词法分析器头文件
│   ├── init.h          # 初始化相关头文件
│   └── CMakeLists.txt  # C++构建配置
├── ui/                 # Python前端代码
│   ├── src/
│   │   ├── main.py    # PyQt5界面实现
│   │   └── main.cpp   # C++可执行文件入口
│   └── CMakeLists.txt # Python构建配置
├── test/              # Rust测试代码
│   ├── simple.rs      # 简单Rust代码示例
│   ├── struct.rs      # 结构体示例
│   └── trait.rs       # trait示例
├── install.bat        # 一键安装脚本
└── CMakeLists.txt     # 根目录构建配置
```

## 构建步骤

### 环境要求

- CMake 3.15 或更高版本
- Python 3.6 或更高版本
- Visual Studio 2019 或更高版本（Windows）
- PyQt5 5.15.9

### 环境安装

有两种方式安装环境：

1. 使用一键安装脚本（推荐）：
```bash
./install.bat
```

2. 手动安装：
   - 安装 Python：
     - 访问 [Python官网](https://www.python.org/downloads/) 下载并安装 Python 3.6 或更高版本
     - 安装时勾选 "Add Python to PATH" 选项

   - 安装 CMake：
     - 访问 [CMake官网](https://cmake.org/download/) 下载并安装 CMake
     - 安装时选择 "Add CMake to the system PATH for all users"

   - 安装 Visual Studio：
     - 访问 [Visual Studio官网](https://visualstudio.microsoft.com/) 下载并安装 Visual Studio 2019 或更高版本
     - 安装时选择 "Desktop development with C++" 工作负载

   - 安装 PyQt5：
     ```bash
     pip install PyQt5==5.15.9
     ```

   - 安装其他依赖：
     ```bash
     pip install pyinstaller
     ```

### 构建过程

1. 克隆项目：
```bash
git clone https://github.com/937miaow/Rust_tokens_spilt.git
cd Rust_tokens_spilt
```

2. 创建构建目录：
```bash
mkdir build
cd build
```

3. 配置项目：
```bash
cmake ..
```

4. 构建项目：
```bash
cmake --build . --config Release
```

5. 运行程序：
```bash
cd bin
./run.bat
```

### 打包成可执行文件

1. 进入bin目录：
```bash
cd build/bin
```

2. 运行打包脚本：
```bash
./build_exe.bat
```

3. 打包后的程序位于 `dist` 目录下

4. 复制C++后端程序：
```bash
copy Rust_tokens_spilt.exe dist\
```

5. 现在 `dist` 目录下的 `main.exe` 和 `Rust_tokens_spilt.exe` 可以复制到任何机器上运行，无需安装环境。

## 使用说明

1. 在输入框中输入Rust代码，或点击"选择文件"按钮选择Rust源文件
2. 点击"分析"按钮进行词法分析
3. 分析结果将显示在下方的输出框中

## 测试代码

项目包含三个测试文件：
- `simple.rs`：包含基本的变量、循环和向量操作
- `struct.rs`：包含结构体和方法的定义
- `trait.rs`：包含trait和实现

## 作者

- 作者：hhh937meow
- GitHub：[937miaow](https://github.com/937miaow) 