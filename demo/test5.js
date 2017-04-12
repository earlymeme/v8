function a() {
    var len = 0;
    for (var i in animations) if (animations.hasOwnProperty(i)) {
        var a = animations[i],
            b = a.get(),
            res;
        len++;
        a.s = (b - a.b) / (a.dur / a.spd);
        if (a.s >= 1) {
            delete animations[i];
            a.s = 1;
            len--;
            (function (a) {
                setTimeout(function () {
                    eve("mina.finish." + a.id, a);
                });
            }(a));
        }
        a.update();
    }
    len && requestAnimFrame(frame);
}