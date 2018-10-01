all: clean build run

clean:
	rm -rf build

build:
	mkdir build
	(cd build; cmake ..)
	(cd build; make)

run:
	build/proj1
