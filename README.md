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

  1 2 3
  
[1 2 3]

  ___
  
[]

  1 ==(2)
  
[F]

  _ 1 !==(2)
  
[T]

  _ now
  
[Time(2020-01-22 20:22:54)]
```

### license
[MIT](https://github.com/codr7/cfoo/blob/master/LICENSE.txt)

### support
Please consider a donation if you would like to support the project, every contribution helps.

<a href="https://liberapay.com/codr7/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a>