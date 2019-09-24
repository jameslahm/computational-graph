# 派生类包 : 基本计算节点

## 基本信息

- 文件夹名： `basic_calc_pack`
- 头文件名： `basic_calc_pack.h`

<br/>

## 概述

包含四个双目运算节点

1. `PluCNode` 加法节点
2. `MinCNode` 减法节点
3. `MulCNode` 乘法节点
4. `DivCNode` 除法节点

<br/>

## 派生类介绍

### PluCNode

`PluCNode` 是加法节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值相加，储存在 `*Result` 中并返回

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

### MinCNode

`MinCNode` 是减法节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值相减，储存在 `*Result` 中并返回

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

### MulCNode

`MulCNode` 是乘法节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值相乘，储存在 `*Result` 中并返回

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

### DivCNode

`DivCNode` 是除法节点

实现了成员 `Calc()` ，将两个操作元的 `GetVal()` 返回值相除，储存在 `*Result` 中并返回

除法的第二个操作元不能为0，如果不满足该条件，会抛出 `std::string` 类异常 `"ERROR: Division by zero"`

实现了成员 `grad(Node<_T>* node)` ，对node节点进行导数计算并返回结果 

<br/>

## 使用方法

在主程序中使用 

```c++
#include "basic_calc_pack/basic_calc_pack.h"
```

可包含本扩展包中所有库

使用 `CalcNode` 的构造函数构造

如果你使用 `ComGraph` 构建计算图

可使用其成员函数 `BuildCalcNode<CLASSNAME>(int,vector<string>)` 构建 `CLASSNAME` 类型的节点
