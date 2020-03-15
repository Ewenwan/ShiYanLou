# LLVM源码分析

[LLVM  CMU 教案 ](www.cs.cmu.edu/afs/cs.cmu.edu/academic/class/15745-s14/public/lectures/)

[深入剖析-iOS-编译-Clang---LLVM](https://github.com/ming1016/study/wiki/深入剖析-iOS-编译-Clang---LLVM)

[LLVM_proj](https://github.com/Ewenwan/llvm-project/blob/master/README.md)

[LLVM编程索引](http://llvm.org/docs/ProgrammersManual.html)

[llvm源码浏览带跳转](http://androidxref.com/4.2.2_r1/xref/external/llvm/)

[llvm-clang-samples ](https://github.com/Ewenwan/llvm-clang-samples/tree/master/src_clang)

[llvm-tutor  A collection of LLVM passes (with tests and build scripts)](https://github.com/Ewenwan/llvm-tutor)

## LLVM开发者手册

[英文参考](http://llvm.org/docs/ProgrammersManual.html)

[中文翻译](https://blog.csdn.net/qq_23599965/article/details/88538590?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)

### 重要且有用的LLVM APIs  函数

> cast类型相关

[源码](http://llvm.org/doxygen/Casting_8h_source.html)

**1. isa<>：  isa<具体类型>(变量名)  判断该变量是否是某个具体类型**

isa<>操作符的工作原理与Java “instanceof”操作符完全相同。它返回true或false，这取决于引用或指针是否指向指定类的实例。这对于各种类型的约束检查非常有用

**2.cast<>： 已知类型的强制转换**

cast<>操作符是一个“已检查的强制转换”操作。它将指针或引用从基类转换为派生类，如果不是正确类型的实例，则会导致断言失败。当你有一些信息使你相信某样东西是正确的类型时，应该使用这种方法。isa<> 和 cast<> 模板的一个例子是：

```c
static bool isLoopInvariant(const Value *V, const Loop *L) {
  if (isa<Constant>(V) || isa<Argument>(V) || isa<GlobalValue>(V))  // V是常量/参数/全局变量?
    return true;

  // Otherwise, it must be an instruction...  否则V是指令
  return !L->contains(cast<Instruction>(V)->getParent());  // 将基类Value类型的V转换cast成派生类类型的 Instruction
}
```
**3.dyn_cast<>：未知类型的强制转换**

dyn_cast<>操作符是一个“检查转换”操作。它检查操作数是否属于指定的类型，如果是，则返回指向它的指针(该操作符不与引用一起工作)。如果操作数类型不正确，则返回空指针。因此，这与c++中的dynamic_cast<>操作符非常相似，应该在相同的环境中使用。通常，dyn_cast<>操作符用于if语句或其他类似的流控制语句中：
```c
if (auto *AI = dyn_cast<AllocationInst>(Val)) { // 如果Val可以转换成 AllocationInst类型则if语句成立，否则返回NULL，if语句不成立
  // ...
}
```
这种形式的if语句有效地将对 isa<> 的调用和对 cast<> 的调用组合到一个语句中，这非常方便。
注意，dyn_cast<>操作符可以被滥用，就像c++的dynamic_cast<>或Java的instanceof操作符一样。特别是，不应该使用大的if/then/else块来检查类的许多不同变体。如果您发现自己想这样做，那么使用 InstVisitor 类直接分派指令类型会更清晰、更有效。


**4.cast_or_null<>：**

cast_or_null<>操作符的工作原理与 cast<>操作符类似，只是它允许一个空指针作为参数(然后将其传播)。这有时很有用，允许您将多个null检查合并到一个检查中。

**5.dyn_cast_or_null<>：**

dyn_cast_or_null<>操作符的工作原理与 dyn_cast<> 操作符类似，只是它允许一个空指针作为参数(然后将其传播)。这有时很有用，允许您将多个null检查合并到一个检查中。


> **传递字符串(StringRef和Twine类)**

虽然LLVM通常不做太多字符串操作，但是我们有几个重要的APIs接受字符串。两个重要的例子是 Value 类（它有指令、函数等的名称）和 StringMap 类（在 LLVM 和 Clang 中广泛使用）。

这些是泛型类，它们需要能够接受可能包含空字符的字符串。因此，它们不能简单地接受const char *，而接受const std::string&要求客户机执行堆分配，这通常是不必要的。代替的是，许多LLVM APIs使用StringRef或const twine&来有效地传递字符串。

**1.StringRef类**

StringRef数据类型表示对常量字符串（一个字符数组和一个长度）的引用，并支持std::string上可用的公共操作，但不需要堆分配。
它可以使用一个C风格的以null结尾的字符串、一个std::string隐式地被造，也可以使用一个字符指针和长度显式地构造。例如，StringRef find函数声明为：

 iterator find(StringRef Key);

client可以用以下任意一种方式调用这个函数：Map是  StringMap 类对象
```c
Map.find("foo");                 // Lookup "foo"    C风格的以null结尾的字符串构造 StringRef 
Map.find(std::string("bar"));    // Lookup "bar"    C风格的以null结尾的字符串构造 std::string 再构造成 StringRef 
Map.find(StringRef("\0baz", 4)); // Lookup "\0baz"  一个字符指针 和 长度   显式地构造 StringRef 
```

类似地，需要返回string的APIs可能会返回一个StringRef实例，该实例可以直接使用，也可以使用str成员函数将其转换为std::string。有关更多信息，请查看 [llvm/ADT/StringRef.h](http://llvm.org/doxygen/StringRef_8h_source.html)

您应该很少直接使用StringRef类，因为它包含指向外部内存的指针，所以存储该类的实例通常是不安全的（除非您知道不会释放外部存储）。StringRef在 LLVM 中足够小和普遍，因此它应该总是通过值传递。

**2. **

