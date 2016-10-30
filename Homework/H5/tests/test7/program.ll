; ModuleID = 'program.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct.internal_data_t = type { [20 x i8], i64, [20 x i8] }

@.str = private unnamed_addr constant [24 x i8] c"H5: \09Value of d1 = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [21 x i8] c"p6pbbUlpLo0BL1bM2k8K\00", align 1
@stderr = external global %struct._IO_FILE*, align 8
@.str.1 = private unnamed_addr constant [31 x i8] c"libCAT: ERROR = input is NULL\0A\00", align 1
@.str.2 = private unnamed_addr constant [41 x i8] c"libCAT: ERROR = data has been corrupted\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = icmp sgt i32 %argc, 10
  br i1 %1, label %2, label %4

; <label>:2                                       ; preds = %0
  %3 = call i8* @CAT_create_signed_value(i64 8)
  br label %6

; <label>:4                                       ; preds = %0
  %5 = call i8* @CAT_create_signed_value(i64 8)
  br label %6

; <label>:6                                       ; preds = %4, %2
  %d1.0 = phi i8* [ %3, %2 ], [ %5, %4 ]
  %7 = call i64 @CAT_get_signed_value(i8* %d1.0)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str, i32 0, i32 0), i64 %7)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: nounwind uwtable
define noalias i8* @CAT_create_signed_value(i64 %value) #2 {
  %1 = tail call noalias i8* @malloc(i64 56) #5
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i64 0, i64 0), i64 20, i32 1, i1 false)
  %2 = getelementptr inbounds i8, i8* %1, i64 32
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i64 0, i64 0), i64 20, i32 1, i1 false)
  %3 = getelementptr inbounds i8, i8* %1, i64 24
  %4 = bitcast i8* %3 to i64*
  store i64 %value, i64* %4, align 8, !tbaa !1
  ret i8* %1
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #3

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #4

; Function Attrs: nounwind uwtable
define i64 @CAT_get_signed_value(i8* readonly %v) #2 {
  %1 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %v)
  %2 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %1, i64 0, i32 1
  %3 = load i64, i64* %2, align 8, !tbaa !1
  ret i64 %3
}

; Function Attrs: nounwind uwtable
define internal fastcc %struct.internal_data_t* @internal_check_data(i8* readonly %v) unnamed_addr #2 {
  %1 = icmp eq i8* %v, null
  br i1 %1, label %2, label %5

; <label>:2                                       ; preds = %0
  %3 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !tbaa !6
  %4 = tail call i64 @fwrite(i8* nonnull getelementptr inbounds ([31 x i8], [31 x i8]* @.str.1, i64 0, i64 0), i64 30, i64 1, %struct._IO_FILE* %3) #8
  tail call void @abort() #9
  unreachable

; <label>:5                                       ; preds = %0
  %6 = bitcast i8* %v to %struct.internal_data_t*
  %7 = tail call i32 @strncmp(i8* nonnull %v, i8* nonnull getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i64 0, i64 0), i64 20) #10
  %8 = icmp eq i32 %7, 0
  br i1 %8, label %9, label %13

; <label>:9                                       ; preds = %5
  %10 = getelementptr inbounds i8, i8* %v, i64 32
  %11 = tail call i32 @strncmp(i8* %10, i8* nonnull getelementptr inbounds ([21 x i8], [21 x i8]* @.str.3, i64 0, i64 0), i64 20) #10
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %16, label %13

; <label>:13                                      ; preds = %9, %5
  %14 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !tbaa !6
  %15 = tail call i64 @fwrite(i8* nonnull getelementptr inbounds ([41 x i8], [41 x i8]* @.str.2, i64 0, i64 0), i64 40, i64 1, %struct._IO_FILE* %14) #8
  tail call void @abort() #9
  unreachable

; <label>:16                                      ; preds = %9
  ret %struct.internal_data_t* %6
}

; Function Attrs: nounwind
declare i64 @fwrite(i8* nocapture, i64, i64, %struct._IO_FILE* nocapture) #5

; Function Attrs: noreturn nounwind
declare void @abort() #6

; Function Attrs: nounwind readonly
declare i32 @strncmp(i8* nocapture, i8* nocapture, i64) #7

; Function Attrs: nounwind uwtable
define void @CAT_binary_sub(i8* %result, i8* readonly %v1, i8* readonly %v2) #2 {
  %1 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %v1)
  %2 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %v2)
  %3 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %result)
  %4 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %1, i64 0, i32 1
  %5 = load i64, i64* %4, align 8, !tbaa !1
  %6 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %2, i64 0, i32 1
  %7 = load i64, i64* %6, align 8, !tbaa !1
  %8 = sub nsw i64 %5, %7
  %9 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %3, i64 0, i32 1
  store i64 %8, i64* %9, align 8, !tbaa !1
  ret void
}

; Function Attrs: nounwind uwtable
define void @CAT_binary_add(i8* %result, i8* readonly %v1, i8* readonly %v2) #2 {
  %1 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %v1)
  %2 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %v2)
  %3 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %result)
  %4 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %1, i64 0, i32 1
  %5 = load i64, i64* %4, align 8, !tbaa !1
  %6 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %2, i64 0, i32 1
  %7 = load i64, i64* %6, align 8, !tbaa !1
  %8 = add nsw i64 %7, %5
  %9 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %3, i64 0, i32 1
  store i64 %8, i64* %9, align 8, !tbaa !1
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind }
attributes #5 = { nounwind }
attributes #6 = { noreturn nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { nounwind readonly "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { cold }
attributes #9 = { noreturn nounwind }
attributes #10 = { nounwind readonly }

!llvm.ident = !{!0, !0}

!0 = !{!"clang version 3.8.1 (tags/RELEASE_381/final)"}
!1 = !{!2, !5, i64 24}
!2 = !{!"", !3, i64 0, !5, i64 24, !3, i64 32}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
!5 = !{!"long", !3, i64 0}
!6 = !{!7, !7, i64 0}
!7 = !{!"any pointer", !3, i64 0}
