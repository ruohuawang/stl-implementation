CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = test.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)