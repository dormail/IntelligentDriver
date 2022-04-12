
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

all: $(TARGET) ConstantSpeedToCSV

$(TARGET): $(TARGET).cpp $(SOURCE) $(HEADER)
	g++ $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp $(SOURCE)

ConstantSpeedToCSV: ConstantSpeedToCSV.cpp $(SOURCE) $(HEADER)
	g++ $(CXXFLAGS) -o ConstantSpeedToCSV $(SOURCE) ConstantSpeedToCSV.cpp

valgrind: $(TARGET)
	valgrind -v ./$(TARGET)

valgrind-leak-check: $(TARGET)
	valgrind -v --leak-check=full ./$(TARGET)

clean:
	$(RM) $(TARGET) ConstantSpeedToCSV

.phony: $(TARGET) ConstantSpeedToCSV