CC 	= g++
CFLAGS	= -Wall -c -g
LIBS 	= -lSDL2
TARGET 	= build/raycast
SOURCES = src/app.cpp
SOURCES += src/display/display.cpp src/display/engine.cpp
SOURCES += src/game/gamestate.cpp src/game/map.cpp
SOURCES += src/util/vector.cpp
OBJECTS	= $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p build
	$(CC) $(LIBS) -o $@ $(OBJECTS)
%.o: %.cpp
	$(CC) $(CFLAGS) -I src -o $@ $<

.PHONY: clean
clean: 
	rm -rf $(OBJECTS) $(TARGET)
