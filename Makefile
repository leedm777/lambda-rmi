#
# lambda-rmi - a demonstration of using lambda funcitons for RMI
#
# Copyright (C) 2011, David M. Lee, II <leedm777@yahoo.com>
#
# This program is free software, distributed under the terms of
# the BSD License. See the LICENSE.txt file at the top of the
# source tree.
#

#
# Default settings.
#

CXX=g++
CXXFLAGS=-std=c++0x
ANT=ant
SBT=sbt
CMAKE=cmake
CLIENT_CPP_ARGS=

all: scala-stuff cpp-stuff

scala-stuff: server-scala

cpp-stuff: client-cpp

# install ice.jar into the local Maven repo
mvn-install-ice:
	cd api && $(ANT) install-ice

api-java:
	cd api && $(ANT)

api-cpp:
	mkdir -p api/target/cmake
	cd api/target/cmake && \
	  $(CMAKE) ../.. \
	    -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_CXX_FLAGS=$(CXXFLAGS)
	$(CMAKE) --build api/target/cmake

server-scala: api-java
	cd server-scala && $(SBT) update compile

client-cpp: api-cpp
	mkdir -p client-cpp/target/cmake
	cd client-cpp/target/cmake && \
	  $(CMAKE) ../.. \
	    -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_CXX_FLAGS=$(CXXFLAGS)
	$(CMAKE) --build client-cpp/target/cmake

webapp-scala: api-java
	cd webapp-scala && $(SBT) update compile

run-server: server-scala
	cd server-scala && $(SBT) run

run-webapp: webapp-scala
	cd webapp-scala && $(SBT) jetty

run-client-cpp: client-cpp
	./client-cpp/target/cmake/client-cpp

clean:
	rm -rf api/target client-cpp/target
	cd server-scala && $(SBT) clean

.PHONY: all scala-stuff cpp-stuff mvn-install-ice api-java api-cpp \
	server-scala run-server client-cpp clean
