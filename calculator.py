INTEGER = "INTEGER"
PLUS = "PLUS"
EOF = "EOF"

class Token:
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return f"Token({self.type}, {self.value})"

    def __repr__(self):
        return self.__str__()

class Interpreter:
    def __init__(self, text):
        self.input_text = text.replace(" ", "") # strip whitespace
        self.pos = 0
        self.current_token = None

    def error(self):
        raise Exception("Unable to parse input")

    def get_next_token(self):
        text = self.input_text
        if self.pos > len(text) - 1:
            return Token(EOF, None)
        current_char = text[self.pos]
        if current_char.isdigit():
            int_literal = current_char
            try:
                while text[self.pos + 1].isdigit():
                    self.pos += 1
                    int_literal += text[self.pos]
            except IndexError:
                pass
            token = Token(INTEGER, int(int_literal))
            self.pos += 1
            return token
        if current_char == '+':
            token = Token(PLUS, current_char)
            self.pos += 1
            return token
        self.error()

    def eat(self, token_type):
        if self.current_token.type == token_type:
            self.current_token = self.get_next_token()
        else:
            self.error()

    def expr(self):
        self.current_token = self.get_next_token()
        left = self.current_token
        self.eat(INTEGER)
        operator = self.current_token
        self.eat(PLUS)
        right = self.current_token
        self.eat(INTEGER)
        result = left.value + right.value
        return result

def main():
    while True:
        try:
            text = input("Dante's Calculator Repl > ")
        except EOFError:
            break
        if not text:
            continue
        interpreter = Interpreter(text)
        result = interpreter.expr()
        print(result)

if __name__ == "__main__":
    main()