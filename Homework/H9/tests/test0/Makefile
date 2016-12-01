LLVM_SUFFIX= 
INCLUDES_TO_CONSIDER=-I../misc
LLVMPASSPATH=~/CAT/lib/CAT.so

all: program_optimized.bc program.bc

program.bc: program.c
	clang$(LLVM_SUFFIX) ${INCLUDES_TO_CONSIDER} -O0 -emit-llvm -c $<
	opt$(LLVM_SUFFIX) -mem2reg $@ -o $@
	llvm-link$(LLVM_SUFFIX) $@ ../misc/CAT.bc -o $@
	llvm-dis$(LLVM_SUFFIX) $@

program_optimized.bc: program.bc
	../misc/time.sh $< $@ ${LLVMPASSPATH} 5
	llvm-dis $@

time_pass: program.bc
	../misc/time.sh $< $@ ${LLVMPASSPATH} 10

program: program.bc
	clang$(LLVM_SUFFIX) -O3 -lm $< -o $@

program_optimized: program_optimized.bc
	clang$(LLVM_SUFFIX) -O3 -lm $< -o $@

program_output: program
	./$< > $@

program_optimized_output: program_optimized
	./$< > $@
	../misc/remove_last_line.sh $@

check: program_optimized_output
	../misc/check.sh "program_optimized_output" output2

clean:
	rm -f program program_optimized program_optimized_output *.bc *.ll a.out diff/* tmpOut time_pass program_output ;
	cp bitcode/* . ;

.PHONY: time_pass clean
