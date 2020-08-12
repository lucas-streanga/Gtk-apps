SRC1 = choice.cpp
SRC2 = chipotle.cpp
SRC3 = wins.cpp
OBJ1 = $(SRC1:.cpp=.o)
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
EXE = Chipotle

HFILES = globals.h chipotle.h wins.h

GTKMM = `pkg-config --cflags --libs gtkmm-3.0`

all : $(EXE)

$(EXE) : $(OBJ1) $(OBJ2) $(OBJ3)
	g++ -o $(EXE) $(OBJ1) $(OBJ2) $(OBJ3)  $(GTKMM)

$(OBJ1) : $(SRC1) $(HFILES)
	g++ -c -g $(SRC1) $(GTKMM) -o $(OBJ1)

$(OBJ2) : $(SRC2) $(HFILES)
	g++ -c -g $(SRC2) $(GTKMM) -o $(OBJ2)

$(OBJ3) : $(SRC3) $(HFILES)
	g++ -c -g $(SRC3) $(GTKMM) -o $(OBJ3)

cleanup:
	@echo "Deleting object and executable files..."
	rm *.o
	rm $(EXE)

clean:
	@echo "Clean build..."
	@make cleanup
	@make
