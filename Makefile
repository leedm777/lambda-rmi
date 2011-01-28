all: api-java api-cpp client-cpp

api-java:
	cd api && ant

api-cpp:
	mkdir -p api/target/cmake
	cd api/target/cmake && cmake ../..
	cmake --build api/target/cmake

client-cpp:
	mkdir -p client-cpp/target/cmake
	cd client-cpp/target/cmake && cmake ../..
	cmake --build client-cpp/target/cmake

clean:
	rm -rf api/target client-cpp/target

.PHONY: all api-java api-cpp client-cpp clean
