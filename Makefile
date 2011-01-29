all: api-java api-cpp client-cpp

api-java:
	cd api && ant

api-cpp:
	mkdir -p api/target/cmake
	cd api/target/cmake && cmake ../..
	cmake --build api/target/cmake

server-scala:
	cd server-scala && sbt update compile

client-cpp:
	mkdir -p client-cpp/target/cmake
	cd client-cpp/target/cmake && cmake ../..
	cmake --build client-cpp/target/cmake

clean:
	rm -rf api/target client-cpp/target
	cd server-scala && sbt clean

.PHONY: all api-java api-cpp server-scala client-cpp clean
