// The ISO might frown furtively at this code but it's just
// for learning purposes

#include <string>
#include <iostream>
#include <algorithm>
#include <exception>

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

    TokenType get_token_type(void) {
        return this->token_type;
    }

    int get_int_value(void) {
        return std::stoi(this->value);
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
        std::cerr << "Unable to parse input" << std::endl;
        throw std::exception();
    }

    Token get_next_token(void) {
        auto text = this->input_text;
        if (this->pos > (text.length() - 1)) {
            Token token(TokenType::END_OF_FILE, std::string("EOF"));
            return token;
        }

        char charbuf[] = "x"; // placeholder
        charbuf[0] = text[this->pos];
        if (charbuf[0] >= '0' && charbuf[0] <= '9') {
            std::string int_literal = charbuf;
            while (text[this->pos + 1] >= '0' && text[this->pos + 1] <= '9') {
                this->pos++;
                int_literal += text[this->pos];
            }
            this->pos++;
            Token token(TokenType::INTEGER, int_literal);
            return token;
        }

        if (charbuf[0] == '+') {
            Token token(TokenType::PLUS, std::string(charbuf));
            this->pos++;
            return token;
        }

        if (charbuf[0] == '-') {
            Token token(TokenType::MINUS, std::string(charbuf));
            this->pos++;
            return token;
        }

        this->error();

        // prevent compiler warnings
        Token token;
        return token;
    }

    void eat(TokenType token_type) {
        if (this->current_token.get_token_type() == token_type) {
            this->current_token = this->get_next_token();
        } else {
            this->error();
        }
    }

    int expr(void) {
        this->current_token = this->get_next_token();
        auto left = this->current_token;
        this->eat(TokenType::INTEGER);
        auto op = this->current_token;
        switch (op.get_token_type()) {
            case TokenType::PLUS:
                this->eat(TokenType::PLUS); break;
            case TokenType::MINUS:
                this->eat(TokenType::MINUS); break;
        }
        auto right = this->current_token;
        this->eat(TokenType::INTEGER);
        int result;
        switch (op.get_token_type()) {
            case TokenType::PLUS:
                result = left.get_int_value() + right.get_int_value(); break;
            case TokenType::MINUS:
                result = left.get_int_value() - right.get_int_value(); break;
        }
        return result;
    }
};

int main(void) {
    while (!std::cin.eof()) {
        std::string input;
        std::cout << "Dante's Calculator Repl > ";
        std::getline(std::cin, input);
        if (input == "") {
            continue;
        }
        Interpreter interpreter(input);
        std::cout << interpreter.expr() << std::endl;
    }
    return 0;
}
