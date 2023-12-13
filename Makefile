NAME = progName

SRC = $(wildcard ./SRC/*.cpp)

HED =  

CXX = c++ 
CXXFLAGS= -std=c++98 -Wall -Werror -Wextra
OBJ = $(SRC:.cpp=.o)

all: $(NAME)



$(NAME): $(OBJ) 
		$(CXX) $(CXXFLAGS) $(OBJ) -o $@

clean:
		rm -rf $(OBJ)

fclean: clean
		rm -rf $(NAME)

re:     fclean all

run:all
		./$(NAME)