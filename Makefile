
# -Wall		more compiler warnings 
# -g 		debugging capabilities
CXXFLAGS = --include-directory=include -Wall -g

# clean stuff
RM = rm -rf

# source files
SOURCE = include/car.cpp include/road.cpp
HEADER = include/road.h include/car.h


all: build/main build/ConstantSpeedToCSV

build/main: main.cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) -o $@ main.cpp $(SOURCE)

build/ConstantSpeedToCSV: ConstantSpeedToCSV.cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) -o $@ ConstantSpeedToCSV.cpp $(SOURCE)

valgrind: $(TARGET)
	valgrind -v ./$(TARGET)

valgrind-leak-check: $(TARGET)
	valgrind -v --leak-check=full ./$(TARGET)

build:
	mkdir -p build

clean:
	$(RM) build