CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c95
LDFLAGS = -lSDL

SRC = main.c;
OBJ = $(SRC:.cc=.o)
EXEC = STFighter

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)