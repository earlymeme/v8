--- Raw source ---
(o) {
    return o.x;
}


--- Optimized code ---
optimization_id = 0
source_position = 10
kind = OPTIMIZED_FUNCTION
name = f
stack_slots = 5
compiler = crankshaft
Instructions (size = 167)
0x87e461860a0     0  55             push rbp
0x87e461860a1     1  4889e5         REX.W movq rbp,rsp
0x87e461860a4     4  56             push rsi
0x87e461860a5     5  57             push rdi
0x87e461860a6     6  4883ec08       REX.W subq rsp,0x8
0x87e461860aa    10  50             push rax
0x87e461860ab    11  b801000000     movl rax,0x1
0x87e461860b0    16  49baefdeefbeaddeefbe REX.W movq r10,0xbeefdeadbeefdeef
0x87e461860ba    26  4c8914c4       REX.W movq [rsp+rax*8],r10
0x87e461860be    30  ffc8           decl rax
0x87e461860c0    32  75f8           jnz 26  (0x87e461860ba)
0x87e461860c2    34  58             pop rax
                  ;;; <@0,#0> -------------------- B0 --------------------
                  ;;; <@6,#4> prologue
                  ;;; Prologue begin
                  ;;; Prologue end
                  ;;; <@10,#6> -------------------- B1 --------------------
                  ;;; <@12,#7> context
0x87e461860c3    35  488b45f8       REX.W movq rax,[rbp-0x8]
                  ;;; <@13,#7> gap
0x87e461860c7    39  488945e8       REX.W movq [rbp-0x18],rax
                  ;;; <@16,#11> -------------------- B2 --------------------
                  ;;; <@17,#11> gap
0x87e461860cb    43  488bf0         REX.W movq rsi,rax
                  ;;; <@18,#13> stack-check
0x87e461860ce    46  493ba5400d0000 REX.W cmpq rsp,[r13+0xd40]
0x87e461860d5    53  7305           jnc 60  (0x87e461860dc)
0x87e461860d7    55  e8447bf4ff     call StackCheck  (0x87e460cdc20)    ;; code: BUILTIN
                  ;;; <@20,#13> lazy-bailout
                  ;;; <@21,#13> gap
0x87e461860dc    60  488b4510       REX.W movq rax,[rbp+0x10]
                  ;;; <@22,#15> check-non-smi
0x87e461860e0    64  a801           test al,0x1
0x87e461860e2    66  0f8441000000   jz 137  (0x87e46186129)
                  ;;; <@24,#16> check-maps
0x87e461860e8    72  49ba31e398bac03a0000 REX.W movq r10,0x3ac0ba98e331    ;; object: 0x3ac0ba98e331 <Map(FAST_HOLEY_ELEMENTS)>
0x87e461860f2    82  4c3950ff       REX.W cmpq [rax-0x1],r10
0x87e461860f6    86  0f8532000000   jnz 142  (0x87e4618612e)
                  ;;; <@26,#17> load-named-field
0x87e461860fc    92  4c8b5017       REX.W movq r10,[rax+0x17]
0x87e46186100    96  41f6c201       testb r10,0x1
0x87e46186104   100  7410           jz 118  (0x87e46186116)
                  Abort message: 
                  Operand is not a smi
0x87e46186106   102  48ba0000000076000000 REX.W movq rdx,0x7600000000
0x87e46186110   112  e80be0e7ff     call Abort  (0x87e46004120)    ;; code: BUILTIN
0x87e46186115   117  cc             int3l
0x87e46186116   118  8b401b         movl rax,[rax+0x1b]
                  ;;; <@28,#21> smi-tag
0x87e46186119   121  8bd8           movl rbx,rax
0x87e4618611b   123  48c1e320       REX.W shlq rbx, 32
                  ;;; <@29,#21> gap
0x87e4618611f   127  488bc3         REX.W movq rax,rbx
                  ;;; <@30,#19> return
0x87e46186122   130  488be5         REX.W movq rsp,rbp
0x87e46186125   133  5d             pop rbp
0x87e46186126   134  c21000         ret 0x10
                  ;;; -------------------- Jump table --------------------
0x87e46186129   137  e8dcdecfff     call 0x87e45e8400a       ;; debug: deopt position, script offset '-1'
                                                             ;; debug: deopt position, inlining id '-1'
                                                             ;; debug: deopt reason 'Smi'
                                                             ;; debug: deopt index 1
                                                             ;; deoptimization bailout 1
0x87e4618612e   142  e8e1decfff     call 0x87e45e84014       ;; debug: deopt position, script offset '-1'
                                                             ;; debug: deopt position, inlining id '-1'
                                                             ;; debug: deopt reason 'wrong map'
                                                             ;; debug: deopt index 2
                                                             ;; deoptimization bailout 2
0x87e46186133   147  90             nop
                  ;;; Safepoint table.

Inlined functions (count = 0)

Deoptimization Input Data (deopt points = 3)
 index  ast id    argc     pc  commands
     0       4       0     60  BEGIN {frame count=1, js frame count=1}
                               JS_FRAME {ast_id=4, function=0x3be561308781 <String[1]: f>, height=1}
                               STACK_SLOT {input=3}
                               STACK_SLOT {input=-2}
                               STACK_SLOT {input=-1}
                               STACK_SLOT {input=4}
     1       4       0     -1  BEGIN {frame count=1, js frame count=1}
                               JS_FRAME {ast_id=4, function=0x3be561308781 <String[1]: f>, height=1}
                               STACK_SLOT {input=3}
                               STACK_SLOT {input=-2}
                               REGISTER {input=rax}
                               STACK_SLOT {input=4}
     2       4       0     -1  BEGIN {frame count=1, js frame count=1}
                               JS_FRAME {ast_id=4, function=0x3be561308781 <String[1]: f>, height=1}
                               STACK_SLOT {input=3}
                               STACK_SLOT {input=-2}
                               REGISTER {input=rax}
                               STACK_SLOT {input=4}

Safepoints (size = 19)
0x87e461860dc    60  10000 (sp -> fp)       0

RelocInfo (size = 267)
0x87e461860c3  comment  (;;; <@0,#0> -------------------- B0 --------------------)
0x87e461860c3  comment  (;;; <@6,#4> prologue)
0x87e461860c3  comment  (;;; Prologue begin)
0x87e461860c3  comment  (;;; Prologue end)
0x87e461860c3  comment  (;;; <@10,#6> -------------------- B1 --------------------)
0x87e461860c3  comment  (;;; <@12,#7> context)
0x87e461860c7  comment  (;;; <@13,#7> gap)
0x87e461860cb  comment  (;;; <@16,#11> -------------------- B2 --------------------)
0x87e461860cb  comment  (;;; <@17,#11> gap)
0x87e461860ce  comment  (;;; <@18,#13> stack-check)
0x87e461860d8  code target (BUILTIN)  (0x87e460cdc20)
0x87e461860dc  comment  (;;; <@20,#13> lazy-bailout)
0x87e461860dc  comment  (;;; <@21,#13> gap)
0x87e461860e0  comment  (;;; <@22,#15> check-non-smi)
0x87e461860e8  comment  (;;; <@24,#16> check-maps)
0x87e461860ea  embedded object  (0x3ac0ba98e331 <Map(FAST_HOLEY_ELEMENTS)>)
0x87e461860fc  comment  (;;; <@26,#17> load-named-field)
0x87e46186106  comment  (Abort message: )
0x87e46186106  comment  (Operand is not a smi)
0x87e46186111  code target (BUILTIN)  (0x87e46004120)
0x87e46186119  comment  (;;; <@28,#21> smi-tag)
0x87e4618611f  comment  (;;; <@29,#21> gap)
0x87e46186122  comment  (;;; <@30,#19> return)
0x87e46186129  comment  (;;; -------------------- Jump table --------------------)
0x87e46186129  deopt script offset  (-1)
0x87e46186129  deopt inlining id  (-1)
0x87e46186129  deopt reason  (Smi)
0x87e46186129  deopt index
0x87e4618612a  runtime entry  (deoptimization bailout 1)
0x87e4618612e  deopt script offset  (-1)
0x87e4618612e  deopt inlining id  (-1)
0x87e4618612e  deopt reason  (wrong map)
0x87e4618612e  deopt index
0x87e4618612f  runtime entry  (deoptimization bailout 2)
0x87e46186134  comment  (;;; Safepoint table.)

--- End code ---
