function get(/* ...k, j, i */) {
    const a = arguments, l = a.length, n = l - 1
    switch (l) {
        case 0: return this
        case 1: return this[a[n]]
        default: return get.apply(this[a[a.length = n]], a) // a.length = n = a.length - 1
    }
}

print(get.call({x:{y:{z:'!!!'}}}, 'z','y','x')) // => "!!!"