INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj
PACS_ROOT ?= /home/mteresa0/pacs/pacs-examples/Examples
PACS_INC_DIR = $(PACS_ROOT)/include
INCLUDE = -I$(INC_DIR) -I$(PACS_INC_DIR)

CXX = g++
CXXFLAGS += -std=c++20 -O3
CPPFLAGS = -Wall -Wextra $(INCLUDE) 

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
HDRS = $(wildcard $(INC_DIR)/*.hpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
EXEC = main

run : main
	./main

all : clean
	make main
	./main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) -o main

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

boh : $(SRCS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o main $^

.PHONY : all clean

clean : 
	rm -f main
	rm -f $(OBJS)