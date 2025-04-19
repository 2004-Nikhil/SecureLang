#include "Lexer.h"
#include <iostream> // For error reporting, debugging
#include <cctype>   // For isspace, isalpha, isdigit, etc.
#include <utility>  // For std::move
#include <stdexcept> // For potential errors

// Initialize the static keyword map
const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"let", TokenType::LET},
    {"number", TokenType::NUMBER_TYPE},
    {"array", TokenType::ARRAY_TYPE},
    {"text", TokenType::TEXT_TYPE},
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
    {"print_text", TokenType::PRINT_TEXT},
    {"size_of", TokenType::SIZE_OF}
};

// TODO: Make column tracking in makeToken robust

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
     // This needs careful adjustment based on how start column is tracked.
    // Using a dedicated start_col variable captured before processing is best.
    // Example: assume startCol was captured before calling the helper function
    // return Token(type, value, line, startCol); // <--- Ideal uses saved startCol
    int startCol = std::max(1, column - (int)value.length()); // Approximation if startCol wasn't passed
    if (type == TokenType::STRING_LITERAL) startCol = std::max(1, column - (int)value.length() -1); // Approx for strings (need to account for quotes)
    return Token(type, value, line, startCol); // Needs real startCol passed or managed better
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
        // Check if previous token allows unary minus, otherwise it's binary
        // For lexer, assume it's part of number if followed by digit
        if (std::isdigit(static_cast<unsigned char>(peek()))) {
            numValue += currentChar;
            advance();
            isNegative = true;
        } else {
            // It's just a MINUS operator token
            advance(); // consume '-'
            return Token(TokenType::MINUS, "-", line, startCol);
        }
    }

    if (currentChar == EOF || !std::isdigit(static_cast<unsigned char>(currentChar))) {
        // Should only happen if '-' wasn't followed by digit and wasn't handled above
        // This indicates an error state or unexpected input
        std::string errVal(1,currentChar); advance();
        return Token(TokenType::UNKNOWN, errVal, line, startCol);
   }

   while (currentChar != EOF && std::isdigit(static_cast<unsigned char>(currentChar))) {
       numValue += currentChar;
       advance();
   }
    return Token(TokenType::NUMBER_LITERAL, numValue, line, startCol);
}

// *** NEW HELPER FUNCTION for String Literals ***
Token Lexer::stringLiteral() {
    int startLine = line;   // Record starting position
    int startCol = column;

    std::string value;
    advance(); // Consume the opening quote "

    while (currentChar != '"' && currentChar != EOF) {
        if (currentChar == '\n') {
             // Unterminated string literal error (strings cannot span lines)
             // Optionally report error here or return specific token
             std::cerr << "Lexer Error (Line " << startLine << ", Col " << startCol
                       << "): Unterminated string literal." << std::endl;
             return Token(TokenType::ERROR_UNTERMINATED_STRING, value, startLine, startCol);
        }

        if (currentChar == '\\') {
            // Handle escape sequence
            advance(); // Consume the backslash
            switch (currentChar) {
                case 'n': value += '\n'; advance(); break;
                case 't': value += '\t'; advance(); break;
                case '\\': value += '\\'; advance(); break;
                case '"': value += '"'; advance(); break;
                case EOF: // Unterminated escape sequence at EOF
                     std::cerr << "Lexer Error (Line " << line << ", Col " << column
                       << "): Unterminated escape sequence at end of file." << std::endl;
                    return Token(TokenType::ERROR_UNTERMINATED_STRING, value, startLine, startCol);
                default:
                    // Invalid escape sequence, treat as literal backslash + char
                    // Or report error:
                    std::cerr << "Lexer Warning (Line " << line << ", Col " << (column -1)
                              << "): Invalid escape sequence '\\" << currentChar << "'." << std::endl;
                    value += '\\'; // Keep the literal backslash
                    value += currentChar; advance(); // Keep the character
                    break;
            }
        } else {
            // Regular character in string
            value += currentChar;
            advance();
        }
    }

    if (currentChar == '"') {
        advance(); // Consume the closing quote
        return Token(TokenType::STRING_LITERAL, value, startLine, startCol);
    } else {
        // Reached EOF without closing quote
        std::cerr << "Lexer Error (Line " << startLine << ", Col " << startCol
                  << "): Unterminated string literal at end of file." << std::endl;
        return Token(TokenType::ERROR_UNTERMINATED_STRING, value, startLine, startCol);
    }
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
         if (std::isspace(static_cast<unsigned char>(currentChar))) {
            // This happens if skipWhitespace stops at \n and skipComment leaves us at \n
            advance(); // Consume the whitespace/newline
            continue;
       } // Might hit newline after comment


        int startCol = column; // Record start column *before* processing character


        // Check for different token types
        if (std::isalpha(static_cast<unsigned char>(currentChar)) || currentChar == '_') {
            return identifier(); // Handles keywords too
        }

        if (std::isdigit(static_cast<unsigned char>(currentChar))) {
            return number();
        }
        // *** ADDED: Check for string literal start ***
        if (currentChar == '"') {
            return stringLiteral();
        }

        // Operators and Punctuation
        switch (currentChar) {
            case '+': advance(); return Token(TokenType::PLUS, "+", line, startCol);
            case '-':
                // Check if it's potentially part of a negative number *or* just minus
                if (std::isdigit(static_cast<unsigned char>(peek()))) {
                    // Let number() handle it - it will consume '-' if needed
                    return number();
                } else {
                    // Just the MINUS operator
                    advance();
                    return Token(TokenType::MINUS, "-", line, startCol);
                }
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