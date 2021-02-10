; ModuleID = 'test.cpp'
source_filename = "test.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.X = type { i32 (...)**, %struct.X* }
%struct.Z = type { %struct.Y, %struct.X* }
%struct.Y = type { %struct.X, %struct.X* }

$_ZN1XC2Ev = comdat any

$_ZN1ZC2Ev = comdat any

$_ZN1YC2Ev = comdat any

$_ZN1X1fEv = comdat any

$_ZN1Z1fEv = comdat any

$_ZN1Y1fEv = comdat any

$_ZTV1X = comdat any

$_ZTS1X = comdat any

$_ZTI1X = comdat any

$_ZTV1Z = comdat any

$_ZTS1Z = comdat any

$_ZTS1Y = comdat any

$_ZTI1Y = comdat any

$_ZTI1Z = comdat any

$_ZTV1Y = comdat any

@a = dso_local global i32 0, align 4
@_ZTV1X = linkonce_odr dso_local unnamed_addr constant { [3 x i8*] } { [3 x i8*] [i8* null, i8* bitcast ({ i8*, i8* }* @_ZTI1X to i8*), i8* bitcast (void (%struct.X*)* @_ZN1X1fEv to i8*)] }, comdat, align 8
@_ZTVN10__cxxabiv117__class_type_infoE = external dso_local global i8*
@_ZTS1X = linkonce_odr dso_local constant [3 x i8] c"1X\00", comdat, align 1
@_ZTI1X = linkonce_odr dso_local constant { i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv117__class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @_ZTS1X, i32 0, i32 0) }, comdat, align 8
@_ZTV1Z = linkonce_odr dso_local unnamed_addr constant { [3 x i8*] } { [3 x i8*] [i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTI1Z to i8*), i8* bitcast (void (%struct.Z*)* @_ZN1Z1fEv to i8*)] }, comdat, align 8
@_ZTVN10__cxxabiv120__si_class_type_infoE = external dso_local global i8*
@_ZTS1Z = linkonce_odr dso_local constant [3 x i8] c"1Z\00", comdat, align 1
@_ZTS1Y = linkonce_odr dso_local constant [3 x i8] c"1Y\00", comdat, align 1
@_ZTI1Y = linkonce_odr dso_local constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @_ZTS1Y, i32 0, i32 0), i8* bitcast ({ i8*, i8* }* @_ZTI1X to i8*) }, comdat, align 8
@_ZTI1Z = linkonce_odr dso_local constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([3 x i8], [3 x i8]* @_ZTS1Z, i32 0, i32 0), i8* bitcast ({ i8*, i8*, i8* }* @_ZTI1Y to i8*) }, comdat, align 8
@_ZTV1Y = linkonce_odr dso_local unnamed_addr constant { [3 x i8*] } { [3 x i8*] [i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTI1Y to i8*), i8* bitcast (void (%struct.Y*)* @_ZN1Y1fEv to i8*)] }, comdat, align 8

; Function Attrs: noinline optnone uwtable
define dso_local %struct.X* @_Z4newXv() #0 {
entry:
  %temp = alloca %struct.X*, align 8
  %call = call noalias nonnull i8* @_Znwm(i64 16) #4
  %0 = bitcast i8* %call to %struct.X*
  call void @_ZN1XC2Ev(%struct.X* %0) #5
  store %struct.X* %0, %struct.X** %temp, align 8
  %1 = load %struct.X*, %struct.X** %temp, align 8
  ret %struct.X* %1
}

; Function Attrs: nobuiltin allocsize(0)
declare dso_local nonnull i8* @_Znwm(i64) #1

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1XC2Ev(%struct.X* %this) unnamed_addr #2 comdat align 2 {
entry:
  %this.addr = alloca %struct.X*, align 8
  store %struct.X* %this, %struct.X** %this.addr, align 8
  %this1 = load %struct.X*, %struct.X** %this.addr, align 8
  %0 = bitcast %struct.X* %this1 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTV1X, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %0, align 8
  ret void
}

; Function Attrs: noinline optnone uwtable
define dso_local %struct.Z* @_Z4newZv() #0 {
entry:
  %temp = alloca %struct.Z*, align 8
  %call = call noalias nonnull i8* @_Znwm(i64 32) #4
  %0 = bitcast i8* %call to %struct.Z*
  call void @_ZN1ZC2Ev(%struct.Z* %0) #5
  store %struct.Z* %0, %struct.Z** %temp, align 8
  %1 = load %struct.Z*, %struct.Z** %temp, align 8
  ret %struct.Z* %1
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1ZC2Ev(%struct.Z* %this) unnamed_addr #2 comdat align 2 {
entry:
  %this.addr = alloca %struct.Z*, align 8
  store %struct.Z* %this, %struct.Z** %this.addr, align 8
  %this1 = load %struct.Z*, %struct.Z** %this.addr, align 8
  %0 = bitcast %struct.Z* %this1 to %struct.Y*
  call void @_ZN1YC2Ev(%struct.Y* %0) #5
  %1 = bitcast %struct.Z* %this1 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTV1Z, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %1, align 8
  ret void
}

; Function Attrs: noinline optnone uwtable
define dso_local %struct.Y* @_Z4newYv() #0 {
entry:
  %temp = alloca %struct.Y*, align 8
  %call = call noalias nonnull i8* @_Znwm(i64 24) #4
  %0 = bitcast i8* %call to %struct.Y*
  call void @_ZN1YC2Ev(%struct.Y* %0) #5
  store %struct.Y* %0, %struct.Y** %temp, align 8
  %1 = load %struct.Y*, %struct.Y** %temp, align 8
  ret %struct.Y* %1
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1YC2Ev(%struct.Y* %this) unnamed_addr #2 comdat align 2 {
entry:
  %this.addr = alloca %struct.Y*, align 8
  store %struct.Y* %this, %struct.Y** %this.addr, align 8
  %this1 = load %struct.Y*, %struct.Y** %this.addr, align 8
  %0 = bitcast %struct.Y* %this1 to %struct.X*
  call void @_ZN1XC2Ev(%struct.X* %0) #5
  %1 = bitcast %struct.Y* %this1 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTV1Y, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %1, align 8
  ret void
}

; Function Attrs: noinline optnone uwtable
define dso_local void @_Z4huluPP1X(%struct.X** %x) #0 {
entry:
  %x.addr = alloca %struct.X**, align 8
  store %struct.X** %x, %struct.X*** %x.addr, align 8
  %call = call noalias nonnull i8* @_Znwm(i64 16) #4
  %0 = bitcast i8* %call to %struct.X*
  call void @_ZN1XC2Ev(%struct.X* %0) #5
  %1 = load %struct.X**, %struct.X*** %x.addr, align 8
  store %struct.X* %0, %struct.X** %1, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.X* @_Z4passP1X(%struct.X* %x) #2 {
entry:
  %x.addr = alloca %struct.X*, align 8
  store %struct.X* %x, %struct.X** %x.addr, align 8
  %0 = load %struct.X*, %struct.X** %x.addr, align 8
  ret %struct.X* %0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @_Z4testPPP1XS1_(%struct.X*** %a, %struct.X** %b) #2 {
entry:
  %a.addr = alloca %struct.X***, align 8
  %b.addr = alloca %struct.X**, align 8
  store %struct.X*** %a, %struct.X**** %a.addr, align 8
  store %struct.X** %b, %struct.X*** %b.addr, align 8
  %0 = load %struct.X**, %struct.X*** %b.addr, align 8
  %1 = load %struct.X***, %struct.X**** %a.addr, align 8
  store %struct.X** %0, %struct.X*** %1, align 8
  ret void
}

; Function Attrs: noinline norecurse optnone uwtable
define dso_local i32 @main() #3 {
entry:
  %retval = alloca i32, align 4
  %p = alloca %struct.X**, align 8
  %q = alloca %struct.X**, align 8
  %t = alloca %struct.X*, align 8
  %x = alloca %struct.X*, align 8
  %y = alloca %struct.X*, align 8
  %z = alloca %struct.X*, align 8
  store i32 0, i32* %retval, align 4
  call void @_Z4testPPP1XS1_(%struct.X*** %p, %struct.X** %z)
  call void @_Z4testPPP1XS1_(%struct.X*** %q, %struct.X** %z)
  call void @_Z4huluPP1X(%struct.X** %x)
  %call = call %struct.X* @_Z4newXv()
  store %struct.X* %call, %struct.X** %y, align 8
  %0 = load %struct.X*, %struct.X** %x, align 8
  %call1 = call %struct.X* @_Z4passP1X(%struct.X* %0)
  %1 = load %struct.X**, %struct.X*** %p, align 8
  store %struct.X* %call1, %struct.X** %1, align 8
  %call2 = call %struct.Y* @_Z4newYv()
  %2 = bitcast %struct.Y* %call2 to %struct.X*
  %3 = load %struct.X*, %struct.X** %x, align 8
  %foo = getelementptr inbounds %struct.X, %struct.X* %3, i32 0, i32 1
  store %struct.X* %2, %struct.X** %foo, align 8
  %call3 = call %struct.Z* @_Z4newZv()
  %4 = bitcast %struct.Z* %call3 to %struct.X*
  %5 = load %struct.X*, %struct.X** %y, align 8
  %foo4 = getelementptr inbounds %struct.X, %struct.X* %5, i32 0, i32 1
  store %struct.X* %4, %struct.X** %foo4, align 8
  %6 = load %struct.X*, %struct.X** %z, align 8
  %foo5 = getelementptr inbounds %struct.X, %struct.X* %6, i32 0, i32 1
  %7 = load %struct.X*, %struct.X** %foo5, align 8
  store %struct.X* %7, %struct.X** %t, align 8
  %8 = load %struct.X*, %struct.X** %t, align 8
  %9 = bitcast %struct.X* %8 to void (%struct.X*)***
  %vtable = load void (%struct.X*)**, void (%struct.X*)*** %9, align 8
  %vfn = getelementptr inbounds void (%struct.X*)*, void (%struct.X*)** %vtable, i64 0
  %10 = load void (%struct.X*)*, void (%struct.X*)** %vfn, align 8
  call void %10(%struct.X* %8)
  %11 = load i32, i32* @a, align 4
  ret i32 %11
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1X1fEv(%struct.X* %this) unnamed_addr #2 comdat align 2 {
entry:
  %this.addr = alloca %struct.X*, align 8
  store %struct.X* %this, %struct.X** %this.addr, align 8
  %this1 = load %struct.X*, %struct.X** %this.addr, align 8
  store i32 9, i32* @a, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1Z1fEv(%struct.Z* %this) unnamed_addr #2 comdat align 2 {
entry:
  %this.addr = alloca %struct.Z*, align 8
  store %struct.Z* %this, %struct.Z** %this.addr, align 8
  %this1 = load %struct.Z*, %struct.Z** %this.addr, align 8
  store i32 4, i32* @a, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1Y1fEv(%struct.Y* %this) unnamed_addr #2 comdat align 2 {
entry:
  %this.addr = alloca %struct.Y*, align 8
  store %struct.Y* %this, %struct.Y** %this.addr, align 8
  %this1 = load %struct.Y*, %struct.Y** %this.addr, align 8
  store i32 7, i32* @a, align 4
  ret void
}

attributes #0 = { noinline optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin allocsize(0) "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { builtin allocsize(0) }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b5e14d78f16960ebb94f0a22e2a8c10f421f8757)"}
