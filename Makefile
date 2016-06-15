all: clean-all build-all

build-app:
	cd build && make -s && cd ..
	
clean:
	cd build && make clean && cd ..
	
clean-all:
	rm -rf build/*
	
build-all:
	cd build && cmake .. -G "MSYS Makefiles" && make -s && cd ..