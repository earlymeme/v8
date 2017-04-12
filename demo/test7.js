function f(o) {
    return o.x;
}

f({x : 1}); f({x : 2});
%OptimizeFunctionOnNextCall(f);
f({x : 3});
