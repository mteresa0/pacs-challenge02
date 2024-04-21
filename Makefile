CXX = g++
CXXFLAGS += -std=c++20
CPPFLAGS = -Wall -Wextra -Iinc -Iexternals/json/include

INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
HDRS = $(wildcard $(INC_DIR)/*.hpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
EXEC = main

all : main

run : clean
	make main
	./main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) -o main


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -Iinc -c $< -o $@

boh : $(SRCS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o main $^

.PHONY : all clean

clean : 
	rm -f main
	rm -f $(OBJS)