#ifndef COMPUTATIONAL_GRAPH_GRADCNODE_H
#define COMPUTATIONAL_GRAPH_GRADCNODE_H

#include "../lib/calcnode.h"

using namespace std;
template <typename _T>
class GradCNode : public CalcNode<_T>
{
protected:
    _T Calc();

public:
    using CalcNode<_T>::Result;
    using CalcNode<_T>::OperandNum;
    using CalcNode<_T>::Operands;
    using CalcNode<_T>::CalcNode;
    virtual _T grad(Node<_T> *node);
};

template <>
double GradCNode<double>::Calc()
{ //调用第一个操作元的grad()对第二个操作元求导
    Node<double> *node = Operands[1];
    Result = new double(Operands[0]->grad(node));
    return *Result;
}

template <>
Tensor<double> GradCNode<Tensor<double>>::Calc()
{ //特化Tensor类型，与double类似
    Node<Tensor<double>> *node = Operands[1];
    Tensor<double> res = Operands[0]->grad(node);
    Result = new Tensor<double>(res);
    return *Result;
}

template <>
double GradCNode<double>::grad(Node<double> *node)
{ //特化double类型的 求导节点再求导，差分法实现任意高阶导及偏导的计算
    double value = node->GetVal();
    double DELTA = 1E-4; //delta此时取1E-4，可以根据精度更改
    node->SetVal(value + DELTA);
    double d1 = Operands[0]->grad(Operands[1]);
    node->SetVal(value - DELTA);
    double d2 = Operands[0]->grad(Operands[1]);
    node->SetVal(value);
    double result = double((d1 - d2) / (2 * DELTA));
    return result;
}
template <>
Tensor<double> GradCNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{ //特化Tensor类型
    Tensor<double> value = node->GetVal();
    double DELTA = 1E-4;
    Tensor<double> Tdelta(DELTA);
    node->SetVal(value + Tdelta);
    Tensor<double> d1 = GetVal();
    node->SetVal(value - Tdelta);
    Tensor<double> d2 = GetVal();
    Tensor<double> result((d1 - d2) / (2 * DELTA));
    return result;
}

#endif