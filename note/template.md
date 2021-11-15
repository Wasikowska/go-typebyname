# Name Resolution
## Dependent Name
有以下几种情况（假设template parameter为T）：
1. 在名字中，显式包含了T的qualified name
2. 对于self.name或者this->name这样的表达式，如果self和this的类型依赖于
   T，那么name就成了dependent name
3. func(x, y)或者x + y这样的表达式，如果x或者y的类型依赖于T，那么func
   和operator +就成了dependent name
   
第一种情况是qualified dependent name，第二种和第三种情况是unqualified
dependent name

编译器默认dependent name表示value，如果其实际上是type，就需要加一个
typename前缀，如果其实际上是template，就需要加一个template前缀

## Name Resolution Rule
对于nondepent name，在碰到name的时候resolve一次，使用Ordinary Lookup以
及ADL，ADL的结果优先。nondepent name的resolution，不会考虑Dependent
Base Class

对于Dependent Name， 需要Two Phase Lookup：
- Unqalified Dependent Name（包括） 在遇到时，进行Ordinary Lookup，此时
  Resolution并没有结束
- 在Template Instantiation的过程中，Qualified Dependent Name会进行
  Ordinary Lookup（qualified name不需要考虑ADL），Unqualified Dependent Name进行ADL（如果ADL是可做
  的）。这个过程会考虑Dependent Base Class
  
# Template Instantiation
两个特点：
- On Demand Instantiation
- Lazy Instantiation

## POI
POI表示Instantiate出来的代码，所存放的位置

template function instantiation和template variable instantiation，放在
POI containing namespace的后面，template class instantiation放在POI
containing namespace的前面

### secondary POI
产生的instantiation中，如果有dependent name，那么这些dependent name会
再产生POI，称为secondary POI。

如果secondary POI对应一个template function instantiation，那么放置规则
和primary POI一样，放在secondary POI containing namespace的后面; 如果
对应一个template class instantiation，那么比较特殊，需要放在primary
POI所产生的instantiation的前面

## Inclusion Model
指的是，POI和Template Definition必须在一个translation unit中，不然
instantiation无法发生

inclusion model会影响对template代码的组织

exportion model自C++11开始已经从语言中移除，原先的export关键字，被用在
了module相关的语法中

### Manual Instantiation
在头文件中，只包括template declaration，这样就不用在头文件中暴露
template definition，但template的client code就再也不会出现POI了

所以就需要在一个额外的translation unit中，通过explicit template
instantiation手动提供template instantiation

使用这种方法，需要完全接管instantiation，可能会是个巨大的负担

### Explicit Instantiation Declaration
在explicit instantiation前加上extern，告诉编译器，对于这个特定的
instantiation，在这个translation unit中，只出现declaration，不出现
definition; 同样的，在另一个translation unit中，需要有explicit
template instantiation

但在某些情况下，definition必须出现才行，此时编译器会忽略这个declaration

这种方法只针对特定的template specialization给出declaration的提示，没有
管理上的负担，是个比较好的减少由于greedy instantiation产生的编译时间负
担的方法，标准库中就用这种方法，针对常用的tempalte specialization给出
了explicit instantiation declaration

这是比较有用的特性，可用于减少特定实例的重复代码

## Greedy Instantiation Scheme
这是C++编译器主流实现，遇到POI时，在当前的translation unit中放入实例定
义，这种方法会造成translation unit中出现重复定义，在linking时，把这些
重复定义当作inline definition处理，随机选择一个，和编译器处理普通的
inline function一样; 在随即选择了一个定义后，linker需要修改各个
translation unit中各个定义的指针

# Template Argument Deduction
auto使用的就是这个规则，事实上，auto作为类型推导的关键字，出现在TAD之
后，所以就直接借用了这个规则

假设A是Call Argument Type，P是Template Parameter Type：
1. 如果P是reference type，那么将P调整为referenced type，A保持不变;
   否则，将A调整为decayed type; P = A
2. 如果P是一个复合类型，那么从整体到局部，依次匹配部分; P所拥有的复合
   结构被称为deduced context
3. 在匹配的过程中，A可以按照需要，补充const和volatile修饰

必须存在deduced context，才可以进行推导

有一些特殊的情况，A不是Call Argument Type，例如将函数模板实例的地址赋
值给一个函数指针

备注：简单来说，decay指的是将array type和function type变成pointer type，
并去掉const和volatile修饰

上述第1条中，如果P是reference type，且是forward reference，TAD还有有特
殊的规则，这是为了支持perfect forwarding，具体来说，如果argument是一个
lvalue expression，那么P = A&，如果argument是一个rvalue expression，那
么P = A，不管哪种情况，P都需要经历Reference Collapsing的过程

为免造成过多迷惑，forward reference被严格限定在以下场景：
1. 必须是一个显式的function template
2. 函数参数必须是 template-parameter&&，且template-parameter定义在
   function template中

## initializer list相关的推导
当argument是一个initializer list的时候，由于initializer list没有类型，
一般来说，类型推导是不起作用的，但是：
1. 如果P的类型（去除reference，去除顶层的const和volatile修饰后）是
   std::initializer_list<Deducable_Pattern>，那么initializer list的成
   员类型，会和Deducable_Pattern进行匹配
2. 如果P的类型是一个数组引用，数组的成员类型是一个Deducable_Pattern，
   那么initializer list的成员类型会和Deducable_Pattern进行匹配

# overloading & specialiazation

## overloading

function和function template支持overloading，对function template的
overloading resolution会产生function template POI

在overloading set中进行表决的规则是：
1. 在各种条件都一样的情况下，non-template优于template
2. 两个function template之间，优先more specialized template

# partial specification

specification的顺序很重要，在前面看到的才会被使用

选择使用哪一个specication时，规则和overloading以及argument deduction相
似，在full specification和partial specification之间，优先partial
specification，在specification和general template definition之间，优先
specification。相同优先级之间，选择more specialized的那个

partial specification可以拥有相比primary template更多的template
parameter，并且在一个specification的定义中，可以使用前面出现过的任何
specification

variable specification很奇怪，语言规范中允许，但却没有规定具体的语
法？？？虽然各编译器实现都选择了相同的，符合直觉的语法

# Reference Collapsing Rule
作用于三种情况：
1. template type argument
2. type alias
3. decltype construct

从直观上去理解，这个rule让你可以轻松控制reference kind

在进行collapse时，被reference的type的const和volatile qualifier会被去除

std::forward本质上就是一个static_cast

0常被用做null pointer，但在perfect forwarding的场景中，0无法被完美的传
递给指针类型参数的函数，用nullptr表示null pointer，就不存在这样的问题

## perfect forwarding for return value
decltype(auto) forwardToG(Ts&&... xs)
{
return g(std::forward<Ts>(xs)...); // forward all xs to g()
}

等价于：
auto forwardToG(Ts&&... xs) -> decltype(g(std::forward<Ts>(xs)...))
{
return g(std::forward<Ts>(xs)...); // forward all xs to g()
}

decltype内的表达式，是rvalue expression，所以decltype表达式的type，就
是内部rvalue expression的type，很巧妙

# guaranteed copy elision
https://jonasdevlieghere.com/guaranteed-copy-elision/

http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0135r0.html

将prvalue看成是initializer expression，而不是temp object，是概念上的巨大进步

# std::ref() / std::cref()
底层实现是使用了一个表示reference的封装类型，可以被隐式转成原类型

很好的点子，将reference对象化

# use auto as the function result type
auto always decay !

这样就不用担心，返回引用类型了
