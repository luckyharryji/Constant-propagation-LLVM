CC_SUFFIX=-3.8
OPTIONS=-O1

all: hello_world

hello_world: hello_world.c
	cat-c $(OPTIONS) $^ -o $@

hello_world_from_bitcode_to_bitcode.bc: hello_world.bc
	cat-c $(OPTIONS) $^ -emit-llvm -c -o $@
	llvm-dis$(CC_SUFFIX) $@

hello_world_from_bitcode_to_binary: hello_world.bc
	llc$(CC_SUFFIX) -filetype=obj $^ -o hello_world.o
	clang$(CC_SUFFIX) hello_world.o -o hello_world

hello_world_from_bitcode_to_binary2: hello_world.bc
	clang$(CC_SUFFIX) hello_world.bc -o hello_world

invoke_1: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -CAT -disable-output $^

invoke_2: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -CAT -disable-output --debug-pass=Structure  $^

invoke_3: hello_world.bc
	opt$(CC_SUFFIX) -load ~/CAT/lib/CAT.* -CAT -disable-output -time-passes  $^


hello_world.bc: hello_world.c
	clang$(CC_SUFFIX) -O0 -emit-llvm $^ -c -o $@
	llvm-dis$(CC_SUFFIX) $@

clean:
	rm -f hello_world *.bc *.ll *.s hello_world_from_assembly a.out *.cpp hello_world_from_bitcode *.o
