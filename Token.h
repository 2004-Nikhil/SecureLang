#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant> // To potentially store different literal types later if needed

// Enum for all possible token types
enum class TokenType {
    // Keywords
    LET,         // let
    NUMBER_TYPE, // number (type name)
    ARRAY_TYPE,  // array (type name)
    OF,          // of (used in array declaration)
    IF,          // if
    THEN,        // then (optional, consider if needed with {})
    ELSE,        // else
    END_IF,      // end_if (optional, consider if needed with {})
    LOOP,        // loop
    WHILE,       // while
    DO,          // do (optional, consider if needed with {})
    END_LOOP,    // end_loop (optional, consider if needed with {})
    DEFINE,      // define
    RETURNS,     // returns
    RETURN,      // return
    READ_NUMBER, // read_number
    PRINT_NUMBER,// print_number
    SIZE_OF,     // size_of (could be keyword or operator depending on parsing)

    // Literals
    NUMBER_LITERAL, // e.g., 123, -45

    // Identifiers
    IDENTIFIER,     // e.g., my_variable

    // Operators and Punctuation
    PLUS,           // +
    MINUS,          // -
    STAR,           // *
    SLASH,          // /
    PERCENT,        // %
    ASSIGN,         // =
    EQUAL_EQUAL,    // ==
    BANG,           // ! (used in !=)
    BANG_EQUAL,     // !=
    LESS,           // <
    LESS_EQUAL,     // <=
    GREATER,        // >
    GREATER_EQUAL,  // >=
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    LBRACKET,       // [
    RBRACKET,       // ]
    COLON,          // :
    SEMICOLON,      // ; (Decide if you want semicolons to terminate statements)

    // Special Tokens
    COMMENT,        // ~ ... \n (Maybe skip directly, or make a token if needed)
    UNKNOWN,        // For unrecognized characters
    END_OF_FILE     // Represents the end of the input stream
};

// Structure to represent a token
struct Token {
    TokenType type;
    std::string value; // The actual text of the token (e.g., "my_var", "123", "+")
    int line;          // Line number where the token starts
    int column;        // Column number where the token starts

    // Constructor
    Token(TokenType t, std::string v, int l, int c)
        : type(t), value(std::move(v)), line(l), column(c) {}

    // For easy printing (optional but helpful for debugging)
    std::string toString() const {
        // Convert TokenType enum to string (implementation omitted for brevity,
        // but you'd have a switch or map here)
        std::string typeStr = "TokenType::..."; // Placeholder
        return "Token(" + typeStr + ", '" + value + "', Line: " + std::to_string(line) + ", Col: " + std::to_string(column) + ")";
    }
};

#endif // TOKEN_H