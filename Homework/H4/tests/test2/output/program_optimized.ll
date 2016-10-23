; ModuleID = 'program_optimized.bc'
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
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i32 0, i32 0), i64 8)
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i32 0, i32 0), i64 8)
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i32 0, i32 0), i64 8)
  call void @CAT_binary_add(i8* %1, i8* %2, i8* %3)
  %13 = call i64 @CAT_get_signed_value(i8* %1)
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i64 %13)
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i32 0, i32 0), i64 8)
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i32 0, i32 0), i64 8)
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i32 0, i32 0), i64 8)
  %18 = icmp sgt i32 %userInput, 20
  br i1 %18, label %19, label %21

; <label>:19                                      ; preds = %7
  %20 = call i8* @CAT_create_signed_value(i64 0)
  br label %21

; <label>:21                                      ; preds = %19, %7
  %22 = call i64 @CAT_get_signed_value(i8* %1)
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i64 %22)
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i32 0, i32 0), i64 8)
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i32 0, i32 0), i64 8)
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i32 0, i32 0), i64 8)
  %27 = icmp sgt i32 %userInput, 25
  br i1 %27, label %28, label %30

; <label>:28                                      ; preds = %21
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.4, i32 0, i32 0), i64 8)
  br label %30

; <label>:30                                      ; preds = %28, %21
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
