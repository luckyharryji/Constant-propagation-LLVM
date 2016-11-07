; ModuleID = 'program.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1

; Function Attrs: nounwind uwtable
define i64 @generic_C_function(i8* %y) #0 {
  %1 = call noalias i8* @malloc(i64 8) #3
  %2 = bitcast i8* %1 to i8**
  %3 = call i8* @CAT_create_signed_value(i64 8)
  store i8* %3, i8** %2, align 8
  %4 = call i64 @CAT_get_signed_value(i8* %y)
  %5 = icmp sgt i64 %4, 10
  br i1 %5, label %6, label %11

; <label>:6                                       ; preds = %0
  %7 = call i64 @CAT_get_signed_value(i8* %3)
  %8 = bitcast i8** %2 to i8*
  call void @free(i8* %8) #3
  %9 = mul nsw i64 %7, 51
  %10 = sdiv i64 %9, 2
  br label %13

; <label>:11                                      ; preds = %0
  %12 = bitcast i8** %2 to i8*
  call void @free(i8* %12) #3
  br label %13

; <label>:13                                      ; preds = %11, %6
  %.0 = phi i64 [ %10, %6 ], [ 0, %11 ]
  ret i64 %.0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

declare i8* @CAT_create_signed_value(i64) #2

declare i64 @CAT_get_signed_value(i8*) #2

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = call i8* @CAT_create_signed_value(i64 12)
  %2 = call i64 @generic_C_function(i8* %1)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0), i64 %2)
  ret i32 0
}

declare i32 @printf(i8*, ...) #2

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
