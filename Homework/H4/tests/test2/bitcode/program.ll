; ModuleID = 'program.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [21 x i8] c"H1: \09Value 1 = %lld\0A\00", align 1
@.str.1 = private unnamed_addr constant [21 x i8] c"H1: \09Value 2 = %lld\0A\00", align 1
@.str.2 = private unnamed_addr constant [21 x i8] c"H1: \09Value 3 = %lld\0A\00", align 1
@.str.3 = private unnamed_addr constant [21 x i8] c"H1: \09Value 4 = %lld\0A\00", align 1
@.str.4 = private unnamed_addr constant [21 x i8] c"H1: \09Value 5 = %lld\0A\00", align 1

; Function Attrs: nounwind uwtable
define void @CAT_execution(i32 %userInput) #0 {
  %1 = call i8* @CAT_create_signed_value(i64 5)
  %2 = call i8* @CAT_create_signed_value(i64 8)
  %3 = call i8* @CAT_create_signed_value(i64 8)
  %4 = call i8* @CAT_create_signed_value(i64 8)
  %5 = icmp sgt i32 %userInput, 10
  br i1 %5, label %6, label %7

; <label>:6                                       ; preds = %0
  call void @CAT_binary_add(i8* %1, i8* %2, i8* %3)
  br label %7

; <label>:7                                       ; preds = %6, %0
  %8 = call i64 @CAT_get_signed_value(i8* %1)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i64 %8)
  %10 = call i64 @CAT_get_signed_value(i8* %2)
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i32 0, i32 0), i64 %10)
  %12 = call i64 @CAT_get_signed_value(i8* %3)
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i32 0, i32 0), i64 %12)
  %14 = call i64 @CAT_get_signed_value(i8* %4)
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i32 0, i32 0), i64 %14)
  call void @CAT_binary_add(i8* %1, i8* %2, i8* %3)
  %16 = call i64 @CAT_get_signed_value(i8* %1)
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i64 %16)
  %18 = call i64 @CAT_get_signed_value(i8* %2)
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i32 0, i32 0), i64 %18)
  %20 = call i64 @CAT_get_signed_value(i8* %3)
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i32 0, i32 0), i64 %20)
  %22 = call i64 @CAT_get_signed_value(i8* %4)
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i32 0, i32 0), i64 %22)
  %24 = icmp sgt i32 %userInput, 20
  br i1 %24, label %25, label %27

; <label>:25                                      ; preds = %7
  %26 = call i8* @CAT_create_signed_value(i64 0)
  br label %27

; <label>:27                                      ; preds = %25, %7
  %28 = call i64 @CAT_get_signed_value(i8* %1)
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i64 %28)
  %30 = call i64 @CAT_get_signed_value(i8* %2)
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i32 0, i32 0), i64 %30)
  %32 = call i64 @CAT_get_signed_value(i8* %3)
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i32 0, i32 0), i64 %32)
  %34 = call i64 @CAT_get_signed_value(i8* %4)
  %35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i32 0, i32 0), i64 %34)
  %36 = icmp sgt i32 %userInput, 25
  br i1 %36, label %37, label %40

; <label>:37                                      ; preds = %27
  %38 = call i64 @CAT_get_signed_value(i8* %4)
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.4, i32 0, i32 0), i64 %38)
  br label %40

; <label>:40                                      ; preds = %37, %27
  ret void
}

declare i8* @CAT_create_signed_value(i64) #1

declare void @CAT_binary_add(i8*, i8*, i8*) #1

declare i32 @printf(i8*, ...) #1

declare i64 @CAT_get_signed_value(i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  call void @CAT_execution(i32 %argc)
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
