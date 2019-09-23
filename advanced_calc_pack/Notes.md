# 派生类包 : 高级计算节点

## 基本信息

- 文件夹名： `advanced_calc_pack`
- 头文件名： `advanced_calc_pack.h`

<br/>

## 概述

包含五个单目计算节点：

1. `ExpCNode` 指数运算节点
2. `LogCNode` 对数运算节点
3. `SinCNode` 正弦节点
4. `TanhCNode` 双曲正切节点
5. `SigCNode`  $sigmoid$ 函数计算节点
6. `AssignCNode` $Assign$运算节点
7. `GradNode` 求导节点( L = GRAD A 中的L )
8. `GradCNode` 具体求导运算节点( Lx = L AT x 中的Lx )
9. `ReluCNode` $relu$ 激活函数计算节点
10. `SoftmaxCNode` $softmax$ 激活函数计算节点

<br/>

## 派生类介绍

### ExpCNode

`ExpCNode` 是指数运算节点

实现了成员 `Calc()` ，将操作元的 `GetVal()` 返回值取指数，储存在 `*Result` 中并返回

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

### LogCNode

`LogCNode` 是对数运算节点

实现了成员 `Calc()` ，将操作元的 `GetVal()` 返回值取自然对数，储存在 `*Result` 中并返回

取对数的操作元必须为正数，如果不满足该条件，会抛出 `std::string` 类异常 `"ERROR: LOG operator's input must be positive"`

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

### SinCNode

`SinCNode` 是正弦节点

实现了成员 `Calc()` ，将操作元的 `GetVal()` 返回值取正弦，储存在 `*Result` 中并返回

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

### TanhCNode

`TanhCNode` 是双曲正切节点

实现了成员 `Calc()` ，将操作元的 `GetVal()` 返回值取双曲正切，储存在 `*Result` 中并返回

双曲正切函数计算公式为：

$$\tanh(x)=\frac{e^x-e^{-x}}{e^x+e^{-x}}$$

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

### SigCNode

`SigCNode` 是 $\mathrm{sigmoid}$ 函数计算节点

实现了成员 `Calc()` ，将操作元的 `GetVal()` 返回值代入$\mathrm{sigmoid}$函数，储存在 `*Result` 中并返回

$sigmoid$ 函数计算公式为：

$$sigmoid(x)=\frac{1}{1+e^{-x}}$$

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

### AssignCNode

`AssignNode` 是进行Assign运算的计算节点子类

实现了成员`grad(Node<_T>* node)` ，返回默认值0，并输出提示 AssignNode没有导数，不可求导。

实现了成员`Calc()`，将AssignNode节点中存储的第二个变量节点的值赋给Assign节点。

实现了成员`Clear()`，在一次Eval结束后，Assign节点的Clear()函数被调用，将第二个变量节点的值赋给第一个变量节点，以保证Assign运算不在当前Eval中生效，并清空Assign节点的值。
### GradNode
`GradNode` 是求导运算中建立的求导节点

实现了成员`grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果

### GradCNode
`GradCNode` 是求导具体计算的节点

实现了成员 `Calc()` ，调用第一个操作元的`grad(Node<_T>* node)` ，node参数为第二个操作元，返回值储存在 `*Result` 中并返回

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果，该函数用于实现求任意高阶导以及偏导。

### ReluCNode
`ReluCNode` 是relu激活函数计算节点

实现了成员 `Calc()` ，调用第一个操作元的`GetVal()` ，对返回值取relu激活函数后储存在 `*Result` 中并返回

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果。

### SoftmaxCNode
`SoftmaxCNode` 是softmax激活函数计算节点

实现了成员 `Calc()` ，调用第一个操作元的`GetVal()` ，对返回值取softmax激活函数后储存在 `*Result` 中并返回

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果。

<br/>

## 使用方法

在主程序中使用 

```c++
#include "advanced_calc_pack/advanced_calc_pack.h"
```

可包含本扩展包中所有库

使用 `CalcNode` 的构造函数构造

如果你使用 `ComGraph<double>` 构建计算图

可使用其成员函数 `BuildCalcNode<CLASSNAME>(int,vector<string>)` 构建 `CLASSNAME` 类型的节点
