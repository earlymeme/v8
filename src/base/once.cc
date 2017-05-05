// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/base/once.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sched.h>
#endif

#include "src/base/atomicops.h"

namespace v8 {
namespace base {

void CallOnceImpl(OnceType* once, PointerArgFunction init_func, void* arg) {
  AtomicWord state = Acquire_Load(once);
  // Fast path. The provided function was already executed.
  // 已经执行了的直接返回
  if (state == ONCE_STATE_DONE) {
    return;
  }

  // The function execution did not complete yet. The once object can be in one
  // of the two following states:
  //   - UNINITIALIZED: We are the first thread calling this function.
  //   - EXECUTING_FUNCTION: Another thread is already executing the function.
  //
  // First, try to change the state from UNINITIALIZED to EXECUTING_FUNCTION
  // atomically.
  // 函数执行还没有完成。
  // once对象可能是2种状态：UNINITIALIZED（第一次调用这个函数）；EXECUTING_FUNCTION（另外一个线程正在执行这个函数）
  // 首先尝试状态从UNINITIALIZED变成EXECUTING_FUNCTION
  state = Acquire_CompareAndSwap(
      once, ONCE_STATE_UNINITIALIZED, ONCE_STATE_EXECUTING_FUNCTION);
  if (state == ONCE_STATE_UNINITIALIZED) {
    // We are the first thread to call this function, so we have to call the
    // function.
    // 如果是第一个线程调用这个函数，就调用它
    init_func(arg);
    Release_Store(once, ONCE_STATE_DONE);
  } else {
    // Another thread has already started executing the function. We need to
    // wait until it completes the initialization.
    // 有另外一个线程已经开始执行这个函数了的话，就等待，直到完成初始化
    while (state == ONCE_STATE_EXECUTING_FUNCTION) {
#ifdef _WIN32
      ::Sleep(0);
#else
      sched_yield();
#endif
      state = Acquire_Load(once);
    }
  }
}

}  // namespace base
}  // namespace v8
