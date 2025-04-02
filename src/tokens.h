#pragma once
#include "init.h"
#include <vector>
#include <stack>
#include <string>
#include <variant>

// 错误类型枚举
enum class ErrorType
{
    InvalidCharacter,  // 非法字符
    UnmatchedBracket,  // 括号不匹配
    InvalidNumber,     // 非法数字
    InvalidIdentifier, // 非法标识符
    InvalidString,     // 非法字符串
    UnexpectedEOF,     // 意外的文件结束
    InvalidOperator,   // 非法运算符
    InvalidDelimiter   // 非法分隔符
};

// 错误结构体
struct Error
{
    ErrorType type;
    size_t line;
    size_t column;
    string message;
};

// Token结构体
struct Token
{
    RustTokenType type;
    string lexeme;
    size_t line;
    size_t column;
};

// Tokenizer类
class Tokenizer
{
public:
    Tokenizer() = default;
    ~Tokenizer() = default;

    // 解析输入文本
    vector<Token> tokenize(const string &input);

    // 获取错误信息
    const vector<Error> &getErrors() const { return errors; }

    // 获取格式化输出
    string getFormattedOutput() const;

    // 获取token类型字符串
    string getTokenTypeString(const Token &token) const;

    // 获取token值字符串
    string getTokenValueString(const Token &token) const;

private:
    vector<Error> errors;
    vector<Token> tokens;
    size_t currentLine = 1;
    size_t currentColumn = 1;

    // 辅助函数
    Token parseIdentifier(const string &input, size_t &pos);
    Token parseNumber(const string &input, size_t &pos);
    Token parseString(const string &input, size_t &pos);
    Token parseComment(const string &input, size_t &pos);
    Token parseOperator(const string &input, size_t &pos);
    Token parseDelimiter(const string &input, size_t &pos);

    // 错误处理
    void addError(ErrorType type, size_t line, size_t column, const string &message);

    // 括号匹配检查
    void checkBrackets();
    bool isOpeningBracket(char c) const;
    bool isClosingBracket(char c) const;
    char getMatchingBracket(char c) const;
};