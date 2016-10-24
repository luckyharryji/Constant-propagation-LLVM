CC_SUFFIX=-3.8
OPTIONS=-O0

all: program

program: program.bc
	cat-c $(OPTIONS) $^ -emit-llvm -c -o program_transformed.bc
	llvm-dis$(CC_SUFFIX) program_transformed.bc
	clang$(CC_SUFFIX) program_transformed.bc -o program_transformed

program.bc: program.c
	clang$(CC_SUFFIX) -O0 -emit-llvm $^ -c -o $@
	llvm-dis$(CC_SUFFIX) $@
	clang$(CC_SUFFIX) $@ -o program

clean:
	rm -f program program_transformed *.bc *.ll *.s program_from_assembly a.out *.cpp program_from_bitcode *.o
