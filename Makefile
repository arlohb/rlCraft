source_files = $(wildcard src/*.cpp)
out = out

main:
	clang++ -std=c++11 -Wall -Wextra $(source_files) -lraylib -lm -g -o $(out)

run: main
	./$(out)

lint:
	clang-tidy $(source_files)
	clang-check $(source_files)

clean:
	rm -f $(out)

