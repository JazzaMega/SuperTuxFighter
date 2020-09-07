CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic
LDFLAGS = -lSDL -lSDL_image

SRC = main.cpp; 
OBJ = $(SRC:.cc=.o)
EXEC = SuperTuxFighter

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)