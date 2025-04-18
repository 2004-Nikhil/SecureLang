#include <iostream>
#include <fstream>
#include <vector>
#include "Lexer.h" // Assuming Lexer.h includes Token.h

// Helper function to convert TokenType to string for printing
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LET: return "LET";
        case TokenType::NUMBER_TYPE: return "NUMBER_TYPE";
        case TokenType::ARRAY_TYPE: return "ARRAY_TYPE";
        case TokenType::OF: return "OF";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::ELSE: return "ELSE";
        case TokenType::END_IF: return "END_IF";
        case TokenType::LOOP: return "LOOP";
        case TokenType::WHILE: return "WHILE";
        case TokenType::DO: return "DO";
        case TokenType::END_LOOP: return "END_LOOP";
        case TokenType::DEFINE: return "DEFINE";
        case TokenType::RETURNS: return "RETURNS";
        case TokenType::RETURN: return "RETURN";
        case TokenType::READ_NUMBER: return "READ_NUMBER";
        case TokenType::PRINT_NUMBER: return "PRINT_NUMBER";
        case TokenType::SIZE_OF: return "SIZE_OF";
        case TokenType::NUMBER_LITERAL: return "NUMBER_LITERAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::PERCENT: return "PERCENT";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::BANG: return "BANG"; // If you have it
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::COLON: return "COLON";
        case TokenType::SEMICOLON: return "SEMICOLON"; // If used
        case TokenType::COMMENT: return "COMMENT"; // If used
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        default: return "INVALID_TOKEN_TYPE";
    }
}


int main(int argc, char* argv[]) {
    std::istream* inputStream = &std::cin; // Default to standard input
    std::ifstream fileStream;

    if (argc > 1) {
        fileStream.open(argv[1]);
        if (!fileStream) {
            std::cerr << "Error: Could not open file '" << argv[1] << "'" << std::endl;
            return 1;
        }
        inputStream = &fileStream;
        std::cout << "Reading from file: " << argv[1] << std::endl;
    } else {
        std::cout << "Reading from standard input. Enter code and press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish." << std::endl;
    }

    Lexer lexer(*inputStream);
    std::vector<Token> tokens;
    Token currentToken = lexer.getNextToken();

    while (currentToken.type != TokenType::END_OF_FILE) {
        tokens.push_back(currentToken);
         // Print token info for debugging
        std::cout << "Token( " << tokenTypeToString(currentToken.type)
                  << ", '" << currentToken.value
                  << "', L:" << currentToken.line
                  << ", C:" << currentToken.column << " )" << std::endl;

        currentToken = lexer.getNextToken();
    }

    // Add the final EOF token
    tokens.push_back(currentToken);
     std::cout << "Token( " << tokenTypeToString(currentToken.type)
              << ", '" << currentToken.value
              << "', L:" << currentToken.line
              << ", C:" << currentToken.column << " )" << std::endl;


    std::cout << "\nLexing finished. Total tokens: " << tokens.size() << std::endl;

    // You can now pass the 'tokens' vector to your Parser

    return 0;
}