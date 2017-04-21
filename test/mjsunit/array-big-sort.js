var numbers1 = [];
for (var i = 0; i < 10000; i++) {
  numbers1.push(i);
}
var numbers2 = numbers1.slice();

function compareFn1(a, b) {
  return b - a;
}

numbers2.sort(compareFn1);

assertEquals(9999, numbers2[0], '出错了!');