
# -Wall		more compiler warnings 
# -g 		debugging capabilities
CXXFLAGS = --include-directory=include -Wall -g --std=c++2a

# clean stuff
RM = rm -rf

# source files
SOURCE = include/car.cpp include/road.cpp
HEADER = include/road.h include/car.h


all: build/main build/EulerCongestionToCSV build/MultiLaneTest build/tests/OneOnLaneTest

build/main: main.cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) -o $@ main.cpp $(SOURCE)

build/EulerCongestionToCSV: EulerCongestionToCSV.cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) EulerCongestionToCSV.cpp -o $@ $(SOURCE)

valgrind: $(TARGET)
	valgrind -v ./$(TARGET)

valgrind-leak-check: $(TARGET)
	valgrind -v --leak-check=full ./$(TARGET)

build/data/position.csv: build/data build/EulerCongestionToCSV
	./build/EulerCongestionToCSV > build/data/position.csv

analysis/animation.mp4: analysis/SimpleAnimation.py build/data/position.csv
	python analysis/SimpleAnimation.py

# multi lane simulation
build/MultiLaneTest: tests/MultiLaneTest.cpp $(SOURCE) $(HEADER) build
	g++ $(CXXFLAGS) tests/MultiLaneTest.cpp -o $@ $(SOURCE)

build/data/multi_lane.csv: build/MultiLaneTest build/data
	./build/MultiLaneTest > $@

analysis/multi_lane.mp4: analysis/multi_lane.py build/data/multi_lane.csv
	python3 analysis/multi_lane.py

# tests
build/tests/OneOnLaneTest: tests/OneOnLaneTest.cpp $(SOURCE) $(HEADER) build/tests
	g++ $(CXXFLAGS) tests/OneOnLaneTest.cpp -o $@ $(SOURCE)

# folders
build:
	mkdir -p build

build/data: build
	mkdir -p build/data

build/tests: build
	mkdir -p build/tests

clean:
	$(RM) build analysis/*.mp4
