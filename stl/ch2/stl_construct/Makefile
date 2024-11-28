CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = test
SRCS = test.cpp
INCLUDES = -I.
DEPS = stl_construct.h type_traits.h
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)