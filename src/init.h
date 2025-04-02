#ifndef INIT_H
#define INIT_H
#include <unordered_set>
#include <string_view> //need c++17
#include <string>
#include <variant> //need c++17
using namespace std;

inline unordered_set<string_view> strict_keywords = {
    "as",
    "async",
    "await",
    "break",
    "const",
    "continue",
    "crate",
    "else",
    "enum",
    "extern",
    "false",
    "fn",
    "for",
    "if",
    "impl",
    "in",
    "let",
    "loop",
    "match",
    "mod",
    "move",
    "mut",
    "pub",
    "ref",
    "return",
    "self",
    "Self",
    "static",
    "struct",
    "super",
    "trait",
    "true",
    "type",
    "unsafe",
    "use",
    "where",
    "while",
    "dyn"};

inline unordered_set<string_view> reserved_keywords = {
    "abstract",
    "become",
    "box",
    "do",
    "final",
    "macro",
    "override",
    "priv",
    "typeof",
    "unsized",
    "virtual",
    "yield",
    "try"};

inline unordered_set<string_view> weak_keywords = {
    "union",
    "'static",
    "macro_rules"};

inline const unordered_set<char32_t> whitespaces = {
    U'\t',     // U+0009
    U'\n',     // U+000A
    U'\r',     // U+000D
    U' ',      // U+0020
    U'\u0085', // U+0085
    U'\u200E', // U+200E
    U'\u200F', // U+200F
    U'\u2028', // U+2028
    U'\u2029'  // U+2029
};

// 运算符集合
inline const unordered_set<char> single_char_ops = {
    '=', '<', '>', '!', '~', '+', '-', '*', '/', '%',
    '^', '&', '|', '.', '#', '?', '@'};

inline const unordered_set<string_view> compound_ops = {
    "==", "!=", "<=", ">=", "+=", "-=", "*=", "/=", "%=",
    "^=", "&=", "|=", ">>", "<<", "->", "=>", "...",
    "|>", "&&", "||"};

// 分隔符集合
inline const unordered_set<char> single_char_delimiters = {
    '(', ')', '{', '}', '[', ']', ',', ';', ':'};

inline const unordered_set<string_view> compound_delimiters = {
    "::",
    ".."};

struct NumericLiteral
{
    enum class Base
    {
        Binary,
        Octal,
        Decimal,
        Hexadecimal
    };
    enum class Type
    {
        Integer,
        Float
    };

    string raw_value; // 原始字符串（含下划线）
    Base base;        // 进制类型
    Type type;        // 整数/浮点
    bool has_suffix;  // 是否带类型后缀（如u32）
    string suffix;    // 后缀内容（如果有）
};

struct StringLiteral
{
    enum class Kind
    {
        Char,       // 'H'
        Normal,     // "hello"
        Raw,        // r#"..."#
        ByteChar,   // b"H"
        ByteNormal, // b"hello"
        ByteRaw,    // br#"..."#
    };

    string content; // 去引号后的内容
    Kind kind;
    int hash_count = 0;     // 仅对Raw字符串有效（r#中的#数量）
    bool is_unicode_escape; // 是否包含\u{1F600}类转义
};

using Literal = variant<
    NumericLiteral,
    StringLiteral,
    bool>;

enum class TokenKind
{
    Identifier, // 标识符
    Keyword,    // 关键字
    Number,     // 数字
    String,     // 字符串
    Comment,    // 注释
    Operator,   // 运算符
    Delimiter,  // 分隔符
    MacroCall   // 宏调用名
};

using RustTokenType = variant<
    Literal,
    TokenKind,
    string_view,
    string>;

#endif
