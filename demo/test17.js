var obj = {};
Object.defineProperty(obj, 'key', {
  __proto__: null, // no inherited properties
  value: 'static'  // not enumerable
                   // not configurable
                   // not writable
                   // as defaults
});
// being explicit
Object.defineProperty(obj, 'key1', {
  enumerable: true,
  configurable: false,
  writable: false,
  value: 'static'
});

/xyz/.test(function(){xyz});