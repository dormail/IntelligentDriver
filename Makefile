
# -Wall		more compiler warnings 
# -g 		debugging capabilities
CXXFLAGS = --include-directory=include -Wall -g

# clean stuff
RM = rm -rf

# source files
SOURCE = include/car.cpp include/road.cpp
HEADER = include/road.h include/car.h


all: build/main build/EulerCongestionToCSV build/MultiLaneTest

build/main: main.cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) -o $@ main.cpp $(SOURCE)

build/EulerCongestionToCSV: EulerCongestionToCSV.cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) EulerCongestionToCSV.cpp -o $@ $(SOURCE)

build/MultiLaneTest: tests/MultiLaneTest.cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) tests/MultiLaneTest.cpp -o $@ $(SOURCE)

valgrind: $(TARGET)
	valgrind -v ./$(TARGET)

valgrind-leak-check: $(TARGET)
	valgrind -v --leak-check=full ./$(TARGET)

build/data/position.csv: build/data build/EulerCongestionToCSV
	./build/EulerCongestionToCSV > build/data/position.csv

analysis/animation.mp4: analysis/SimpleAnimation.py build/data/position.csv
	python analysis/SimpleAnimation.py

build:
	mkdir -p build

build/data: build
	mkdir -p build/data

clean:
	$(RM) build
