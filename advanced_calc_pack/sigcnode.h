#ifndef COMPUTATIONAL_GRAPH_SIGCNODE_H
#define COMPUTATIONAL_GRAPH_SIGCNODE_H

#include "../lib/calcnode.h"

template <typename _T>
class SigCNode : public CalcNode<_T>
{
protected:
    _T Calc(); //重载Calc，在这里进行计算

public:
    using CalcNode<_T>::Result;
    using CalcNode<_T>::OperandNum;
    using CalcNode<_T>::Operands;
    using CalcNode<_T>::CalcNode;
    virtual _T grad(Node<_T> *node);
};

template <>
double SigCNode<double>::Calc()
{
    Result = new double(1 / (1 + exp((-1) * Operands[0]->GetVal())));
    return *Result;
}

template <>
Tensor<double> SigCNode<Tensor<double>>::Calc() //特化Tensor类型
{
    Tensor<double> res = Operands[0]->GetVal().sig();
    Result = new Tensor<double>(res);
    return *Result;
}

template <>
double SigCNode<double>::grad(Node<double> *node) //求导
{
    if (this == node)
    {
        return 1;
    }
    else
    {
        double result = double(exp((-1) * Operands[0]->GetVal()) / pow((1 + exp((-1) * Operands[0]->GetVal())), 2) * Operands[0]->grad(node));
        return result;
    }
}

template <>
Tensor<double> SigCNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //特化Tensor类型
{
    if (this == node)
    {
        Tensor<double> result(1);
        return result;
    }
    else
    {
        Tensor<double> temp11(-1);
        Tensor<double> temp21(1);
        Tensor<double> res1 = (Operands[0]->GetVal() * temp11).exp();
        Tensor<double> res2 = ((Operands[0]->GetVal() * temp11 + temp21).exp()).pow(2);
        Tensor<double> res3 = Operands[0]->grad(node);
        Tensor<double> result = res1 / res2 * res3;
        return result;
    }
}
#endif //COMPUTATIONAL_GRAPH_SIGCNODE_H
