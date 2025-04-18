#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <istream> // For reading input
#include <string>
#include <unordered_map> // For keywords


class Lexer {
public:
    // Constructor: Takes an input stream (like std::cin or std::ifstream)
    Lexer(std::istream& input);

    // The main method: Gets the next token from the stream
    Token getNextToken();

private:
    std::istream& inputStream; // Reference to the input stream
    std::string currentLineText; // Store the current line for context (optional)
    int line;                  // Current line number (1-based)
    int column;                // Current column number (1-based)
    char currentChar;          // The character most recently read

    // Helper methods
    void advance();            // Read the next character from input
    char peek();               // Look at the next character without consuming it
    void skipWhitespace();     // Skip spaces, tabs, newlines
    void skipComment();        // Skip single-line comments starting with ~
    Token makeToken(TokenType type, const std::string& value); // Helper to create token with current pos
    Token identifier();         // Handles identifiers and keywords
    Token number();             // Handles number literals
    Token singleCharToken(TokenType type); // Handles simple single-char tokens

    // Keyword map
    static const std::unordered_map<std::string, TokenType> keywords;
};

#endif // LEXER_H