
# -Wall		more compiler warnings 
# -g 		debugging capabilities
CXXFLAGS = --include-directory=include -Wall -g

# clean stuff
RM = rm -rf

#
TARGET = main

# source files
SOURCE = include/car.cpp include/road.cpp
HEADER = include/road.h include/car.h

all: build/$(TARGET)

build/$(TARGET): $(TARGET).cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) -o build/$(TARGET) $(TARGET).cpp $(SOURCE)

valgrind: $(TARGET)
	valgrind -v ./$(TARGET)

valgrind-leak-check: $(TARGET)
	valgrind -v --leak-check=full ./$(TARGET)

build:
	mkdir -p build

clean:
	$(RM) $(TARGET) ConstantSpeedToCSV build

.phony: $(TARGET) ConstantSpeedToCSV