### Build
```
tools/dev/v8gen.py x64.debug -- v8_enable_object_print=true v8_enable_gdbjit=true v8_enable_trace_maps=true v8_enable_verify_heap=true

ninja -C out.gn/x64.debug

```

### Debug
```

out.gn/x64.debug/d8 --trace-opt --trace-deopt --allow-natives-syntax demo/test11.js

gdb --args out.gn/x64.debug/d8 --ignition --trace-ignition --trace --trace-opt-verbose --trace-deopt --log --log-colour --trace-maps --allow-natives-syntax --print-ast --print-bytecode  --enable-inspector demo/test3.js

out.gn/x64.debug/d8  --trace --trace-opt-verbose --trace-deopt --allow-natives-syntax  --enable-inspector --trace-maps --code-comments demo/test6.js

out.gn/x64.debug/d8 --print-opt-code --print-code-verbose --allow-natives-syntax --code-comments  demo/test7.js

gdb --args out.gn/x64.debug/cctest test-func-name-inference/GlobalProperty

tools/run-tests.py --gn -m debug debugger/debug/debug-stepout-scope-part3

gdb --args out.gn/x64.debug/v8_hello_world

```

### Update v8

1. gclient sync
2. ./gs v8/DEPS

- 修改了
```
install-sysroot.py
v8/third_party/binutils/download.py
v8/tools/clang/scripts/update.py

```
