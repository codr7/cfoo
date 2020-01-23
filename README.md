### setup
[cfoo](https://github.com/codr7/cfoo) requires a C11 compiler, CMake and [libcodr7](https://github.com/codr7/libcodr7) to build.

```
$ git clone https://github.com/codr7/cfoo.git
$ cd cfoo
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
$ valgrind ./tests
$ ./cfoo
cfoo v0.1.1

Press Return in empty row to evaluate.

  42

[42]
```

### syntax
By default, arguments are expected to appear before function calls just like in Forth.

```
  1 2 +

[3]
```

Prefix/infix notation may be activated by enclosing trailing arguments in parens.

```
  *(1 +(2) 3)

[9]
```

### the stack
Literals, references to constants and variables, and method results are pushed on the stack.

```
  1 2 3

[1 2 3]
```

`_` may be used to pop the stack.

```
  1 2 3 _

[1 2]
```

### identity & equality
`=` returns true if the specified values are equal.

```
  =([1 2 3] [1 2 3])

[T]
```

While `==` only returns true if they live at the same address. 

```
  ==([1 2 3] [1 2 3])

[F]
```

### license
[MIT](https://github.com/codr7/cfoo/blob/master/LICENSE.txt)

### support
Please consider a donation if you would like to support the project, every contribution helps.

<a href="https://liberapay.com/codr7/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a>