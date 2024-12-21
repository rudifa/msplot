CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

TARGET1 = bin/demo
TARGET2 = bin/msplot_demo
SRCS1 = demo.cpp plot.cpp svgex.cpp
SRCS2 = msplot_demo.cpp

.PHONY: all clean run create_bin

all: create_bin $(TARGET1) $(TARGET2)

create_bin:
	mkdir -p bin

$(TARGET1): $(SRCS1) | create_bin
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGET2): $(SRCS2) | create_bin
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGET1) $(TARGET2)

run: $(TARGET1) $(TARGET2)
	pushd bin && ./demo && ./msplot_demo && popd

run1: $(TARGET1)
	pushd bin && ./demo && popd

run2: $(TARGET2)
	pushd bin && ./msplot_demo && popd
