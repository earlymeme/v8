function Person(name, age) {
  this.name = name;
  this.age = age;
}

print("before");
const p = new Person("Daniel", 41);
print(p.name);
print(p.age);
print("after");