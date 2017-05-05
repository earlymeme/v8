// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_H_
#define V8_COMPILER_H_

#include <memory>

#include "src/allocation.h"
#include "src/bailout-reason.h"
#include "src/contexts.h"
#include "src/isolate.h"
#include "src/zone/zone.h"

namespace v8 {
namespace internal {

// Forward declarations.
class CompilationInfo;
class CompilationJob;
class JavaScriptFrame;
class ParseInfo;
class ScriptData;
template <typename T>
class ThreadedList;
template <typename T>
class ThreadedListZoneEntry;

// The V8 compiler API.
//
// This is the central hub for dispatching to the various compilers within V8.
// Logic for which compiler to choose and how to wire compilation results into
// the object heap should be kept inside this class.
//
// General strategy: Scripts are translated into anonymous functions w/o
// parameters which then can be executed. If the source code contains other
// functions, they might be compiled and allocated as part of the compilation
// of the source code or deferred for lazy compilation at a later point.
// 编译器API
// 策略：脚本会被翻译成不带有形参（之后可能会执行）的匿名函数。
// 如果有函数嵌套，这些内部函数会作为编译的一部分，
// 或者在之后某个时间点延迟编译
class V8_EXPORT_PRIVATE Compiler : public AllStatic {
 public:
  // 清除异常标志
  enum ClearExceptionFlag { KEEP_EXCEPTION, CLEAR_EXCEPTION };
  // 并发模式
  enum ConcurrencyMode { NOT_CONCURRENT, CONCURRENT };

  // ===========================================================================
  // The following family of methods ensures a given function is compiled. The
  // general contract is that failures will be reported by returning {false},
  // whereas successful compilation ensures the {is_compiled} predicate on the
  // given function holds (except for live-edit, which compiles the world).
  // 标志位。一般约定：失败返回false，成功设置is_compiled（除了实时编辑会编译全部）
  static bool Compile(Handle<JSFunction> function, ClearExceptionFlag flag);
  static bool CompileOptimized(Handle<JSFunction> function, ConcurrencyMode);
  static bool CompileDebugCode(Handle<SharedFunctionInfo> shared);
  static MaybeHandle<JSArray> CompileForLiveEdit(Handle<Script> script);

  // Prepare a compilation job for unoptimized code. Requires ParseAndAnalyse.
  // 预备未优化代码的编译任务
  static CompilationJob* PrepareUnoptimizedCompilationJob(
      CompilationInfo* info);

  // Generate and install code from previously queued compilation job.
  // 生成，设置已加入队列里的编译任务
  static bool FinalizeCompilationJob(CompilationJob* job);

  // Give the compiler a chance to perform low-latency initialization tasks of
  // the given {function} on its instantiation. Note that only the runtime will
  // offer this chance, optimized closure instantiation will not call this.
  // 执行指定函数低延迟初始化任务。只在运行时才有效，已优化的闭包初始化不会调用这个方法。
  static void PostInstantiation(Handle<JSFunction> function, PretenureFlag);
  // 内部函数队列
  typedef ThreadedList<ThreadedListZoneEntry<FunctionLiteral*>>
      EagerInnerFunctionLiterals;

  // Parser::Parse, then Compiler::Analyze.
  static bool ParseAndAnalyze(ParseInfo* info, Isolate* isolate);
  // Convenience function
  // 方便方法
  static bool ParseAndAnalyze(CompilationInfo* info);
  // Rewrite, analyze scopes, and renumber. If |eager_literals| is non-null, it
  // is appended with inner function literals which should be eagerly compiled.
  // 重写，分析作用域，重排。如果eager_literals不为null，立刻要编译的内部函数会被追加，
  static bool Analyze(ParseInfo* info, Isolate* isolate,
                      EagerInnerFunctionLiterals* eager_literals = nullptr);
  // Convenience function
  // 便利函数
  static bool Analyze(CompilationInfo* info,
                      EagerInnerFunctionLiterals* eager_literals = nullptr);
  // Adds deoptimization support, requires ParseAndAnalyze.
  // 去优化支持
  static bool EnsureDeoptimizationSupport(CompilationInfo* info);
  // Ensures that bytecode is generated, calls ParseAndAnalyze internally.
  // 保证已经生成字节码
  static bool EnsureBytecode(CompilationInfo* info);

  // ===========================================================================
  // The following family of methods instantiates new functions for scripts or
  // function literals. The decision whether those functions will be compiled,
  // is left to the discretion of the compiler.
  //
  // Please note this interface returns shared function infos.  This means you
  // need to call Factory::NewFunctionFromSharedFunctionInfo before you have a
  // real function with a context.

  // Create a (bound) function for a String source within a context for eval.
  // 下面的方法会初始化function实例（scripts或者function），决定哪些function会被编译交给编译器
  // 如果需要一个上下文函数实例，需要调用Factory::NewFunctionFromSharedFunctionInfo得到共享function对象
  // 在上下文里，从eval里的字符串源码创建一个字符串（绑定的）函数
  MUST_USE_RESULT static MaybeHandle<JSFunction> GetFunctionFromEval(
      Handle<String> source, Handle<SharedFunctionInfo> outer_info,
      Handle<Context> context, LanguageMode language_mode,
      ParseRestriction restriction, int parameters_end_pos,
      int eval_scope_position, int eval_position, int line_offset = 0,
      int column_offset = 0, Handle<Object> script_name = Handle<Object>(),
      ScriptOriginOptions options = ScriptOriginOptions());

  // Create a (bound) function for a String source within a context for eval.
  // 处理eval，字符串得到函数
  MUST_USE_RESULT static MaybeHandle<JSFunction> GetFunctionFromString(
      Handle<Context> context, Handle<String> source,
      ParseRestriction restriction, int parameters_end_pos);

  // Create a shared function info object for a String source within a context.
  // 处理字符串源码
  static Handle<SharedFunctionInfo> GetSharedFunctionInfoForScript(
      Handle<String> source, Handle<Object> script_name, int line_offset,
      int column_offset, ScriptOriginOptions resource_options,
      Handle<Object> source_map_url, Handle<Context> context,
      v8::Extension* extension, ScriptData** cached_data,
      ScriptCompiler::CompileOptions compile_options,
      NativesFlag is_natives_code);

  // Create a shared function info object for a Script that has already been
  // parsed while the script was being loaded from a streamed source.
  // 处理stream流已加载，解析的脚本
  static Handle<SharedFunctionInfo> GetSharedFunctionInfoForStreamedScript(
      Handle<Script> script, ParseInfo* info, int source_length);

  // Create a shared function info object (the code may be lazily compiled).
  // 得到共享函数对象，这个script的code可能会延迟编译
  static Handle<SharedFunctionInfo> GetSharedFunctionInfo(
      FunctionLiteral* node, Handle<Script> script, CompilationInfo* outer);

  // Create a shared function info object for a native function literal.
  // 处理原生函数
  static Handle<SharedFunctionInfo> GetSharedFunctionInfoForNative(
      v8::Extension* extension, Handle<String> name);

  // ===========================================================================
  // The following family of methods provides support for OSR. Code generated
  // for entry via OSR might not be suitable for normal entry, hence will be
  // returned directly to the caller.
  //
  // Please note this interface is the only part dealing with {Code} objects
  // directly. Other methods are agnostic to {Code} and can use an interpreter
  // instead of generating JIT code for a function at all.

  // Generate and return optimized code for OSR, or empty handle on failure.
  /**
   * 处理OSR，获得已优化的代码
   * @param {JSFunction} function js函数
   * @param {BailoutId} osr_ast_id
   * @return {Code}
   */
  MUST_USE_RESULT static MaybeHandle<Code> GetOptimizedCodeForOSR(
      Handle<JSFunction> function, BailoutId osr_ast_id,
      JavaScriptFrame* osr_frame);
};

// A base class for compilation jobs intended to run concurrent to the main
// thread. The job is split into three phases which are called in sequence on
// different threads and with different limitations:
//  1) PrepareJob:   Runs on main thread. No major limitations.
//  2) ExecuteJob:   Runs concurrently. No heap allocation or handle derefs.
//  3) FinalizeJob:  Runs on main thread. No dependency changes.
//
// Each of the three phases can either fail or succeed. The current state of
// the job can be checked using {state()}.
// 并行处理编译任务
// 分成3个阶段：1）预备：执行主线程。2）执行：并行执行。没有内存分配或者句柄析构。3）完成：执行主线程。没有依赖改变。
// 每个阶段可能失败或者成功。当前的任务状态使用state()可以知道。
class V8_EXPORT_PRIVATE CompilationJob {
 public:
  enum Status { SUCCEEDED, FAILED };
  enum class State {
    kReadyToPrepare,
    kReadyToExecute,
    kReadyToFinalize,
    kSucceeded,
    kFailed,
  };

  CompilationJob(Isolate* isolate, CompilationInfo* info,
                 const char* compiler_name,
                 State initial_state = State::kReadyToPrepare);
  virtual ~CompilationJob() {}

  // Prepare the compile job. Must be called on the main thread.
  // 必须在主线程上调用
  MUST_USE_RESULT Status PrepareJob();

  // Executes the compile job. Can be called on a background thread if
  // can_execute_on_background_thread() returns true.
  // 如果can_execute_on_background_thread()返回true可以在后台进程上调用
  MUST_USE_RESULT Status ExecuteJob();

  // Finalizes the compile job. Must be called on the main thread.
  // 必须在主线程上调用
  MUST_USE_RESULT Status FinalizeJob();

  // Report a transient failure, try again next time. Should only be called on
  // optimization compilation jobs.
  // 报告短暂失败，然后重试。在编译优化时调用
  Status RetryOptimization(BailoutReason reason);

  // Report a persistent failure, disable future optimization on the function.
  // Should only be called on optimization compilation jobs.
  // 报告永久失败，禁用未来的优化
  Status AbortOptimization(BailoutReason reason);
  // 记录优化编译状态
  void RecordOptimizedCompilationStats() const;
  // 记录未优化的编译状态
  void RecordUnoptimizedCompilationStats() const;
  // 是否可以执行在后台进程上
  virtual bool can_execute_on_background_thread() const { return true; }
  // 设置栈顶指针
  void set_stack_limit(uintptr_t stack_limit) { stack_limit_ = stack_limit; }
  // 返回栈顶指针
  uintptr_t stack_limit() const { return stack_limit_; }
  // 设置can_execute_on_background_thread
  bool executed_on_background_thread() const {
    DCHECK_IMPLIES(!can_execute_on_background_thread(),
                   !executed_on_background_thread_);
    return executed_on_background_thread_;
  }
  // 状态
  State state() const { return state_; }
  // 编译的信息
  CompilationInfo* info() const { return info_; }
  // 隔离环境
  Isolate* isolate() const;

 protected:
  // Overridden by the actual implementation.
  // 会被实际实现重写
  virtual Status PrepareJobImpl() = 0;
  virtual Status ExecuteJobImpl() = 0;
  virtual Status FinalizeJobImpl() = 0;

  // Registers weak object to optimized code dependencies.
  // TODO(turbofan): Move this to pipeline.cc once Crankshaft dies.
  // 在已优化代码依赖注册弱对象
  void RegisterWeakObjectsInOptimizedCode(Handle<Code> code);

 private:
  CompilationInfo* info_;
  ThreadId isolate_thread_id_;
  base::TimeDelta time_taken_to_prepare_;
  base::TimeDelta time_taken_to_execute_;
  base::TimeDelta time_taken_to_finalize_;
  const char* compiler_name_;
  State state_;
  uintptr_t stack_limit_;
  // 标志位，是否可以在后台进程上执行
  bool executed_on_background_thread_;
  // 更新状态
  MUST_USE_RESULT Status UpdateState(Status status, State next_state) {
    if (status == SUCCEEDED) {
      state_ = next_state;
    } else {
      state_ = State::kFailed;
    }
    return status;
  }
};

}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_H_
