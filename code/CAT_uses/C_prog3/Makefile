CC=clang

all: machine_code_without_CAT machine_code_with_CAT 

machine_code_without_CAT: source_code.c
	$(CC) -O0 $^ -lm -o $@

machine_code_with_CAT: source_code.c
	$(CC) -O3 -fvectorize -fslp-vectorize -fslp-vectorize-aggressive -ffast-math -fno-signed-zeros -fno-stack-protector $^ -lm -o $@

run_without_CAT: machine_code_without_CAT
	./$^ 2000000 10.5

run_with_CAT: machine_code_with_CAT
	./$^ 2000000 10.5

clean:
	rm -f machine_code* *.bc *.ll
