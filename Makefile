# C++ compiler.  Pass CXX=<compiler> use something other than g++
CXX=g++
CXXFLAGS=-std=c++0x

all: api-java api-cpp client-cpp

api-java:
	cd api && ant

api-cpp:
	mkdir -p api/target/cmake
	cd api/target/cmake && cmake ../.. -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_CXX_FLAGS=$(CXXFLAGS)
	cmake --build api/target/cmake

server-scala:
	cd server-scala && sbt update compile

client-cpp:
	mkdir -p client-cpp/target/cmake
	cd client-cpp/target/cmake && cmake ../.. -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_CXX_FLAGS=$(CXXFLAGS)
	cmake --build client-cpp/target/cmake

clean:
	rm -rf api/target client-cpp/target
	cd server-scala && sbt clean

.PHONY: all api-java api-cpp server-scala client-cpp clean
