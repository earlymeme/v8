## 学习笔记

handles.h

```
template <typename S>
  static const Handle<T> cast(Handle<S> that) {
    T::cast(*reinterpret_cast<T**>(that.location_));// 调用T类型的静态方法cast,比如
    return Handle<T>(reinterpret_cast<T**>(that.location_));
  }
  
```

feedback-vector.cc

```

Handle<FeedbackMetadata> metadata = Handle<FeedbackMetadata>::cast(array); // 类型cast,FixedArray => FeedbackMetadata

```


compiler.cc

```
Handle<SharedFunctionInfo> CompileToplevel(CompilationInfo* info) {
...
  OFStream os(stdout);
  array->FixedArrayPrint(os);
  os << std::flush;
...
}

```

```interpreter.cc InterpreterCompilationJob::PrepareJobImpl
if (print_bytecode_) {
    OFStream os(stdout);
    std::unique_ptr<char[]> name = info()->GetDebugName();
    os << "[generating bytecode for function: " << info()->GetDebugName().get()
       << "]" << std::endl
       << std::flush;
  }
```

