#nado menyat UNAME chtoby pomenyat OS

UNAME= linux

CC=g++
CFLAGS=-c -Wall

ifeq ($(UNAME),linux)

LDFLAGS=-lglfw3 -lGL -lGLU -lGLEW -liniparser -lstdc++ -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lXinerama -lXcursor
INC=#-I. glm/ -I GL/ -I common/ -I common/iniparser/
LIBS=#-L.
SOURCES=main.cpp  #hello.cpp factorial.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@
clean:
	rm -rf *.o $(EXECUTABLE)
clean2:
	rm -rf *.o
endif
ifeq ($(UNAME),win)

LDFLAGS=-lglfw3 -lglew32.dll -lini -lgdi32 -lopengl32 -static
INC=-I. glm/ -I GL/ -I common/ -I common/iniparser/
LIBS=-L.
SOURCES=main.cpp  #hello.cpp factorial.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@
clean:
	rm -rf *.o $(EXECUTABLE)
clean2:
	rm -rf *.o
endif

