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

@a = dso_local global i32 0, align 4, !dbg !0
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
define dso_local %struct.X* @_Z4newXv() #0 !dbg !11 {
entry:
  %temp = alloca %struct.X*, align 8
  call void @llvm.dbg.declare(metadata %struct.X** %temp, metadata !27, metadata !DIExpression()), !dbg !28
  %call = call noalias nonnull i8* @_Znwm(i64 16) #5, !dbg !29, !heapallocsite !15
  %0 = bitcast i8* %call to %struct.X*, !dbg !29
  call void @_ZN1XC2Ev(%struct.X* %0) #6, !dbg !30
  store %struct.X* %0, %struct.X** %temp, align 8, !dbg !28
  %1 = load %struct.X*, %struct.X** %temp, align 8, !dbg !31
  ret %struct.X* %1, !dbg !32
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nobuiltin allocsize(0)
declare dso_local nonnull i8* @_Znwm(i64) #2

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1XC2Ev(%struct.X* %this) unnamed_addr #3 comdat align 2 !dbg !33 {
entry:
  %this.addr = alloca %struct.X*, align 8
  store %struct.X* %this, %struct.X** %this.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.X** %this.addr, metadata !35, metadata !DIExpression()), !dbg !36
  %this1 = load %struct.X*, %struct.X** %this.addr, align 8
  %0 = bitcast %struct.X* %this1 to i32 (...)***, !dbg !37
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTV1X, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %0, align 8, !dbg !37
  ret void, !dbg !37
}

; Function Attrs: noinline optnone uwtable
define dso_local %struct.Z* @_Z4newZv() #0 !dbg !38 {
entry:
  %temp = alloca %struct.Z*, align 8
  call void @llvm.dbg.declare(metadata %struct.Z** %temp, metadata !58, metadata !DIExpression()), !dbg !59
  %call = call noalias nonnull i8* @_Znwm(i64 32) #5, !dbg !60, !heapallocsite !42
  %0 = bitcast i8* %call to %struct.Z*, !dbg !60
  call void @_ZN1ZC2Ev(%struct.Z* %0) #6, !dbg !61
  store %struct.Z* %0, %struct.Z** %temp, align 8, !dbg !59
  %1 = load %struct.Z*, %struct.Z** %temp, align 8, !dbg !62
  ret %struct.Z* %1, !dbg !63
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1ZC2Ev(%struct.Z* %this) unnamed_addr #3 comdat align 2 !dbg !64 {
entry:
  %this.addr = alloca %struct.Z*, align 8
  store %struct.Z* %this, %struct.Z** %this.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.Z** %this.addr, metadata !66, metadata !DIExpression()), !dbg !67
  %this1 = load %struct.Z*, %struct.Z** %this.addr, align 8
  %0 = bitcast %struct.Z* %this1 to %struct.Y*, !dbg !68
  call void @_ZN1YC2Ev(%struct.Y* %0) #6, !dbg !68
  %1 = bitcast %struct.Z* %this1 to i32 (...)***, !dbg !68
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTV1Z, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %1, align 8, !dbg !68
  ret void, !dbg !68
}

; Function Attrs: noinline optnone uwtable
define dso_local %struct.Y* @_Z4newYv() #0 !dbg !69 {
entry:
  %temp = alloca %struct.Y*, align 8
  call void @llvm.dbg.declare(metadata %struct.Y** %temp, metadata !73, metadata !DIExpression()), !dbg !74
  %call = call noalias nonnull i8* @_Znwm(i64 24) #5, !dbg !75, !heapallocsite !45
  %0 = bitcast i8* %call to %struct.Y*, !dbg !75
  call void @_ZN1YC2Ev(%struct.Y* %0) #6, !dbg !76
  store %struct.Y* %0, %struct.Y** %temp, align 8, !dbg !74
  %1 = load %struct.Y*, %struct.Y** %temp, align 8, !dbg !77
  ret %struct.Y* %1, !dbg !78
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1YC2Ev(%struct.Y* %this) unnamed_addr #3 comdat align 2 !dbg !79 {
entry:
  %this.addr = alloca %struct.Y*, align 8
  store %struct.Y* %this, %struct.Y** %this.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.Y** %this.addr, metadata !81, metadata !DIExpression()), !dbg !82
  %this1 = load %struct.Y*, %struct.Y** %this.addr, align 8
  %0 = bitcast %struct.Y* %this1 to %struct.X*, !dbg !83
  call void @_ZN1XC2Ev(%struct.X* %0) #6, !dbg !83
  %1 = bitcast %struct.Y* %this1 to i32 (...)***, !dbg !83
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTV1Y, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %1, align 8, !dbg !83
  ret void, !dbg !83
}

; Function Attrs: noinline optnone uwtable
define dso_local void @_Z7convertPP1X(%struct.X** %x) #0 !dbg !84 {
entry:
  %x.addr = alloca %struct.X**, align 8
  store %struct.X** %x, %struct.X*** %x.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.X*** %x.addr, metadata !88, metadata !DIExpression()), !dbg !89
  %call = call noalias nonnull i8* @_Znwm(i64 16) #5, !dbg !90, !heapallocsite !15
  %0 = bitcast i8* %call to %struct.X*, !dbg !90
  call void @_ZN1XC2Ev(%struct.X* %0) #6, !dbg !91
  %1 = load %struct.X**, %struct.X*** %x.addr, align 8, !dbg !92
  store %struct.X* %0, %struct.X** %1, align 8, !dbg !93
  ret void, !dbg !94
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.X* @_Z4passP1X(%struct.X* %x) #3 !dbg !95 {
entry:
  %x.addr = alloca %struct.X*, align 8
  store %struct.X* %x, %struct.X** %x.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.X** %x.addr, metadata !98, metadata !DIExpression()), !dbg !99
  %0 = load %struct.X*, %struct.X** %x.addr, align 8, !dbg !100
  ret %struct.X* %0, !dbg !101
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @_Z4testPPP1XS1_(%struct.X*** %a, %struct.X** %b) #3 !dbg !102 {
entry:
  %a.addr = alloca %struct.X***, align 8
  %b.addr = alloca %struct.X**, align 8
  store %struct.X*** %a, %struct.X**** %a.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.X**** %a.addr, metadata !106, metadata !DIExpression()), !dbg !107
  store %struct.X** %b, %struct.X*** %b.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.X*** %b.addr, metadata !108, metadata !DIExpression()), !dbg !109
  %0 = load %struct.X**, %struct.X*** %b.addr, align 8, !dbg !110
  %1 = load %struct.X***, %struct.X**** %a.addr, align 8, !dbg !111
  store %struct.X** %0, %struct.X*** %1, align 8, !dbg !112
  ret void, !dbg !113
}

; Function Attrs: noinline norecurse optnone uwtable
define dso_local i32 @main() #4 !dbg !114 {
entry:
  %retval = alloca i32, align 4
  %p = alloca %struct.X**, align 8
  %q = alloca %struct.X**, align 8
  %t = alloca %struct.X*, align 8
  %x = alloca %struct.X*, align 8
  %y = alloca %struct.X*, align 8
  %z = alloca %struct.X*, align 8
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata %struct.X*** %p, metadata !115, metadata !DIExpression()), !dbg !116
  call void @llvm.dbg.declare(metadata %struct.X*** %q, metadata !117, metadata !DIExpression()), !dbg !118
  call void @llvm.dbg.declare(metadata %struct.X** %t, metadata !119, metadata !DIExpression()), !dbg !120
  call void @llvm.dbg.declare(metadata %struct.X** %x, metadata !121, metadata !DIExpression()), !dbg !122
  call void @llvm.dbg.declare(metadata %struct.X** %y, metadata !123, metadata !DIExpression()), !dbg !124
  call void @llvm.dbg.declare(metadata %struct.X** %z, metadata !125, metadata !DIExpression()), !dbg !126
  call void @_Z4testPPP1XS1_(%struct.X*** %p, %struct.X** %z), !dbg !127
  call void @_Z4testPPP1XS1_(%struct.X*** %q, %struct.X** %z), !dbg !128
  call void @_Z7convertPP1X(%struct.X** %x), !dbg !129
  %call = call %struct.X* @_Z4newXv(), !dbg !130
  store %struct.X* %call, %struct.X** %y, align 8, !dbg !131
  %0 = load %struct.X*, %struct.X** %x, align 8, !dbg !132
  %call1 = call %struct.X* @_Z4passP1X(%struct.X* %0), !dbg !133
  %1 = load %struct.X**, %struct.X*** %p, align 8, !dbg !134
  store %struct.X* %call1, %struct.X** %1, align 8, !dbg !135
  %call2 = call %struct.Y* @_Z4newYv(), !dbg !136
  %2 = bitcast %struct.Y* %call2 to %struct.X*, !dbg !136
  %3 = load %struct.X*, %struct.X** %x, align 8, !dbg !137
  %foo = getelementptr inbounds %struct.X, %struct.X* %3, i32 0, i32 1, !dbg !138
  store %struct.X* %2, %struct.X** %foo, align 8, !dbg !139
  %call3 = call %struct.Z* @_Z4newZv(), !dbg !140
  %4 = bitcast %struct.Z* %call3 to %struct.X*, !dbg !140
  %5 = load %struct.X*, %struct.X** %y, align 8, !dbg !141
  %foo4 = getelementptr inbounds %struct.X, %struct.X* %5, i32 0, i32 1, !dbg !142
  store %struct.X* %4, %struct.X** %foo4, align 8, !dbg !143
  %6 = load %struct.X*, %struct.X** %z, align 8, !dbg !144
  %foo5 = getelementptr inbounds %struct.X, %struct.X* %6, i32 0, i32 1, !dbg !145
  %7 = load %struct.X*, %struct.X** %foo5, align 8, !dbg !145
  store %struct.X* %7, %struct.X** %t, align 8, !dbg !146
  %8 = load %struct.X*, %struct.X** %t, align 8, !dbg !147
  %9 = bitcast %struct.X* %8 to void (%struct.X*)***, !dbg !148
  %vtable = load void (%struct.X*)**, void (%struct.X*)*** %9, align 8, !dbg !148
  %vfn = getelementptr inbounds void (%struct.X*)*, void (%struct.X*)** %vtable, i64 0, !dbg !148
  %10 = load void (%struct.X*)*, void (%struct.X*)** %vfn, align 8, !dbg !148
  call void %10(%struct.X* %8), !dbg !148
  %11 = load i32, i32* @a, align 4, !dbg !149
  ret i32 %11, !dbg !150
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1X1fEv(%struct.X* %this) unnamed_addr #3 comdat align 2 !dbg !151 {
entry:
  %this.addr = alloca %struct.X*, align 8
  store %struct.X* %this, %struct.X** %this.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.X** %this.addr, metadata !152, metadata !DIExpression()), !dbg !153
  %this1 = load %struct.X*, %struct.X** %this.addr, align 8
  store i32 9, i32* @a, align 4, !dbg !154
  ret void, !dbg !155
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1Z1fEv(%struct.Z* %this) unnamed_addr #3 comdat align 2 !dbg !156 {
entry:
  %this.addr = alloca %struct.Z*, align 8
  store %struct.Z* %this, %struct.Z** %this.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.Z** %this.addr, metadata !157, metadata !DIExpression()), !dbg !158
  %this1 = load %struct.Z*, %struct.Z** %this.addr, align 8
  store i32 4, i32* @a, align 4, !dbg !159
  ret void, !dbg !160
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1Y1fEv(%struct.Y* %this) unnamed_addr #3 comdat align 2 !dbg !161 {
entry:
  %this.addr = alloca %struct.Y*, align 8
  store %struct.Y* %this, %struct.Y** %this.addr, align 8
  call void @llvm.dbg.declare(metadata %struct.Y** %this.addr, metadata !162, metadata !DIExpression()), !dbg !163
  %this1 = load %struct.Y*, %struct.Y** %this.addr, align 8
  store i32 7, i32* @a, align 4, !dbg !164
  ret void, !dbg !165
}

attributes #0 = { noinline optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }
attributes #2 = { nobuiltin allocsize(0) "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { builtin allocsize(0) }
attributes #6 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!7, !8, !9}
!llvm.ident = !{!10}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "a", scope: !2, file: !3, line: 1, type: !6, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "clang version 12.0.0 (https://github.com/llvm/llvm-project.git b5e14d78f16960ebb94f0a22e2a8c10f421f8757)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, globals: !5, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "test.cpp", directory: "/home/rksharma/pass")
!4 = !{}
!5 = !{!0}
!6 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!7 = !{i32 7, !"Dwarf Version", i32 4}
!8 = !{i32 2, !"Debug Info Version", i32 3}
!9 = !{i32 1, !"wchar_size", i32 4}
!10 = !{!"clang version 12.0.0 (https://github.com/llvm/llvm-project.git b5e14d78f16960ebb94f0a22e2a8c10f421f8757)"}
!11 = distinct !DISubprogram(name: "newX", linkageName: "_Z4newXv", scope: !3, file: !3, line: 23, type: !12, scopeLine: 23, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!12 = !DISubroutineType(types: !13)
!13 = !{!14}
!14 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !15, size: 64)
!15 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "X", file: !3, line: 2, size: 128, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !16, vtableHolder: !15, identifier: "_ZTS1X")
!16 = !{!17, !22, !23}
!17 = !DIDerivedType(tag: DW_TAG_member, name: "_vptr$X", scope: !3, file: !3, baseType: !18, size: 64, flags: DIFlagArtificial)
!18 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 64)
!19 = !DIDerivedType(tag: DW_TAG_pointer_type, name: "__vtbl_ptr_type", baseType: !20, size: 64)
!20 = !DISubroutineType(types: !21)
!21 = !{!6}
!22 = !DIDerivedType(tag: DW_TAG_member, name: "foo", scope: !15, file: !3, line: 3, baseType: !14, size: 64, offset: 64)
!23 = !DISubprogram(name: "f", linkageName: "_ZN1X1fEv", scope: !15, file: !3, line: 4, type: !24, scopeLine: 4, containingType: !15, virtualIndex: 0, flags: DIFlagPrototyped, spFlags: DISPFlagVirtual)
!24 = !DISubroutineType(types: !25)
!25 = !{null, !26}
!26 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !15, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!27 = !DILocalVariable(name: "temp", scope: !11, file: !3, line: 24, type: !14)
!28 = !DILocation(line: 24, column: 9, scope: !11)
!29 = !DILocation(line: 24, column: 16, scope: !11)
!30 = !DILocation(line: 24, column: 20, scope: !11)
!31 = !DILocation(line: 25, column: 12, scope: !11)
!32 = !DILocation(line: 25, column: 5, scope: !11)
!33 = distinct !DISubprogram(name: "X", linkageName: "_ZN1XC2Ev", scope: !15, file: !3, line: 2, type: !24, scopeLine: 2, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !34, retainedNodes: !4)
!34 = !DISubprogram(name: "X", scope: !15, type: !24, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: 0)
!35 = !DILocalVariable(name: "this", arg: 1, scope: !33, type: !14, flags: DIFlagArtificial | DIFlagObjectPointer)
!36 = !DILocation(line: 0, scope: !33)
!37 = !DILocation(line: 2, column: 8, scope: !33)
!38 = distinct !DISubprogram(name: "newZ", linkageName: "_Z4newZv", scope: !3, file: !3, line: 28, type: !39, scopeLine: 28, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!39 = !DISubroutineType(types: !40)
!40 = !{!41}
!41 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !42, size: 64)
!42 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "Z", file: !3, line: 16, size: 256, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !43, vtableHolder: !15, identifier: "_ZTS1Z")
!43 = !{!44, !53, !54}
!44 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !42, baseType: !45, extraData: i32 0)
!45 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "Y", file: !3, line: 9, size: 192, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !46, vtableHolder: !15, identifier: "_ZTS1Y")
!46 = !{!47, !48, !49}
!47 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !45, baseType: !15, extraData: i32 0)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "foo", scope: !45, file: !3, line: 10, baseType: !14, size: 64, offset: 128)
!49 = !DISubprogram(name: "f", linkageName: "_ZN1Y1fEv", scope: !45, file: !3, line: 11, type: !50, scopeLine: 11, containingType: !45, virtualIndex: 0, flags: DIFlagPrototyped, spFlags: DISPFlagVirtual)
!50 = !DISubroutineType(types: !51)
!51 = !{null, !52}
!52 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !45, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!53 = !DIDerivedType(tag: DW_TAG_member, name: "foo", scope: !42, file: !3, line: 17, baseType: !14, size: 64, offset: 192)
!54 = !DISubprogram(name: "f", linkageName: "_ZN1Z1fEv", scope: !42, file: !3, line: 18, type: !55, scopeLine: 18, containingType: !42, virtualIndex: 0, flags: DIFlagPrototyped, spFlags: DISPFlagVirtual)
!55 = !DISubroutineType(types: !56)
!56 = !{null, !57}
!57 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !42, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!58 = !DILocalVariable(name: "temp", scope: !38, file: !3, line: 29, type: !41)
!59 = !DILocation(line: 29, column: 9, scope: !38)
!60 = !DILocation(line: 29, column: 16, scope: !38)
!61 = !DILocation(line: 29, column: 20, scope: !38)
!62 = !DILocation(line: 30, column: 12, scope: !38)
!63 = !DILocation(line: 30, column: 5, scope: !38)
!64 = distinct !DISubprogram(name: "Z", linkageName: "_ZN1ZC2Ev", scope: !42, file: !3, line: 16, type: !55, scopeLine: 16, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !65, retainedNodes: !4)
!65 = !DISubprogram(name: "Z", scope: !42, type: !55, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: 0)
!66 = !DILocalVariable(name: "this", arg: 1, scope: !64, type: !41, flags: DIFlagArtificial | DIFlagObjectPointer)
!67 = !DILocation(line: 0, scope: !64)
!68 = !DILocation(line: 16, column: 8, scope: !64)
!69 = distinct !DISubprogram(name: "newY", linkageName: "_Z4newYv", scope: !3, file: !3, line: 33, type: !70, scopeLine: 33, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!70 = !DISubroutineType(types: !71)
!71 = !{!72}
!72 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !45, size: 64)
!73 = !DILocalVariable(name: "temp", scope: !69, file: !3, line: 34, type: !72)
!74 = !DILocation(line: 34, column: 9, scope: !69)
!75 = !DILocation(line: 34, column: 16, scope: !69)
!76 = !DILocation(line: 34, column: 20, scope: !69)
!77 = !DILocation(line: 35, column: 12, scope: !69)
!78 = !DILocation(line: 35, column: 5, scope: !69)
!79 = distinct !DISubprogram(name: "Y", linkageName: "_ZN1YC2Ev", scope: !45, file: !3, line: 9, type: !50, scopeLine: 9, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !80, retainedNodes: !4)
!80 = !DISubprogram(name: "Y", scope: !45, type: !50, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: 0)
!81 = !DILocalVariable(name: "this", arg: 1, scope: !79, type: !72, flags: DIFlagArtificial | DIFlagObjectPointer)
!82 = !DILocation(line: 0, scope: !79)
!83 = !DILocation(line: 9, column: 8, scope: !79)
!84 = distinct !DISubprogram(name: "convert", linkageName: "_Z7convertPP1X", scope: !3, file: !3, line: 38, type: !85, scopeLine: 38, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!85 = !DISubroutineType(types: !86)
!86 = !{null, !87}
!87 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!88 = !DILocalVariable(name: "x", arg: 1, scope: !84, file: !3, line: 38, type: !87)
!89 = !DILocation(line: 38, column: 18, scope: !84)
!90 = !DILocation(line: 39, column: 10, scope: !84)
!91 = !DILocation(line: 39, column: 14, scope: !84)
!92 = !DILocation(line: 39, column: 6, scope: !84)
!93 = !DILocation(line: 39, column: 8, scope: !84)
!94 = !DILocation(line: 40, column: 1, scope: !84)
!95 = distinct !DISubprogram(name: "pass", linkageName: "_Z4passP1X", scope: !3, file: !3, line: 41, type: !96, scopeLine: 41, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!96 = !DISubroutineType(types: !97)
!97 = !{!14, !14}
!98 = !DILocalVariable(name: "x", arg: 1, scope: !95, file: !3, line: 41, type: !14)
!99 = !DILocation(line: 41, column: 12, scope: !95)
!100 = !DILocation(line: 42, column: 12, scope: !95)
!101 = !DILocation(line: 42, column: 5, scope: !95)
!102 = distinct !DISubprogram(name: "test", linkageName: "_Z4testPPP1XS1_", scope: !3, file: !3, line: 44, type: !103, scopeLine: 44, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!103 = !DISubroutineType(types: !104)
!104 = !{null, !105, !87}
!105 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !87, size: 64)
!106 = !DILocalVariable(name: "a", arg: 1, scope: !102, file: !3, line: 44, type: !105)
!107 = !DILocation(line: 44, column: 16, scope: !102)
!108 = !DILocalVariable(name: "b", arg: 2, scope: !102, file: !3, line: 44, type: !87)
!109 = !DILocation(line: 44, column: 23, scope: !102)
!110 = !DILocation(line: 45, column: 10, scope: !102)
!111 = !DILocation(line: 45, column: 6, scope: !102)
!112 = !DILocation(line: 45, column: 8, scope: !102)
!113 = !DILocation(line: 46, column: 1, scope: !102)
!114 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 47, type: !20, scopeLine: 47, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!115 = !DILocalVariable(name: "p", scope: !114, file: !3, line: 48, type: !87)
!116 = !DILocation(line: 48, column: 9, scope: !114)
!117 = !DILocalVariable(name: "q", scope: !114, file: !3, line: 48, type: !87)
!118 = !DILocation(line: 48, column: 14, scope: !114)
!119 = !DILocalVariable(name: "t", scope: !114, file: !3, line: 49, type: !14)
!120 = !DILocation(line: 49, column: 8, scope: !114)
!121 = !DILocalVariable(name: "x", scope: !114, file: !3, line: 49, type: !14)
!122 = !DILocation(line: 49, column: 12, scope: !114)
!123 = !DILocalVariable(name: "y", scope: !114, file: !3, line: 49, type: !14)
!124 = !DILocation(line: 49, column: 16, scope: !114)
!125 = !DILocalVariable(name: "z", scope: !114, file: !3, line: 49, type: !14)
!126 = !DILocation(line: 49, column: 20, scope: !114)
!127 = !DILocation(line: 50, column: 5, scope: !114)
!128 = !DILocation(line: 51, column: 5, scope: !114)
!129 = !DILocation(line: 52, column: 5, scope: !114)
!130 = !DILocation(line: 53, column: 9, scope: !114)
!131 = !DILocation(line: 53, column: 7, scope: !114)
!132 = !DILocation(line: 54, column: 15, scope: !114)
!133 = !DILocation(line: 54, column: 10, scope: !114)
!134 = !DILocation(line: 54, column: 6, scope: !114)
!135 = !DILocation(line: 54, column: 8, scope: !114)
!136 = !DILocation(line: 55, column: 14, scope: !114)
!137 = !DILocation(line: 55, column: 5, scope: !114)
!138 = !DILocation(line: 55, column: 8, scope: !114)
!139 = !DILocation(line: 55, column: 12, scope: !114)
!140 = !DILocation(line: 56, column: 14, scope: !114)
!141 = !DILocation(line: 56, column: 5, scope: !114)
!142 = !DILocation(line: 56, column: 8, scope: !114)
!143 = !DILocation(line: 56, column: 12, scope: !114)
!144 = !DILocation(line: 57, column: 9, scope: !114)
!145 = !DILocation(line: 57, column: 12, scope: !114)
!146 = !DILocation(line: 57, column: 7, scope: !114)
!147 = !DILocation(line: 58, column: 5, scope: !114)
!148 = !DILocation(line: 58, column: 8, scope: !114)
!149 = !DILocation(line: 59, column: 12, scope: !114)
!150 = !DILocation(line: 59, column: 5, scope: !114)
!151 = distinct !DISubprogram(name: "f", linkageName: "_ZN1X1fEv", scope: !15, file: !3, line: 4, type: !24, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !23, retainedNodes: !4)
!152 = !DILocalVariable(name: "this", arg: 1, scope: !151, type: !14, flags: DIFlagArtificial | DIFlagObjectPointer)
!153 = !DILocation(line: 0, scope: !151)
!154 = !DILocation(line: 5, column: 10, scope: !151)
!155 = !DILocation(line: 6, column: 4, scope: !151)
!156 = distinct !DISubprogram(name: "f", linkageName: "_ZN1Z1fEv", scope: !42, file: !3, line: 18, type: !55, scopeLine: 18, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !54, retainedNodes: !4)
!157 = !DILocalVariable(name: "this", arg: 1, scope: !156, type: !41, flags: DIFlagArtificial | DIFlagObjectPointer)
!158 = !DILocation(line: 0, scope: !156)
!159 = !DILocation(line: 19, column: 11, scope: !156)
!160 = !DILocation(line: 20, column: 5, scope: !156)
!161 = distinct !DISubprogram(name: "f", linkageName: "_ZN1Y1fEv", scope: !45, file: !3, line: 11, type: !50, scopeLine: 11, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !49, retainedNodes: !4)
!162 = !DILocalVariable(name: "this", arg: 1, scope: !161, type: !72, flags: DIFlagArtificial | DIFlagObjectPointer)
!163 = !DILocation(line: 0, scope: !161)
!164 = !DILocation(line: 12, column: 11, scope: !161)
!165 = !DILocation(line: 13, column: 5, scope: !161)
