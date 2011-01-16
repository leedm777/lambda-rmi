all: api-java api-cpp

api-java:
	cd api && ant

api-cpp:
	mkdir -p api/target/cmake
	cd api/target/cmake && cmake ../..
	cmake --build api/target/cmake

clean:
	rm -rf api/target

.PHONY: all api-java api-cpp clean
