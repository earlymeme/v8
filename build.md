### Build命令

```
tools/dev/v8gen.py x64.debug -- v8_enable_disassembler=true v8_enable_object_print=true v8_enable_gdbjit=true v8_enable_trace_maps=true v8_enable_verify_heap=true v8_enable_v8_checks=true v8_enable_trace_ignition=true v8_interpreted_regexp=true v8_no_inline=true

ninja -C out.gn/x64.debug

gn args --list out.gn/x64.debug
```

### Debug

```
out.gn/x64.debug/d8 --trace-opt --trace-deopt --print-bytecode --ignition --allow-natives-syntax demo/test11.js

gdb --args out.gn/x64.debug/d8 --ignition --trace-ignition --trace --trace-opt-verbose --trace-deopt --log --log-colour --trace-maps --allow-natives-syntax --print-ast --print-bytecode  --enable-inspector demo/test3.js

out.gn/x64.debug/d8  --trace --trace-opt-verbose --trace-deopt --allow-natives-syntax  --enable-inspector --trace-maps --code-comments demo/test6.js

out.gn/x64.debug/d8 --print-opt-code --print-code-verbose --allow-natives-syntax --code-comments  demo/test7.js

// debug 单个c++测试用例
gdb --args out.gn/x64.debug/cctest test-func-name-inference/GlobalProperty
lldb -- out.gn/x64.debug/cctest test-interpreter/InterpreterReturn

// debug 单个测试用例
// 如果想要测试源码里某段代码关联的测试用例,可以使用 UNREACHABLE() 宏
tools/run-tests.py --gn -m debug debugger/debug/debug-stepout-scope-part3
tools/run-tests.py --gn -m debug mjsunit/array-sort
out.gn/x64.debug/unittests --random-seed=-1055853278 --gtest_filter=ValueSerializerTestWithWasm.RountripWasmInline --gtest_random_seed=-1055853278 --gtest_print_time=0 --nohard-abort --nodead-code-elimination --nofold-constants --enable-slow-asserts --verify-heap

gdb --args out.gn/x64.debug/v8_hello_world

lldb -- out.gn/x64.debug/v8_sample_process samples/count-hosts.js
```

### Update v8

1. gclient sync
2. ./gs v8/DEPS

> sync fork
gclient sync 会重写这几个文件,之后runhooks时,会下载失败,需要手动下载,然后修改这几个py文件的main方法,直接return
>
> 1. install-sysroot.py
> 2. v8/third_party/binutils/download.py
> 3. v8/tools/clang/scripts/update.py

export http_proxy=http://host:port
可以让命令行窗口也走代理