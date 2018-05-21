## Arch
1. Ignition
js解释器(改进脚本执行,内存开销,提升网页加载速度)
2. full codegen
v8 baseline compiler
3. crankshaft
v8 classic optimizing compiler
4. TurboFan


## [如何使用gdb调试v8](https://github.com/earlymeme/v8/issues/2)
1. gdbinit
2. gdb-v8-support.py

## [如何使用lldb调试v8](https://github.com/earlymeme/v8/issues/1)
1. lldbinit
2. lldb_commands.py

## 分支
新建了annotated分支,用于加注解,debug代码

master分支用于merge from upsteam/master

如何update fork
```
 gclient sync
 gclient runhooks
```

```
git fetch upstream
git checkout master
git merge upstream/master

```

1. https://help.github.com/articles/syncing-a-fork/
2. https://help.github.com/articles/merging-an-upstream-repository-into-your-fork/

## 准备看代码前

### 术语解释

1. Local
```
Local<String> script_name = ...;
```
v8管理的String对象,让v8对String进行GC

访问Local的IsEmpty
```
(lldb) p script_name.IsEmpty()
(bool) $12 = false
```
访问String类的Length
```
(lldb) p script_name->Length()
(int) $14 = 7
```

2. Handle
  泛型,Handle<T>, 重写了`operator ->`和`operator *`,使用 operator -> 访问类型T真实的成员 
  
3. Isolate
  独立的v8运行时,拥有自己的内存,支持并行

4. V8_EXPORT
```
class V8_EXPORT ArrayBuffer : public Object {
```

```v8.h
// Setup for Windows DLL export/import. When building the V8 DLL the
// BUILDING_V8_SHARED needs to be defined. When building a program which uses
// the V8 DLL USING_V8_SHARED needs to be defined. When either building the V8
// static library or building a program which uses the V8 static library neither
// BUILDING_V8_SHARED nor USING_V8_SHARED should be defined.
#ifdef BUILDING_V8_SHARED
# define V8_EXPORT __declspec(dllexport)
#elif USING_V8_SHARED
# define V8_EXPORT __declspec(dllimport)
#else
# define V8_EXPORT
#endif  // BUILDING_V8_SHARED

```
 可见性,给外部的shared对象调用函数
 default visibility -> global scope,可以从其他shared对象调用
 hidden visibility -> local scope,不能从其他shared对象调用
 默认的话,是public, gcc编译时提供`-fvisibility=hidden`,那么默认的就是hidden的,在build/common.gypi配置
 

5. ToLocalChecked

6. Small Integers

`v8.h`里描述了Smi是如何定义的.从32bit里借最右一位.一个Small integer是由一个32位直接数和64位数的指针.
```
// Formats of Object*:
//  Smi:        [31 bit signed int] 0
//  HeapObject: [32 bit direct pointer] (4 byte aligned) | 01
```
7. Hidden classes

给d8加上`--trace-maps`

比如:
`` lldb -- out.gn/x64.debug/d8 --trace-maps --allow-natives-syntax demo/test16.js
``

```
(lldb) r
Process 26464 launched: '/home/zhujianchen/work/git/v8/out.gn/x64.debug/d8' (x86_64)
[TraceMap: InitialMap map= 0x3afea78032d9 SFI= 0_Object ]
[TraceMaps: Transition from= 0x3afea78032d9 to= 0x3afea7803331 name= testExtraShouldReturnFive ]
[TraceMaps: Transition from= 0x3afea7803331 to= 0x3afea7803389 name= testExtraShouldCallToRuntime ]
[TraceMaps: Transition from= 0x3afea7803389 to= 0x3afea78033e1 name= testFunctionToString ]
[TraceMaps: Transition from= 0x3afea78033e1 to= 0x3afea7803439 name= testStackTrace ]
[TraceMaps: Transition from= 0x3afea7803439 to= 0x3afea7803491 name= testExtraCanUseUtils ]
[TraceMaps: Transition from= 0x3afea78032d9 to= 0x3afea7803cd1 name= pop ]
[TraceMaps: Transition from= 0x3afea7803cd1 to= 0x3afea7803d29 name= push ]
[TraceMaps: Transition from= 0x3afea7803d29 to= 0x3afea7803d81 name= shift ]
[TraceMaps: Transition from= 0x3afea7803d81 to= 0x3afea7803dd9 name= unshift ]
[TraceMaps: Transition from= 0x3afea7803dd9 to= 0x3afea7803e31 name= slice ]
[TraceMaps: Transition from= 0x3afea7803e31 to= 0x3afea7803e89 name= splice ]
[TraceMaps: Transition from= 0x3afea7803e89 to= 0x3afea7803ee1 name= includes ]
[TraceMaps: Transition from= 0x3afea7803ee1 to= 0x3afea7803f39 name= indexOf ]
[TraceMaps: Transition from= 0x3afea7803f39 to= 0x3afea7803f91 name= keys ]
[TraceMaps: Transition from= 0x3afea7803f91 to= 0x3afea7803fe9 name= values ]
[TraceMaps: Transition from= 0x3afea7803fe9 to= 0x3afea7804041 name= entries ]
[TraceMaps: Transition from= 0x3afea78032d9 to= 0x3afea7804099 name= defineProperty ]
[TraceMaps: Transition from= 0x3afea7804099 to= 0x3afea78040f1 name= deleteProperty ]
[TraceMaps: Transition from= 0x3afea78040f1 to= 0x3afea7804149 name= apply ]
[TraceMaps: Transition from= 0x3afea7804149 to= 0x3afea78041a1 name= construct ]
[TraceMaps: Transition from= 0x3afea78041a1 to= 0x3afea78041f9 name= get ]
[TraceMaps: Transition from= 0x3afea78041f9 to= 0x3afea7804251 name= getOwnPropertyDescriptor ]
[TraceMaps: Transition from= 0x3afea7804251 to= 0x3afea78042a9 name= getPrototypeOf ]
[TraceMaps: Transition from= 0x3afea78042a9 to= 0x3afea7804301 name= has ]
[TraceMaps: Transition from= 0x3afea7804301 to= 0x3afea7804359 name= isExtensible ]
[TraceMaps: Transition from= 0x3afea7804359 to= 0x3afea78043b1 name= ownKeys ]
[TraceMaps: Transition from= 0x3afea78043b1 to= 0x3afea7804409 name= preventExtensions ]
[TraceMaps: Transition from= 0x3afea7804409 to= 0x3afea7804461 name= set ]
[TraceMaps: Transition from= 0x3afea7804461 to= 0x3afea78044b9 name= setPrototypeOf ]
[TraceMaps: Transition from= 0x3afea78032d9 to= 0x3afea780aef1 name= constructor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780aef1 to= 0x3afea780afa1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780af49 SFI= 0_ ]
[TraceMaps: Transition from= 0x3afea78032d9 to= 0x3afea780b051 name= constructor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b101 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b0a9 SFI= 1_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b1b1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b159 SFI= 0_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b159 to= 0x3afea780b209 reason= CopyAsPrototype ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b2b9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b261 SFI= 2_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b209 to= 0x3afea780b311 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b3c1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b369 SFI= 3_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b311 to= 0x3afea780b419 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b4c9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b471 SFI= 4_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b419 to= 0x3afea780b521 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b5d1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b579 SFI= 5_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b521 to= 0x3afea780b629 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b6d9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b681 SFI= 6_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b629 to= 0x3afea780b731 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b7e1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b789 SFI= 7_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b731 to= 0x3afea780b839 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b8e9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b891 SFI= 8_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b839 to= 0x3afea780b941 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780b9f1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780b999 SFI= 9_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b941 to= 0x3afea780ba49 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780baf9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780baa1 SFI= 10_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780ba49 to= 0x3afea780bb51 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780bb51 to= 0x3afea780bba9 reason= CopyAddDescriptor ]
[TraceMaps: Normalize from= 0x3afea78032d9 to= 0x3afea780bc01 reason= TransitionToAccessorPair ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780bcb1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780bc59 SFI= 11_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780bd61 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780bd09 SFI= 12_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780be11 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780bdb9 SFI= 13_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780bec1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780be69 SFI= 14_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780bf71 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780bf19 SFI= 15_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c021 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780bfc9 SFI= 16_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c0d1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780c079 SFI= 17_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c181 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780c129 SFI= 18_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c231 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780c1d9 SFI= 19_ ]
[TraceMaps: SlowToFast from= 0x3afea780bc01 to= 0x3afea780c289 reason= ApiNatives::InstantiateObject ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780bba9 to= 0x3afea780c2e1 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c391 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780c339 SFI= 20_ ]
[TraceMaps: Transition from= 0x3afea78032d9 to= 0x3afea780c3e9 name= now ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780c2e1 to= 0x3afea780c441 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78032d9 to= 0x3afea780c499 reason= CopyAsPrototype ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c549 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780c4f1 SFI= 21_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780c499 to= 0x3afea780c5a1 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c651 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780c5f9 SFI= 22_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780c5a1 to= 0x3afea780c6a9 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c759 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780c701 SFI= 23_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780c6a9 to= 0x3afea780c7b1 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780c7b1 to= 0x3afea780c809 reason= CopyAddDescriptor ]
[TraceMaps: InitialMap map= 0x3afea780c861 SFI= 24_Worker ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780c441 to= 0x3afea780c8b9 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780c969 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780c911 SFI= 25_ ]
[TraceMaps: Transition from= 0x3afea78032d9 to= 0x3afea780c9c1 name= system ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780ca71 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780ca19 SFI= 26_ ]
[TraceMaps: Transition from= 0x3afea780c9c1 to= 0x3afea780cac9 name= chdir ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780cb79 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780cb21 SFI= 27_ ]
[TraceMaps: Transition from= 0x3afea780cac9 to= 0x3afea780cbd1 name= setenv ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780cc81 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780cc29 SFI= 28_ ]
[TraceMaps: Transition from= 0x3afea780cbd1 to= 0x3afea780ccd9 name= unsetenv ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780cd89 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780cd31 SFI= 29_ ]
[TraceMaps: Transition from= 0x3afea780ccd9 to= 0x3afea780cde1 name= umask ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780ce91 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780ce39 SFI= 30_ ]
[TraceMaps: Transition from= 0x3afea780cde1 to= 0x3afea780cee9 name= mkdirp ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780cf99 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780cf41 SFI= 31_ ]
[TraceMaps: Transition from= 0x3afea780cee9 to= 0x3afea780cff1 name= rmdir ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780c8b9 to= 0x3afea780d049 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780d049 to= 0x3afea780d0a1 reason= CopyAddDescriptor ]
[TraceMaps: InitialMap map= 0x3afea780d0f9 SFI= 1_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b0a9 to= 0x3afea780d151 reason= ForceSetPrototype ]
[TraceMaps: Transition from= 0x3afea780b159 to= 0x3afea780d1a9 name= print ]
[TraceMaps: Transition from= 0x3afea780d1a9 to= 0x3afea780d201 name= printErr ]
[TraceMaps: Transition from= 0x3afea780d201 to= 0x3afea780d259 name= write ]
[TraceMaps: Transition from= 0x3afea780d259 to= 0x3afea780d2b1 name= read ]
[TraceMaps: Transition from= 0x3afea780d2b1 to= 0x3afea780d309 name= readbuffer ]
[TraceMaps: Transition from= 0x3afea780d309 to= 0x3afea780d361 name= readline ]
[TraceMaps: Transition from= 0x3afea780d361 to= 0x3afea780d3b9 name= load ]
[TraceMaps: Transition from= 0x3afea780d3b9 to= 0x3afea780d411 name= quit ]
[TraceMaps: Transition from= 0x3afea780d411 to= 0x3afea780d469 name= version ]
[TraceMaps: Transition from= 0x3afea780d469 to= 0x3afea780d4c1 name= <Symbol.toStringTag> ]
[TraceMaps: Transition from= 0x3afea780d4c1 to= 0x3afea780d519 name= Realm ]
[TraceMaps: Transition from= 0x3afea780d519 to= 0x3afea780d571 name= performance ]
[TraceMaps: Transition from= 0x3afea780d571 to= 0x3afea780d5c9 name= Worker ]
[TraceMaps: Transition from= 0x3afea780d5c9 to= 0x3afea780d621 name= os ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780aff9 to= 0x3afea780d679 reason= ForceSetPrototype ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780d151 to= 0x3afea780d6d1 reason= ForceSetPrototype ]
[TraceMaps: ReplaceDescriptors from= 0x3afea7805279 to= 0x3afea780d729 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78078f9 to= 0x3afea780d781 reason= GenAll_RootModification2 ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780d781 to= 0x3afea780d7d9 reason= GenAll_RootModification2 ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780d7d9 to= 0x3afea780d831 reason= GenAll_RootModification2 ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780d831 to= 0x3afea780d889 reason= GenAll_RootModification2 ]
[TraceMaps: ReplaceDescriptors from= 0x3afea7802db1 to= 0x3afea780d8e1 reason= CopyAddDescriptor ]
[TraceMaps: InitialMap map= 0x3afea780d991 SFI= 34_WebAssembly ]
[TraceMaps: Transition from= 0x3afea780d991 to= 0x3afea780d9e9 name= <Symbol.toStringTag> ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780da99 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780da41 SFI= 35_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780daf1 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780d9e9 to= 0x3afea780db49 name= compile ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780dbf9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780dba1 SFI= 36_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780dc51 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780db49 to= 0x3afea780dca9 name= validate ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780dd59 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780dd01 SFI= 37_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780ddb1 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780dca9 to= 0x3afea780de09 name= instantiate ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780deb9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780de61 SFI= 38_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780df11 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780de09 to= 0x3afea780df69 name= Module ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780de61 to= 0x3afea780e019 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780dfc1 SFI= 38_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780e0c9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780e071 SFI= 39_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780e121 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780df11 to= 0x3afea780e179 name= imports ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780e229 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780e1d1 SFI= 40_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780e281 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780e179 to= 0x3afea780e2d9 name= exports ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780e389 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780e331 SFI= 41_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780e3e1 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780e2d9 to= 0x3afea780e439 name= customSections ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780e019 to= 0x3afea780e491 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780e491 to= 0x3afea780e4e9 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780e599 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780e541 SFI= 42_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780e5f1 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780df69 to= 0x3afea780e649 name= Instance ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780e541 to= 0x3afea780e6f9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780e6a1 SFI= 42_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780e6f9 to= 0x3afea780e751 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780e751 to= 0x3afea780e7a9 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780e859 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780e801 SFI= 43_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780e8b1 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780e649 to= 0x3afea780e909 name= Table ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780e801 to= 0x3afea780e9b9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780e961 SFI= 43_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780e9b9 to= 0x3afea780ea11 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780eac1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780ea69 SFI= 44_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780ea11 to= 0x3afea780eb19 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780ebc9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780eb71 SFI= 45_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780ec21 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780eb19 to= 0x3afea780ec79 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780ed29 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780ecd1 SFI= 46_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780ed81 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780ec79 to= 0x3afea780edd9 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780ee89 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780ee31 SFI= 47_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780eee1 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780edd9 to= 0x3afea780ef39 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780ef39 to= 0x3afea780ef91 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780f041 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780efe9 SFI= 48_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780f099 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: Transition from= 0x3afea780e909 to= 0x3afea780f0f1 name= Memory ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780efe9 to= 0x3afea780f1a1 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780f149 SFI= 48_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780f1a1 to= 0x3afea780f1f9 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780f2a9 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780f251 SFI= 49_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea78029e9 to= 0x3afea780f301 reason= GenAll_AttributesMismatchProtoMap ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780f1f9 to= 0x3afea780f359 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780b051 to= 0x3afea780f409 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780f3b1 SFI= 50_ ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780f359 to= 0x3afea780f461 reason= CopyAddDescriptor ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780f461 to= 0x3afea780f4b9 reason= CopyAddDescriptor ]
[TraceMaps: Transition from= 0x3afea780f0f1 to= 0x3afea780f511 name= CompileError ]
[TraceMaps: Transition from= 0x3afea780f511 to= 0x3afea780f569 name= LinkError ]
[TraceMaps: Transition from= 0x3afea780f569 to= 0x3afea780f5c1 name= RuntimeError ]
FUNCTION "Person" = function Person
VARIABLE (0x555555637460) (mode = CONST) "p"
[TraceMaps: Normalize from= 0x3afea780b051 to= 0x3afea780f6c9 reason= NormalizeAsPrototype ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780f6c9 to= 0x3afea780f721 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780f671 SFI= 52_Person ]
[TraceMaps: Transition from= 0x3afea780f671 to= 0x3afea780f779 name= name ]
[TraceMaps: Transition from= 0x3afea780f779 to= 0x3afea780f7d1 name= age ]
Process 26464 exited with status = 0 (0x00000000) 

```
上面的log是全部的transitions maps,筛选出有用的如下:
```
[TraceMap: InitialMap map= 0x3afea78032d9 SFI= 0_Object ]
...
FUNCTION "Person" = function Person
VARIABLE (0x555555637460) (mode = CONST) "p"
[TraceMaps: Normalize from= 0x3afea780b051 to= 0x3afea780f6c9 reason= NormalizeAsPrototype ]
[TraceMaps: ReplaceDescriptors from= 0x3afea780f6c9 to= 0x3afea780f721 reason= CopyAsPrototype ]
[TraceMaps: InitialMap map= 0x3afea780f671 SFI= 52_Person ]
[TraceMaps: Transition from= 0x3afea780f671 to= 0x3afea780f779 name= name ]
[TraceMaps: Transition from= 0x3afea780f779 to= 0x3afea780f7d1 name= age ]

```


---

forked from https://github.com/v8/v8

V8 JavaScript Engine
=============

V8 is Google's open source JavaScript engine.

V8 implements ECMAScript as specified in ECMA-262.

V8 is written in C++ and is used in Google Chrome, the open source
browser from Google.

V8 can run standalone, or can be embedded into any C++ application.

V8 Project page: https://github.com/v8/v8/wiki


Getting the Code
=============

Checkout [depot tools](http://www.chromium.org/developers/how-tos/install-depot-tools), and run

        fetch v8

This will checkout V8 into the directory `v8` and fetch all of its dependencies.
To stay up to date, run

        git pull origin
        gclient sync

For fetching all branches, add the following into your remote
configuration in `.git/config`:

        fetch = +refs/branch-heads/*:refs/remotes/branch-heads/*
        fetch = +refs/tags/*:refs/tags/*


Contributing
=============

Please follow the instructions mentioned on the
[V8 wiki](https://github.com/v8/v8/wiki/Contributing).