CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -I.

SRCS      := $(wildcard *.cpp encode/*.cpp decode/*.cpp src/*.cpp)
OBJS      := $(SRCS:.cpp=.o)

MAIN_OBJ  := main.o
CODE_OBJS := $(filter-out $(MAIN_OBJ),$(OBJS))

TEST_SRCS := $(wildcard test/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

APP       := main
TEST_APP  := test_runner

.PHONY: all test clean

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_APP)
	@./$(TEST_APP)

$(TEST_APP): $(TEST_OBJS) $(CODE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lgtest -pthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(APP) $(TEST_APP)