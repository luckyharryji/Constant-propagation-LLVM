; ModuleID = 'program1.bc'
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.11.0"

; Function Attrs: nounwind ssp uwtable
define i32 @CAT_execution1() #0 {
entry:
  %d1 = alloca i8*, align 8
  %d2 = alloca i8*, align 8
  %d3 = alloca i8*, align 8
  %call = call i8* @CAT_create_signed_value(i64 5)
  store i8* %call, i8** %d1, align 8
  %call1 = call i8* @CAT_create_signed_value(i64 8)
  store i8* %call1, i8** %d2, align 8
  %call2 = call i8* @CAT_create_signed_value(i64 0)
  store i8* %call2, i8** %d3, align 8
  %0 = load i8*, i8** %d3, align 8
  %1 = load i8*, i8** %d1, align 8
  %2 = load i8*, i8** %d2, align 8
  call void @CAT_binary_add(i8* %0, i8* %1, i8* %2)
  %3 = load i8*, i8** %d3, align 8
  %call3 = call i64 @CAT_get_signed_value(i8* %3)
  %conv = trunc i64 %call3 to i32
  ret i32 %conv
}

declare i8* @CAT_create_signed_value(i64) #1

declare void @CAT_binary_add(i8*, i8*, i8*) #1

declare i64 @CAT_get_signed_value(i8*) #1

; Function Attrs: nounwind ssp uwtable
define i32 @CAT_execution2() #0 {
entry:
  %d = alloca i8*, align 8
  %call = call i8* @CAT_create_signed_value(i64 5)
  store i8* %call, i8** %d, align 8
  %0 = load i8*, i8** %d, align 8
  %call1 = call i64 @CAT_get_signed_value(i8* %0)
  %conv = trunc i64 %call1 to i32
  ret i32 %conv
}

; Function Attrs: nounwind ssp uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %call = call i32 @CAT_execution1()
  %call1 = call i32 @CAT_execution2()
  %add = add nsw i32 %call, %call1
  ret i32 %add
}

attributes #0 = { nounwind ssp uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
