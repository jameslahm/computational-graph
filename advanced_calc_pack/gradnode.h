#ifndef COMPUTATIONAL_GRAPH_GRADNODE_H
#define COMPUTATIONAL_GRAPH_GRADNODE_H

#include "../lib/calcnode.h"

using namespace std;
template <typename _T>

class GradNode : public CalcNode<_T>
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

template <typename _T>
_T GradNode<_T>::Calc()
{
    return _T(0);
}

template <>
double GradNode<double>::grad(Node<double> *node)
{ //对node求导
    double result = double(Operands[0]->grad(node));
    return result;
}

template <>
Tensor<double> GradNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{ //特化Tensor类型
    Tensor<double> res = Operands[0]->grad(node);
    return res;
}

#endif