CC_SUFFIX=-3.8
OPTIONS=-O0

all: hello_world

hello_world: hello_world.bc
	cat-c $(OPTIONS) $^ -o $@

hello_world.bc: hello_world.c
	clang$(CC_SUFFIX) -O0 -emit-llvm $^ -c -o $@
	opt$(CC_SUFFIX) -mem2reg $@ -o $@
	llvm-dis$(CC_SUFFIX) $@

clean:
	rm -f hello_world *.bc *.ll *.s hello_world_from_assembly a.out *.cpp hello_world_from_bitcode *.o
