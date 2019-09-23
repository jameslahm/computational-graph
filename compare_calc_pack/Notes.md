# 派生类包 : 比较计算节点 

## 基本信息

- 文件夹名： `compare_calc_pack`
- 头文件名： `compare_calc_pack.h`

<br/>

## 概述

包含一个三目条件运算节点：

1. `CondNode` 条件运算节点

和五个双目比较节点:

1. `GTCNode` 大于节点
2. `LSCNode` 小于节点
3. `GECNode` 大于等于节点
4. `LECNode` 小于等于节点
5. `EQCNode` 等于节点

和：
1.`BindCNode` Bind运算


<br/>

## 派生类介绍 

### CondNode

`CondNode` 是条件运算节点

实现了成员 `Calc()` ，会先按顺序调用三个操作元的 `GetVal()` 

如果第一个操作元的值大于 $0$ ，则结果为第二个操作元的值，否则结果为第三个操作元的值

结果储存在 `*Result` 中并返回

只要操作元求值时存在异常，就会被该函数继续抛出，即使最终不会选用该分支的结果

如果存在多个异常，仅会抛出最早遇到的异常

实现了成员 `grad(Node<_T>* node)` ，直接返回默认值0，并输出该节点为CondNode，不可进行求导

### GTCNode

`GTCNode` 是大于节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值用 `>` 进行逻辑运算，为真结果为 $1$ ，为假结果为 $0$ ，结果储存在 `*Result` 中并返回(若为Tensor类型，则进行相应的>运算)

实现了成员 `grad(Node<_T>* node)` ，直接返回默认值0，并输出该节点为GTCNode，不可进行求导

### LSCNode

`LSCNode` 是小于节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值用 `<` 进行逻辑运算，为真结果为 $1$ ，为假结果为 $0$ ，结果储存在 `*Result` 中并返回(若为Tensor类型，则进行相应的<运算)

实现了成员 `grad(Node<_T>* node)` ，直接返回默认值0，并输出该节点为LSCNode，不可进行求导

### GeCNode

`GECNode` 是大于等于节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值用 `>=` 进行逻辑运算，为真结果为 $1$ ，为假结果为 $0$ ，结果储存在 `*Result` 中并返回(若为Tensor类型，则进行相应的>=运算)

实现了成员 `grad(Node<_T>* node)` ，直接返回默认值0，并输出该节点为GeCNode，不可进行求导

### LECNode

`LECNode` 是小于等于节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值用 `<=` 进行逻辑运算，为真结果为 $1$ ，为假结果为 $0$ ，结果储存在 `*Result` 中并返回(若为Tensor类型，则进行相应的<=运算)

实现了成员 `grad(Node<_T>* node)` ，直接返回默认值0，并输出该节点为LECNode，不可进行求导

### EQCNode

`EQCNode` 是等于节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值用 `==` 进行逻辑运算，为真结果为 $1.0$ ，为假结果为 $0.0$ ，结果储存在 `*Result` 中并返回(若为Tensor类型，则进行相应的==运算)

实现了成员 `grad(Node<_T>* node)` ，直接返回默认值0，并输出该节点为EQCNode，不可进行求导

<br>
### BindNode

`BindNode` 是捆绑运算节点

实现了成员 `Calc()` ，会先按顺序调用两个操作元的 `GetVal()` 

并将所求第一个节点的结果储存在 `*Result` 中并返回

只要操作元求值时存在异常，就会被该函数继续抛出，即使最终不会选用该分支的结果

如果存在多个异常，仅会抛出最早遇到的异常

实现了成员 `grad(Node<_T>* node)` ，直接返回默认值0，并输出该节点为BindNode，不可进行求导

## 使用方法

在主程序中使用 

```c++
#include "compare_calc_pack/compare_calc_pack.h"
```

可包含本扩展包中所有库

使用 `CalcNode` 的构造函数构造

如果你使用 `ComGraph<double>` 构建计算图

可使用其成员函数 `BuildCalcNode<CLASSNAME>(int,vector<string>)` 构建 `CLASSNAME` 类型的节点
