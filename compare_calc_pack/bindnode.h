#ifndef COMPUTATIONAL_GRAPH_BINDNODE_H
#define COMPUTATIONAL_GRAPH_BINDNODE_H

#include "../lib/calcnode.h"


template <typename _T>
class BindNode : public CalcNode<_T>
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
double BindNode<double>::Calc()
{ //同时计算两个节点的值
    double Bind = Operands[0]->GetVal();
    double Res1 = Operands[1]->GetVal();
    Result = new double(Bind);
    return *Result;
}
template <>
Tensor<double> BindNode<Tensor<double>>::Calc()
{ //同时计算两个节点的值
    Tensor<double> Bind = Operands[0]->GetVal();
    Tensor<double> Res1 = Operands[1]->GetVal();
    Result = new Tensor<double>(Bind);
    return *Result;
}
template <>
double BindNode<double>::grad(Node<double> *node)
{ //Bind节点无导数，输出提示并返回0
    cout << "BindNode "
         << "NO GRAD" << endl;
    double res = double(0);
    return res;
}
template <>
Tensor<double> BindNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{
    cout << "BindNode "
         << "NO GRAD" << endl;
    Tensor<double> res(0);
    return res;
}
#endif
