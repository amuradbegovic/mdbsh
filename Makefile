all:
	clang++ main.cpp posix_wrapper.cpp -o mdbsh
clean:
	rm mdbsh
