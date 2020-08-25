
DEPS := main.cpp lexer.cpp

all: clean
	@g++ -O0 -std=c++17 -Wall -Wextra -Werror $(DEPS)
	@./a.out

clean:
	@rm -rf a.out
