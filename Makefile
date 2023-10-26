build_dir:
	rm -rf build
	mkdir build

cmake_init: build_dir
	cd build && cmake ..

build: cmake_init
	cd build && make

clean: 
	rm -r build


debug:
	cd build && gf2 ./jsonq
