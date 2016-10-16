CC_SUFFIX=-3.8
OPTIONS=-O0

all: hello_world

hello_world: hello_world.bc
	cat-c $(OPTIONS) $^ -emit-llvm -c -o hello_world_transformed.bc
	llvm-dis$(CC_SUFFIX) hello_world_transformed.bc
	clang$(CC_SUFFIX) hello_world_transformed.bc -o hello_world_transformed

hello_world.bc: hello_world.c
	clang$(CC_SUFFIX) -O0 -emit-llvm $^ -c -o $@
	llvm-dis$(CC_SUFFIX) $@
	clang$(CC_SUFFIX) $@ -o hello_world

clean:
	rm -f hello_world hello_world_transformed *.bc *.ll *.s hello_world_from_assembly a.out *.cpp hello_world_from_bitcode *.o
