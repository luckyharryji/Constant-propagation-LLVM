; ModuleID = 'program1.bc'
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.11.0"

@.str = private unnamed_addr constant [21 x i8] c"H1: \09Value 1 = %lld\0A\00", align 1
@.str.1 = private unnamed_addr constant [21 x i8] c"H1: \09Value 2 = %lld\0A\00", align 1
@.str.2 = private unnamed_addr constant [20 x i8] c"H1: \09Result = %lld\0A\00", align 1

; Function Attrs: nounwind ssp uwtable
define void @CAT_execution(i32 %userInput) #0 {
entry:
  %call = tail call i8* @CAT_create_signed_value(i64 5) #3
  %call1 = tail call i64 @CAT_get_signed_value(i8* %call) #3
  %call2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 %call1)
  %cmp35 = icmp sgt i32 %userInput, 0
  br i1 %cmp35, label %for.body.lr.ph, label %for.cond.cleanup

for.body.lr.ph:                                   ; preds = %entry
  %cmp4 = icmp sgt i32 %userInput, 10
  %cmp8 = icmp sgt i32 %userInput, 20
  br i1 %cmp4, label %for.body.us.preheader, label %for.body.preheader

for.body.preheader:                               ; preds = %for.body.lr.ph
  br label %for.body

for.body.us.preheader:                            ; preds = %for.body.lr.ph
  br label %for.body.us

for.body.us:                                      ; preds = %for.body.us.preheader, %if.end10.us
  %i.036.us = phi i32 [ %inc.us, %if.end10.us ], [ 0, %for.body.us.preheader ]
  %call3.us = tail call i8* @CAT_create_signed_value(i64 8) #3
  tail call void @CAT_binary_add(i8* %call3.us, i8* %call3.us, i8* %call3.us) #3
  %call5.us = tail call i64 @CAT_get_signed_value(i8* %call3.us) #3
  %call6.us = tail call i32 (i8*, ...) @printf(i8* nonnull getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i64 0, i64 0), i64 %call5.us)
  %call7.us = tail call i8* @CAT_create_signed_value(i64 0) #3
  tail call void @CAT_binary_add(i8* %call7.us, i8* %call, i8* %call3.us) #3
  tail call void @CAT_binary_add(i8* %call7.us, i8* %call, i8* %call7.us) #3
  tail call void @CAT_binary_add(i8* %call7.us, i8* %call7.us, i8* %call7.us) #3
  br i1 %cmp8, label %if.then9.us, label %if.end10.us

if.then9.us:                                      ; preds = %for.body.us
  tail call void @CAT_binary_add(i8* %call7.us, i8* %call, i8* %call) #3
  br label %if.end10.us

if.end10.us:                                      ; preds = %if.then9.us, %for.body.us
  %call11.us = tail call i64 @CAT_get_signed_value(i8* %call7.us) #3
  %call12.us = tail call i32 (i8*, ...) @printf(i8* nonnull getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i64 0, i64 0), i64 %call11.us)
  %inc.us = add nuw nsw i32 %i.036.us, 1
  %exitcond = icmp eq i32 %inc.us, %userInput
  br i1 %exitcond, label %for.cond.cleanup.loopexit, label %for.body.us

for.cond.cleanup.loopexit:                        ; preds = %if.end10.us
  br label %for.cond.cleanup

for.cond.cleanup.loopexit57:                      ; preds = %for.body
  br label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond.cleanup.loopexit57, %for.cond.cleanup.loopexit, %entry
  ret void

for.body:                                         ; preds = %for.body.preheader, %for.body
  %i.036 = phi i32 [ %inc, %for.body ], [ 0, %for.body.preheader ]
  %call3 = tail call i8* @CAT_create_signed_value(i64 8) #3
  %call5 = tail call i64 @CAT_get_signed_value(i8* %call3) #3
  %call6 = tail call i32 (i8*, ...) @printf(i8* nonnull getelementptr inbounds ([21 x i8], [21 x i8]* @.str.1, i64 0, i64 0), i64 %call5)
  %call7 = tail call i8* @CAT_create_signed_value(i64 0) #3
  tail call void @CAT_binary_add(i8* %call7, i8* %call, i8* %call3) #3
  tail call void @CAT_binary_add(i8* %call7, i8* %call, i8* %call7) #3
  tail call void @CAT_binary_add(i8* %call7, i8* %call7, i8* %call7) #3
  %call11 = tail call i64 @CAT_get_signed_value(i8* %call7) #3
  %call12 = tail call i32 (i8*, ...) @printf(i8* nonnull getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i64 0, i64 0), i64 %call11)
  %inc = add nuw nsw i32 %i.036, 1
  %exitcond54 = icmp eq i32 %inc, %userInput
  br i1 %exitcond54, label %for.cond.cleanup.loopexit57, label %for.body
}

declare i8* @CAT_create_signed_value(i64) #1

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) #2

declare i64 @CAT_get_signed_value(i8*) #1

declare void @CAT_binary_add(i8*, i8*, i8*) #1

; Function Attrs: nounwind ssp uwtable
define i32 @main(i32 %argc, i8** nocapture readnone %argv) #0 {
entry:
  tail call void @CAT_execution(i32 %argc)
  ret i32 0
}

attributes #0 = { nounwind ssp uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="core2" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+ssse3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
