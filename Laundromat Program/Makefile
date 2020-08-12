SRC1 = laundry.cpp
SRC2 = store.cpp
SRC3 = machines.cpp
SRC4 = patrons.cpp
OBJ1 = $(SRC1:.cpp=.o)
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
OBJ4 = $(SRC4:.cpp=.o)
EXE = laundry

HFILES = store.h machines.h patrons.h

GTKMM = `pkg-config --cflags --libs gtkmm-3.0`

all : $(EXE)

$(EXE) : $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)
	g++ -o $(EXE) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(GTKMM)

$(OBJ1) : $(SRC1) $(HFILES)
	g++ -c -g $(SRC1) $(GTKMM) -o $(OBJ1)

$(OBJ2) : $(SRC2) $(HFILES)
	g++ -c -g $(SRC2) $(GTKMM) -o $(OBJ2)

$(OBJ3) : $(SRC3) $(HFILES)
	g++ -c -g $(SRC3) $(GTKMM) -o $(OBJ3)

$(OBJ4) : $(SRC4) $(HFILES)
	g++ -c -g $(SRC4) $(GTKMM) -o $(OBJ4)

cleanup:
	@echo "Deleting object and executable files..."
	rm *.o
	rm $(EXE)

clean:
	@echo "Clean build..."
	@make cleanup
	@make
