source_files = $(wildcard src/*.c)
out = out

main:
	clang -std=c11 -Wall -Wextra $(source_files) -lraylib -o $(out)

run: main
	./$(out)

lint:
	clang-tidy $(source_files)
	clang-check $(source_files)

clean:
	rm -f $(out)

