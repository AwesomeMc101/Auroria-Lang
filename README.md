# Auroria-Lang
Lightweight, bytecode-serialized interpreted language. Supporting loops, if/else statements, functions and more.

I'll of course write an explanation on the C++ side when it isn't 4am.

## Everything is buggy, its v0.1!

# Coding in Auroria

### Intro
* Auroria is a scripting language and can work without needing a main function. The
  following code is all that is necessary.

```
print("Hello World!")
```
### Variables
* Variables do not have types and are defined by just typing the following
```
foo = 5
```

### CFunctions
* There is like 3 cfunctions, mostly for testing for when I write a lot more later.
```
print("Hello World!")
sin(90)
parsebool(0) -> returns "false"
```

### AFunctions
* Writing functions is simple and can be done by adding () or inputting arguments (x,y)
```
printsomething() {
print("Hi")
}

printsomething()
```
* With arguments:
```
sum(x, y){
return(x + y)
}

print(sum(1,2))
```

### If Else
* If Else statements are simple and done like this
```
if(1 > 2)
{
}
else{
}
```
* Supported bitwise operators are: > | < | >= | <= | != | == | !
* **If strings are inputted in greater/less than, their lengths will be compared.**

### Loops
* Loops take 2 args (int/string, var) and look like this.
```
i = 0
for(10, "i") {
print(i)
}
```
* And with string iteration, this would print H, e, l, l, o, respectively.
```
i = 0
for("Hello", "i") {
print(i)
}
```

## Much more to come, enjoy scripting :)
- Chris
