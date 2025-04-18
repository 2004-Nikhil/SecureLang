#include "Lexer.h"
#include <iostream> // For error reporting, debugging
#include <cctype>   // For isspace, isalpha, isdigit, etc.
#include <utility>  // For std::move

// Initialize the static keyword map
const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"let", TokenType::LET},
    {"number", TokenType::NUMBER_TYPE},
    {"array", TokenType::ARRAY_TYPE},
    {"of", TokenType::OF},
    {"if", TokenType::IF},
    {"then", TokenType::THEN}, // Keep if needed
    {"else", TokenType::ELSE},
    {"end_if", TokenType::END_IF}, // Keep if needed
    {"loop", TokenType::LOOP},
    {"while", TokenType::WHILE},
    {"do", TokenType::DO}, // Keep if needed
    {"end_loop", TokenType::END_LOOP}, // Keep if needed
    {"define", TokenType::DEFINE},
    {"returns", TokenType::RETURNS},
    {"return", TokenType::RETURN},
    {"read_number", TokenType::READ_NUMBER},
    {"print_number", TokenType::PRINT_NUMBER},
    {"size_of", TokenType::SIZE_OF}
};


Lexer::Lexer(std::istream& input) : inputStream(input), line(1), column(0) {
    advance(); // Initialize currentChar
}

void Lexer::advance() {
    if (inputStream.get(currentChar)) { // Read next character
        if (currentChar == '\n') {
            line++;
            column = 0; // Reset column on new line
             currentLineText = ""; // Reset line text (optional)
        } else {
            column++;
            currentLineText += currentChar; // Append char to line text (optional)
        }
    } else {
        // End of file reached or read error
        currentChar = EOF; // Use EOF (or 0) to signal end
    }
}

char Lexer::peek() {
    return static_cast<char>(inputStream.peek()); // Look ahead
}

void Lexer::skipWhitespace() {
    while (currentChar != EOF && std::isspace(static_cast<unsigned char>(currentChar))) {
        advance();
    }
}

void Lexer::skipComment() {
    if (currentChar == '~') {
        // Consume characters until newline or EOF
        while (currentChar != EOF && currentChar != '\n') {
            advance();
        }
        // We might be at EOF or '\n', whitespace skipping will handle the '\n' if needed
    }
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
     // Use the column where the token *started*, which might be tricky if not saved
     // A simple approach: use current column - value.length() (approximate)
     int startColumn = std::max(1, column - (int)value.length() + (type == TokenType::NUMBER_LITERAL && value.length() > 0 && value[0] == '-' ? 1: 0) ); // Adjust for unary minus recognition
     if (type == TokenType::IDENTIFIER || type == TokenType::NUMBER_LITERAL) startColumn = column - value.length() +1; // More reliable for these

     return Token(type, value, line, startColumn);
}

// Helper for simple single-character tokens
Token Lexer::singleCharToken(TokenType type) {
     std::string val(1, currentChar);
     advance(); // Consume the character
     return makeToken(type, val);
}


Token Lexer::identifier() {
    std::string identValue;
    int startCol = column; // Record starting column
    while (currentChar != EOF && (std::isalnum(static_cast<unsigned char>(currentChar)) || currentChar == '_')) {
        identValue += currentChar;
        advance();
    }

    // Check if it's a keyword
    auto keywordIter = keywords.find(identValue);
    if (keywordIter != keywords.end()) {
        // It's a keyword
         return Token(keywordIter->second, identValue, line, startCol);
    } else {
        // It's a regular identifier
         return Token(TokenType::IDENTIFIER, identValue, line, startCol);
    }
}

Token Lexer::number() {
    std::string numValue;
    int startCol = column; // Record starting column
    bool isNegative = false;
    if(currentChar == '-'){
        numValue += currentChar;
        advance();
        isNegative = true;
    }

    if (currentChar == EOF || !std::isdigit(static_cast<unsigned char>(currentChar))) {
        // If '-' wasn't followed by a digit, it's just a MINUS operator
        if(isNegative) {
            return Token(TokenType::MINUS, "-", line, startCol);
        } else {
             // Should not happen if called correctly, maybe error?
             return Token(TokenType::UNKNOWN, std::string(1,currentChar), line, startCol);
        }
    }


    while (currentChar != EOF && std::isdigit(static_cast<unsigned char>(currentChar))) {
        numValue += currentChar;
        advance();
    }
     return Token(TokenType::NUMBER_LITERAL, numValue, line, startCol);
}


Token Lexer::getNextToken() {
    while (currentChar != EOF) {
        skipWhitespace(); // Skip leading whitespace

        if (currentChar == '~') { // Handle comments
            skipComment();
            continue; // Go back to the start of the loop to handle what's next
        }

        // After skipping, check again for EOF or other characters
         if (currentChar == EOF) break;
         if (std::isspace(static_cast<unsigned char>(currentChar))) continue; // Might hit newline after comment


        int startCol = column; // Record start column *before* processing character


        // Check for different token types
        if (std::isalpha(static_cast<unsigned char>(currentChar)) || currentChar == '_') {
            return identifier(); // Handles keywords too
        }

        if (std::isdigit(static_cast<unsigned char>(currentChar))) {
            return number();
        }
         // Handle potential negative numbers vs. minus operator
        if (currentChar == '-') {
             // Peek ahead: if followed by digit, it's part of a number
             if (std::isdigit(static_cast<unsigned char>(peek()))) {
                 return number(); // number() handles the '-' sign
             } else {
                 // Otherwise, it's the MINUS operator
                  return singleCharToken(TokenType::MINUS);
             }
        }


        // Operators and Punctuation
        switch (currentChar) {
            case '+': advance(); return Token(TokenType::PLUS, "+", line, startCol);
            // case '-': handled above/in number()
            case '*': advance(); return Token(TokenType::STAR, "*", line, startCol);
            case '/': advance(); return Token(TokenType::SLASH, "/", line, startCol);
            case '%': advance(); return Token(TokenType::PERCENT, "%", line, startCol);
            case '(': advance(); return Token(TokenType::LPAREN, "(", line, startCol);
            case ')': advance(); return Token(TokenType::RPAREN, ")", line, startCol);
            case '{': advance(); return Token(TokenType::LBRACE, "{", line, startCol);
            case '}': advance(); return Token(TokenType::RBRACE, "}", line, startCol);
            case '[': advance(); return Token(TokenType::LBRACKET, "[", line, startCol);
            case ']': advance(); return Token(TokenType::RBRACKET, "]", line, startCol);
            case ':': advance(); return Token(TokenType::COLON, ":", line, startCol);
            case ';': advance(); return Token(TokenType::SEMICOLON, ";", line, startCol); // If using semicolons

            case '=':
                if (peek() == '=') {
                    advance(); // consume '='
                    advance(); // consume second '='
                    return Token(TokenType::EQUAL_EQUAL, "==", line, startCol);
                } else {
                    advance(); // consume '='
                    return Token(TokenType::ASSIGN, "=", line, startCol);
                }
            case '!':
                if (peek() == '=') {
                    advance(); // consume '!'
                    advance(); // consume '='
                    return Token(TokenType::BANG_EQUAL, "!=", line, startCol);
                } else {
                     // Just '!', maybe an error or a NOT operator if you add booleans later
                    advance();
                     return Token(TokenType::UNKNOWN, "!", line, startCol); // Or BANG if defined
                }
            case '<':
                if (peek() == '=') {
                    advance(); advance();
                    return Token(TokenType::LESS_EQUAL, "<=", line, startCol);
                } else {
                    advance();
                    return Token(TokenType::LESS, "<", line, startCol);
                }
            case '>':
                if (peek() == '=') {
                    advance(); advance();
                    return Token(TokenType::GREATER_EQUAL, ">=", line, startCol);
                } else {
                    advance();
                    return Token(TokenType::GREATER, ">", line, startCol);
                }

            default:
                // Unrecognized character
                std::string unknownChar(1, currentChar);
                advance(); // Consume the unknown char to avoid infinite loop
                return Token(TokenType::UNKNOWN, unknownChar, line, startCol);
        }
    }

    // End of input stream
    return Token(TokenType::END_OF_FILE, "", line, column); // EOF token
}