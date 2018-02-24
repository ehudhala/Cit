CXX = g++-7 -Wall -pedantic -std=c++17

SRC    = $(wildcard *.cc)
HEAD   = $(SRC:.cc=.h)
OBJ    = $(SRC:.cc=.o)
NO_TEST_OBJ = $(patsubst test_%,%,$(OBJ))

.PHONY: all
.PHONY: test
.PHONY: clean

all: test

%.o: %.cc
	$(CXX) -c -MMD $<

driver: $(NO_TEST_OBJ)
	$(CXX) $^ -o driver

main: $(OBJ)
	$(CXX) $^ -lgtest -lgtest_main -lpthread -lboost_serialization -o main

test: main
	./main

clean:
	rm -f *.o *.d main

-include *.d
