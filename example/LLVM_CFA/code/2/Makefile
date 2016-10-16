CC_SUFFIX=-3.8
OPTIONS=-O0

all: hello_world

hello_world: invoke_print.bc print_hello_world.bc
	cat-c $(OPTIONS) $^ -emit-llvm -c
	clang$(CC_SUFFIX) *.bc -o hello_world_transformed

hello_world_all: all.bc
	cat-c $(OPTIONS) $^ -emit-llvm -c -o hello_world_transformed.bc
	llvm-dis$(CC_SUFFIX) hello_world_transformed.bc
	clang$(CC_SUFFIX) hello_world_transformed.bc -o hello_world_transformed

invoke_print.bc: invoke_print.c
	clang$(CC_SUFFIX) $(OPTIONS) -emit-llvm $^ -c -o $@
	llvm-dis$(CC_SUFFIX) $@

print_hello_world.bc: print_hello_world.c
	clang$(CC_SUFFIX) $(OPTIONS) -emit-llvm $^ -c -o $@
	llvm-dis$(CC_SUFFIX) $@

all.bc: invoke_print.bc print_hello_world.bc
	llvm-link$(CC_SUFFIX) $^ -o $@

clean:
	rm -f hello_world hello_world_transformed *.bc *.ll *.s hello_world_from_assembly a.out *.cpp hello_world_from_bitcode *.o
