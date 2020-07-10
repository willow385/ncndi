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

std::string token_type_to_str(TokenType token_type) {
    switch (token_type) {
        case TokenType::NO_TOKEN:
            return std::string("NO_TOKEN");
        case TokenType::INTEGER:
            return std::string("INTEGER");
        case TokenType::PLUS:
            return std::string("PLUS");
        case TokenType::MINUS:
            return std::string("MINUS");
        case TokenType::END_OF_FILE:
            return std::string("END_OF_FILE");
    }

    return std::string("Undefined");
}

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
    char charbuf[2];
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
        charbuf[0] = text[0];
        charbuf[1] = '\0';
    }

    void error(std::string msg) {
        std::cerr << "Unable to parse input due to error: " << msg << std::endl;
        throw std::exception();
    }

    void advance(void) {
        this->pos++;
        if (this->pos > this->input_text.length()) {
            this->charbuf[0] = '\0';
        } else {
            this->charbuf[0] = this->input_text[this->pos];
        }
    }

    void skip_whitespace(void) {
        while (this->charbuf[0] == ' ' || this->charbuf[0] == '\t') {
            this->advance();
        }
    }

    std::string scan_integer(void) {
        std::string result = "";

        // Handle negative numbers
        if (this->charbuf[0] == '-') {
            result += this->charbuf;
            this->advance();
        }

        while (this->charbuf[0] >= '0' && this->charbuf[0] <= '9') {
            result += this->charbuf;
            this->advance();
        }

        return result;
    }

    Token get_next_token(void) {
        while (this->charbuf[0] != '\0') {
            if (this->charbuf[0] == ' ' || this->charbuf[0] == '\t') {
                this->skip_whitespace();
                continue;
            } else if (
                this->charbuf[0] == '-' &&
                    (this->input_text[this->pos + 1] >= '0' && this->input_text[this->pos + 1] <= '9')
            ) {
                return Token(TokenType::INTEGER, this->scan_integer());
            } else if (this->charbuf[0] >= '0' && this->charbuf[0] <= '9') {
                return Token(TokenType::INTEGER, this->scan_integer());
            } else if (this->charbuf[0] == '+') {
                this->advance();
                return Token(TokenType::PLUS, std::string("+"));
            } else if (this->charbuf[0] == '-') {
                this->advance();
                return Token(TokenType::MINUS, std::string("-"));
            } else {
                std::string errmsg = "invalid char '";
                errmsg += charbuf;
                errmsg += "' found";
                this->error(errmsg);
            }
        }

        Token token;
        return token;
    }

    void eat(TokenType token_type) {
        if (this->current_token.get_token_type() == token_type) {
            this->current_token = this->get_next_token();
        } else {
            std::string errmsg = "Expected ";
            errmsg += token_type_to_str(token_type);
            errmsg += ", found ";
            errmsg += token_type_to_str(this->current_token.get_token_type());
            this->error(errmsg);
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

        try {
            Interpreter interpreter(input);
            std::cout << interpreter.expr() << std::endl;
        } catch (std::exception& e) {
            continue;
        }
    }

    return 0;
}
