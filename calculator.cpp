// The ISO might frown furtively at this code but it's just
// for learning purposes

#include <string>
#include <iostream>
#include <algorithm>

enum class TokenType {
    NO_TOKEN,
    INTEGER,
    PLUS,
    MINUS,
    END_OF_FILE
};

class Token {
    TokenType token_type;
    std::string value;
public:
    Token(void) {
        this->token_type = TokenType::NO_TOKEN;
        this->value = std::string("No value");
    }

    Token(TokenType token_type, std::string value) {
        this->token_type = token_type;
        this->value = value;
    }

};

class Interpreter {
    std::string input_text;
    int pos;
    Token current_token;
public:
    Interpreter(std::string& text) {
        text.erase(
            std::remove(
                text.begin(),
                text.end(),
                ' '
            ),
            text.end()
        );
        this->input_text = text;
        this->pos = 0;
    }

    void error(void) {
        throw "Unable to parse input";
    }

    Token get_next_token(void) {
        auto text = this->input_text;
        if (this->pos > (text.length() - 1)) {
            Token token(TokenType::END_OF_FILE, std::string("EOF"));
            return token;
        }
        char current_char = text[this->pos];
        if (current_char >= '0' && current_char <= '9') {
            std::string int_literal = current_char;
            while (text[this->pos] != '\0' && (text[this->pos + 1] >= '0' && text[this->pos + 1] <= '9')) {
                this->pos++;
                int_literal += text[this->pos];
            }
            Token token(TokenType::INTEGER, int_literal);
            return token;
        }
        if (current_char == '+') {
            Token token(TokenType::PLUS, std::string(current_char));
        }
    }
};
