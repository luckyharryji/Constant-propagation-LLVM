CC=clang

all: machine_code_without_CAT machine_code_with_CAT 

machine_code_without_CAT: source_code.c
	$(CC) -O0 $^ -lm -o $@

machine_code_with_CAT: source_code.c
	$(CC) -O3 $^ -lm -o $@

run_without_CAT: machine_code_without_CAT
	taskset -c 2 ./$^ 5000 10.5

run_with_CAT: machine_code_with_CAT
	taskset -c 6 ./$^ 5000 10.5

clean:
	rm -f machine_code*
