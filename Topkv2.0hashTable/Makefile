CXX=g++
DEPS=topKURL.h
OBJ=main.o topKURL.o

%.o: %.cpp $(DEPS)
	$(CXX) -std=c++11 -Wall -c -o $@ $<

topKURL: $(OBJ)
	$(CXX) -std=c++11 -Wall -o $@ $^

topKURLTest: topKURL.o topKURLTest.o
	$(CXX) -std=c++11 -Wall -lgtest_main -lgtest -pthread -lpthread -o $@ $^

.PHONY: clean

clean:
	rm -f topKURL $(OBJ) topKURLTest topKURLTest.o
