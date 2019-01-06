programs = future0 future1 future2 future3 future4 future5
CXX = clang++
CXXFLAGS = -std=c++14 -pthread
LDFLAGS = -pthread

all: $(programs)

clean:
	rm -f $(programs)
