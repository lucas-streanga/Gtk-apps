SRC1 = keyboard.cpp
OBJ1 = $(SRC1:.cpp=.o)
EXE = Keyboard

HFILES = keyboard.xml

GTKMM = `pkg-config --cflags --libs gtkmm-3.0`

all : $(EXE)

$(EXE) : $(OBJ1) $(OBJ2) $(OBJ3)
	g++ -o $(EXE) $(OBJ1) $(OBJ2) $(OBJ3)  $(GTKMM)

$(OBJ1) : $(SRC1) $(HFILES)
	g++ -c -g $(SRC1) $(GTKMM) -o $(OBJ1)

cleanup:
	@echo "Deleting object and executable files..."
	rm *.o

clean:
	@echo "Clean build..."
	rm $(EXE)
	@make cleanup
	@make

full-rebuild:
	@echo "Full Rebuild..."
	@make clean
	@make cleanup
