CXX = g++-7 -Wall -pedantic -std=c++17

BUILDDIR = build

SRC    = $(wildcard *.cc)
OBJ    = $(patsubst %.cc,$(BUILDDIR)/%.o,$(SRC))

GTEST_LINK = -lgtest -lgtest_main -lpthread -lboost_serialization

.PHONY: all
.PHONY: test
.PHONY: clean

all: test

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(OBJ): | $(BUILDDIR)

$(BUILDDIR)/%.o : %.cc
	$(CXX) -MMD $< -c -o $@

$(BUILDDIR)/test_main: $(OBJ) | $(BUILDDIR)
	$(CXX) $^ $(GTEST_LINK) -o $@

test: $(BUILDDIR)/test_main
	$(BUILDDIR)/test_main

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/*.d $(BUILDDIR)/test_main
	rmdir $(BUILDDIR)

-include build/*.d
