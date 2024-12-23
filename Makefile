CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

TARGET = bin/msplot_demo
SRCS = msplot_demo.cpp

.PHONY: all clean run create_bin

all: create_bin $(TARGET1) $(TARGET)

create_bin:
	mkdir -p bin

$(TARGET): $(SRCS) | create_bin
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)

run: $(TARGET)
	pushd bin && ./msplot_demo && popd
