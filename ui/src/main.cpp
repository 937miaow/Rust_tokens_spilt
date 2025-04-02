#include <iostream>
#include <fstream>
#include <string>
#include "../../src/tokens.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // 读取输入文件
    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // 创建词法分析器实例
    Tokenizer tokenizer;

    // 执行词法分析
    auto tokens = tokenizer.tokenize(code);

    // 获取格式化输出
    std::string formatted_output = tokenizer.getFormattedOutput();

    // 输出结果
    std::cout << formatted_output;

    return 0;
}