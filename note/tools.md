# gdb
基于ptrace和SIGTRAP

加断点时，gdb需要把目标地址的代码变成0xcc，这条单字节的指令，是一条产生TRAP的系统调用，最终会变成SIGTRAP

gcc的-g和-O是正交的，-g生成的debugging info没有运行时开销，只会影响file size

gcc的-g，仍会输出一些只有gdb才能会的信息，这些信息可能导致gdb之外的debugger crash

-g和-ggdb的区别：-g生成当前系统原生的debugging format，-ggdb生成gdb喜欢的debugging format，首选dwarf，并生成了很多gdb extension信息

默认的debugging information level是2,可以通过-gLEVEL以及-ggdbLEVEL来指定更高级别，level3会包括macro定义

-Og在O1的基础上，删了一些影响调试的优化pass，在保证一定的优化的前提下，提供良好的调试体验

debian现在用的libc库是glibc的version 2，对于linux来说，这是其采用的第6个版本的libc库;所以debian上有两个package，glibc和libc6，其实是一个东西

## source
info source，可以看到当前pc所处的源码文件的信息，并且也许可以看到当时编译这个文件的编译目录

gdb寻找源码文件的时候，除了寻找原先记录的路径（得是绝对路径，不是相对路径）之后，还会基于source path来寻找。source path默认是$cwd和$cdir，分别是current working dir和compilation dir（如果有记录的话）

directory可用来增加新的source path

另外，substituion rule可将旧的路径替换成新的路径，也用于帮助寻找源码文件

## 看汇编 ?
info line看某一行的汇编

disassemble看一个范围的汇编

## remote debugging / trace point ?


## dprintf
等价于在目标地址处加断点，运行printf，然后再让目标代码继续

## record
这样就可以启用reverse debugging

## trace point
in-process agent

可以在被测试程序中放入一个虚拟机

https://developer.apple.com/library/archive/documentation/DeveloperTools/gdb/gdb/gdb_34.html

## gdb server

## alter execution

## python

c++ pretty print

## command

## gdb script
