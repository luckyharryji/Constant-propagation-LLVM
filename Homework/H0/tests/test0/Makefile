OPT_LEVEL=0

all: program1

program1: program1.c
	cat-c -O$(OPT_LEVEL) $^ -o $@ 2> compiler_output

program1.bc: program1.c
	cat-c -O$(OPT_LEVEL) -emit-llvm -c $^ -o $@
	llvm-dis-3.8 $@

run: program1
	./program1

check: program1
	../misc/check.sh "compiler_output" output

clean:
	rm -f program1 *.bc *.ll a.out compiler_output diff_output diff/*
