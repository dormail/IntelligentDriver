
# -Wall		more compiler warnings 
# -g 		debugging capabilities
CFLAGS = --include-directory=include -Wall -g

# clean stuff
RM = rm -rf

#
TARGET = main

# source files
SOURCE = include/car.cpp include/road.cpp

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	g++ $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(SOURCE)

valgrind: $(TARGET)
	valgrind -v ./$(TARGET)

valgrind-leak-check: $(TARGET)
	valgrind -v --leak-check=full ./$(TARGET)

clean:
	$(RM) $(TARGET)