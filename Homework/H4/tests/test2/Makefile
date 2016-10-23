LLVM_SUFFIX= 
OPTIONS=-I../misc

all: program_optimized.bc

program_optimized.bc: program.bc
	cat-c -emit-llvm -c $< -o $@
	llvm-dis$(LLVM_SUFFIX) $@

program.bc: program.c
	clang$(LLVM_SUFFIX) $(OPTIONS) -O0 -emit-llvm -c $<
	opt$(LLVM_SUFFIX) -mem2reg $@ -o $@
	llvm-dis$(LLVM_SUFFIX) $@

program: program.bc ../misc/CAT.bc
	clang$(LLVM_SUFFIX) -O3 $^ -o $@

program_optimized: program_optimized.bc ../misc/CAT.bc
	clang$(LLVM_SUFFIX) -O3 $^ -o $@

program_output: program
	./$< > $@

program_optimized_output: program_optimized
	./$< > $@

check: program_optimized.bc program_optimized_output
	../misc/check.sh "program_optimized_output" output2 "program_optimized.ll" output

clean:
	rm -f program program_optimized program_optimized_output *.bc *.ll a.out diff/* ;
	cp bitcode/* . ;
