NAME = progName

SRC = $(wildcard ./SRC/*.cpp) $(wildcard ./parcing/*.cpp)
HED = $(wildcard ./INC/*.hpp)


CXX = c++ 
CXXFLAGS= -std=c++98 -Wall -Werror -Wextra -fsanitize=address -g
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

%.o:%.cpp $(HED)
	$(CXX) $(CXXFLAGS) -c $< -o $@



$(NAME): $(OBJ) 
		$(CXX) $(CXXFLAGS) $(OBJ) -o $@

clean:
		rm -rf $(OBJ)

fclean: clean
		rm -rf $(NAME)

re:     fclean all

run:all
		./$(NAME)