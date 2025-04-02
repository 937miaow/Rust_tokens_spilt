#include "tokens.h"
#include <vector>
#include <cctype>
#include <unordered_set>
#include <sstream>
#include <variant>

vector<Token> Tokenizer::tokenize(const string &input)
{
    tokens.clear();
    errors.clear();
    currentLine = 1;
    currentColumn = 1;

    size_t pos = 0;
    while (pos < input.length())
    {
        char c = input[pos];

        // 跳过空白字符
        if (whitespaces.find(static_cast<char32_t>(c)) != whitespaces.end())
        {
            if (c == '\n')
            {
                currentLine++;
                currentColumn = 1;
            }
            else
            {
                currentColumn++;
            }
            pos++;
            continue;
        }

        // 检查双字符分隔符
        if (pos + 1 < input.length())
        {
            string_view two_chars(input.data() + pos, 2);
            if (compound_delimiters.find(two_chars) != compound_delimiters.end())
            {
                Token token;
                token.type = RustTokenType(TokenKind::Delimiter);
                token.line = currentLine;
                token.column = currentColumn;
                token.lexeme = string(two_chars);
                tokens.push_back(token);
                pos += 2;
                currentColumn += 2;
                continue;
            }
        }

        // 根据首字符选择解析函数
        if (std::isalpha(c) || c == '_')
        {
            tokens.push_back(parseIdentifier(input, pos));
        }
        else if (std::isdigit(c))
        {
            tokens.push_back(parseNumber(input, pos));
        }
        else if (c == '"' || c == '\'')
        {
            tokens.push_back(parseString(input, pos));
        }
        else if (c == '/' && pos + 1 < input.length() && input[pos + 1] == '/')
        {
            tokens.push_back(parseComment(input, pos));
        }
        else if (single_char_delimiters.find(c) != single_char_delimiters.end())
        {
            tokens.push_back(parseDelimiter(input, pos));
        }
        else if (single_char_ops.find(c) != single_char_ops.end())
        {
            tokens.push_back(parseOperator(input, pos));
        }
        else
        {
            addError(ErrorType::InvalidCharacter, currentLine, currentColumn,
                     "Invalid character: " + string(1, c));
            pos++;
            currentColumn++;
        }
    }

    // 检查括号匹配
    checkBrackets();

    return tokens;
}

Token Tokenizer::parseIdentifier(const string &input, size_t &pos)
{
    size_t start = pos;
    size_t startLine = currentLine;
    size_t startColumn = currentColumn;

    while (pos < input.length() && (std::isalnum(input[pos]) || input[pos] == '_'))
    {
        pos++;
        currentColumn++;
    }

    string lexeme = input.substr(start, pos - start);
    RustTokenType type;

    // 检查是否是宏调用名（以!结尾）
    if (pos < input.length() && input[pos] == '!')
    {
        lexeme += '!';
        pos++;
        currentColumn++;
        type = TokenKind::MacroCall;
    }
    else if (strict_keywords.find(lexeme) != strict_keywords.end())
    {
        type = TokenKind::Keyword;
    }
    else if (reserved_keywords.find(lexeme) != reserved_keywords.end())
    {
        type = TokenKind::Keyword;
    }
    else if (weak_keywords.find(lexeme) != weak_keywords.end())
    {
        type = TokenKind::Keyword;
    }
    else
    {
        type = TokenKind::Identifier;
    }

    return Token{type, lexeme, startLine, startColumn};
}

Token Tokenizer::parseNumber(const string &input, size_t &pos)
{
    Token token;
    NumericLiteral num;
    num.type = NumericLiteral::Type::Integer;
    num.base = NumericLiteral::Base::Decimal;
    token.type = RustTokenType(TokenKind::Number);
    token.line = currentLine;
    token.column = currentColumn;

    // 处理十六进制
    if (pos + 1 < input.length() && input[pos] == '0' &&
        (input[pos + 1] == 'x' || input[pos + 1] == 'X'))
    {
        num.base = NumericLiteral::Base::Hexadecimal;
        token.lexeme += "0x";
        pos += 2;
        currentColumn += 2;

        while (pos < input.length())
        {
            char c = input[pos];
            if (isxdigit(c) || c == '_')
            {
                token.lexeme += c;
                pos++;
                currentColumn++;
            }
            else
            {
                break;
            }
        }
        return token;
    }

    // 处理十进制和浮点数
    bool hasDecimal = false;
    while (pos < input.length())
    {
        char c = input[pos];
        if (isdigit(c))
        {
            token.lexeme += c;
            pos++;
            currentColumn++;
        }
        else if (c == '.' && !hasDecimal)
        {
            num.type = NumericLiteral::Type::Float;
            token.lexeme += c;
            hasDecimal = true;
            pos++;
            currentColumn++;
        }
        else if (c == '_')
        {
            token.lexeme += c; // 保留下划线
            pos++;
            currentColumn++;
        }
        else
        {
            break;
        }
    }

    return token;
}

Token Tokenizer::parseString(const string &input, size_t &pos)
{
    Token token;
    StringLiteral str;
    str.kind = input[pos] == '\'' ? StringLiteral::Kind::Char : StringLiteral::Kind::Normal;
    token.type = RustTokenType(TokenKind::String);
    token.line = currentLine;
    token.column = currentColumn;

    char quote = input[pos];
    token.lexeme += quote;
    pos++;
    currentColumn++;

    while (pos < input.length())
    {
        char c = input[pos];
        if (c == quote)
        {
            token.lexeme += c;
            pos++;
            currentColumn++;
            break;
        }
        else if (c == '\\')
        {
            token.lexeme += c;
            pos++;
            currentColumn++;
            if (pos < input.length())
            {
                token.lexeme += input[pos++];
                currentColumn++;
            }
        }
        else
        {
            token.lexeme += c;
            pos++;
            currentColumn++;
        }
    }

    return token;
}

Token Tokenizer::parseComment(const string &input, size_t &pos)
{
    Token token;
    token.type = RustTokenType(TokenKind::Comment);
    token.line = currentLine;
    token.column = currentColumn;

    // 跳过 "//"
    token.lexeme += input[pos++];
    token.lexeme += input[pos++];
    currentColumn += 2;

    while (pos < input.length() && input[pos] != '\n')
    {
        token.lexeme += input[pos++];
        currentColumn++;
    }

    return token;
}

Token Tokenizer::parseOperator(const string &input, size_t &pos)
{
    Token token;
    token.type = RustTokenType(TokenKind::Operator);
    token.line = currentLine;
    token.column = currentColumn;

    char c = input[pos];
    token.lexeme += c;
    pos++;
    currentColumn++;

    // 处理双字符运算符
    if (pos < input.length())
    {
        string_view op(token.lexeme + input[pos]);
        if (compound_ops.find(op) != compound_ops.end())
        {
            token.lexeme += input[pos++];
            currentColumn++;
        }
    }

    return token;
}

Token Tokenizer::parseDelimiter(const string &input, size_t &pos)
{
    Token token;
    token.type = RustTokenType(TokenKind::Delimiter);
    token.line = currentLine;
    token.column = currentColumn;

    char c = input[pos];
    token.lexeme += c;
    pos++;
    currentColumn++;

    return token;
}

void Tokenizer::addError(ErrorType type, size_t line, size_t column, const string &message)
{
    Error error;
    error.type = type;
    error.line = line;
    error.column = column;
    error.message = message;
    errors.push_back(error);
}

// 括号匹配检查
void Tokenizer::checkBrackets()
{
    stack<pair<char, size_t>> bracketStack;

    for (const auto &token : tokens)
    {
        if (token.type.index() == 2 && token.lexeme.length() == 1)
        {
            char c = token.lexeme[0];
            if (isOpeningBracket(c))
            {
                bracketStack.push({c, token.line});
            }
            else if (isClosingBracket(c))
            {
                if (bracketStack.empty())
                {
                    addError(ErrorType::UnmatchedBracket, token.line, token.column,
                             "Unmatched closing bracket: " + string(1, c));
                }
                else
                {
                    char expected = getMatchingBracket(bracketStack.top().first);
                    if (c != expected)
                    {
                        addError(ErrorType::UnmatchedBracket, token.line, token.column,
                                 "Expected '" + string(1, expected) + "' but found '" + string(1, c) + "'");
                    }
                    bracketStack.pop();
                }
            }
        }
    }

    if (!bracketStack.empty())
    {
        addError(ErrorType::UnmatchedBracket, bracketStack.top().second, 1,
                 "Unmatched opening bracket: " + string(1, bracketStack.top().first));
    }
}

bool Tokenizer::isOpeningBracket(char c) const
{
    return c == '(' || c == '{' || c == '[';
}

bool Tokenizer::isClosingBracket(char c) const
{
    return c == ')' || c == '}' || c == ']';
}

char Tokenizer::getMatchingBracket(char c) const
{
    switch (c)
    {
    case '(':
        return ')';
    case '{':
        return '}';
    case '[':
        return ']';
    default:
        return '\0';
    }
}

// 输出格式化
string Tokenizer::getFormattedOutput() const
{
    stringstream ss;
    size_t currentLine = 0;
    string currentStatement;
    vector<Token> currentLineTokens;
    string originalLine; // 保存原始行内容

    for (const auto &token : tokens)
    {
        // 如果行号变化，处理上一行的结果
        if (token.line != currentLine)
        {
            if (currentLine != 0)
            {
                // 输出原语句
                ss << originalLine << "\n";

                // 输出解析结果（缩进4个空格）
                for (const auto &t : currentLineTokens)
                {
                    ss << "    " << t.lexeme << ": " << getTokenTypeString(t);
                    if (!getTokenValueString(t).empty())
                    {
                        ss << " (" << getTokenValueString(t) << ")";
                    }
                    ss << "\n";
                }
                ss << "\n";
            }
            currentLine = token.line;
            currentStatement.clear();
            currentLineTokens.clear();
            originalLine.clear();
        }

        // 收集当前语句和token
        currentStatement += token.lexeme;
        currentLineTokens.push_back(token);

        // 保存原始行内容（包括空格）
        if (token.column > originalLine.length() + 1)
        {
            originalLine += string(token.column - originalLine.length() - 1, ' ');
        }
        originalLine += token.lexeme;
    }

    // 处理最后一行
    if (!currentStatement.empty())
    {
        ss << originalLine << "\n";
        for (const auto &t : currentLineTokens)
        {
            ss << "    " << t.lexeme << ": " << getTokenTypeString(t);
            if (!getTokenValueString(t).empty())
            {
                ss << " (" << getTokenValueString(t) << ")";
            }
            ss << "\n";
        }
    }

    return ss.str();
}

string Tokenizer::getTokenTypeString(const Token &token) const
{
    if (token.type.index() == 1)
    { // TokenKind
        switch (get<TokenKind>(token.type))
        {
        case TokenKind::Identifier:
            return "标识符";
        case TokenKind::Keyword:
            return "关键字";
        case TokenKind::Number:
            return "数字";
        case TokenKind::String:
            return "字符串";
            return "字符串";
        case TokenKind::Comment:
            return "注释";
        case TokenKind::Operator:
            return "运算符";
        case TokenKind::Delimiter:
            return "分隔符";
        case TokenKind::MacroCall:
            return "宏调用名";
        default:
            return "未知类型";
        }
    }
    else if (token.type.index() == 0)
    { // Literal
        if (holds_alternative<NumericLiteral>(get<Literal>(token.type)))
        {
            const auto &num = get<NumericLiteral>(get<Literal>(token.type));
            string base;
            switch (num.base)
            {
            case NumericLiteral::Base::Binary:
                base = "二进制";
                break;
            case NumericLiteral::Base::Octal:
                base = "八进制";
                break;
            case NumericLiteral::Base::Decimal:
                base = "十进制";
                break;
            case NumericLiteral::Base::Hexadecimal:
                base = "十六进制";
                break;
            }
            return num.type == NumericLiteral::Type::Integer ? base + "整数" : base + "浮点数";
        }
        else if (holds_alternative<StringLiteral>(get<Literal>(token.type)))
        {
            const auto &str = get<StringLiteral>(get<Literal>(token.type));
            switch (str.kind)
            {
            case StringLiteral::Kind::Char:
                return "字符";
            case StringLiteral::Kind::Normal:
                return "字符串";
            case StringLiteral::Kind::Raw:
                return "原始字符串";
            case StringLiteral::Kind::ByteChar:
                return "字节字符";
            case StringLiteral::Kind::ByteNormal:
                return "字节字符串";
            case StringLiteral::Kind::ByteRaw:
                return "原始字节字符串";
            default:
                return "未知字符串类型";
            }
        }
    }
    return "未知类型";
}

string Tokenizer::getTokenValueString(const Token &token) const
{
    if (token.type.index() == 0)
    { // Literal
        if (holds_alternative<NumericLiteral>(get<Literal>(token.type)))
        {
            const auto &num = get<NumericLiteral>(get<Literal>(token.type));
            return num.raw_value + (num.has_suffix ? num.suffix : "");
        }
        else if (holds_alternative<StringLiteral>(get<Literal>(token.type)))
        {
            const auto &str = get<StringLiteral>(get<Literal>(token.type));
            return str.content;
        }
    }
    return "";
}
