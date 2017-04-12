function add(obj) {
    return obj.prop + obj.prop;
}

const length = 1000 * 1000;

const o = { prop: 1 };

for (let i = 0; i < length; i++) {
    add(o);
}

var object = {
    __proto__: {
        _x: 0,
        set x(v) {
            return this._x = v;
        }
    },
    set x(v) {
        super.x = v;
    }
};
object.x = 1;
object._x;
Object.getPrototypeOf(object)._x;