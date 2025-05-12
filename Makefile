# Makefile

CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -I.

# 1) Собираем все .cpp для основной программы
SRCS      := $(wildcard *.cpp encode/*.cpp decode/*.cpp src/*.cpp)
OBJS      := $(SRCS:.cpp=.o)

# 2) Отдельно главную точку входа и "остальные" модули
MAIN_OBJ  := main.o
CODE_OBJS := $(filter-out $(MAIN_OBJ),$(OBJS))

# 3) Тестовые исходники и объектники
TEST_SRCS := $(wildcard test/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

# Имена бинарников
APP       := main
TEST_APP  := test_runner

.PHONY: all test clean

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_APP)
	@./$(TEST_APP)

# В тестовый раннер НЕ попадает main.o
$(TEST_APP): $(TEST_OBJS) $(CODE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lgtest -pthread

# Общее правило .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(APP) $(TEST_APP)