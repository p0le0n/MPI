CXX = mpicxx
CXXFLAGS = -O3 -Wall
TARGETS = task4 task4_canon task4_canonmatrixgen

all: $(TARGETS)

task4: task4.cpp
	$(CXX) $(CXXFLAGS) task4.cpp -o task4

task4_canon: task4_canon.cpp
	$(CXX) $(CXXFLAGS) task4_canon.cpp -o task4_canon

task4_canonmatrixgen: task4_canonmatrixgen.cpp
	$(CXX) $(CXXFLAGS) task4_canonmatrixgen.cpp -o task4_canonmatrixgen

clean:
	rm -f A.txt B.txt
