#ifndef COMPUTATIONAL_GRAPH_SINCNODE_H
#define COMPUTATIONAL_GRAPH_SINCNODE_H

#include "../lib/calcnode.h"

template <typename _T>
class SinCNode : public CalcNode<_T>
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
double SinCNode<double>::Calc()
{
    Result = new double(sin(Operands[0]->GetVal()));
    return *Result;
}

template <>
Tensor<double> SinCNode<Tensor<double>>::Calc() //特化Tensor类型
{
    Tensor<double> res = Operands[0]->GetVal().sin();
    Result = new Tensor<double>(res);
    return *Result;
}

template <>
double SinCNode<double>::grad(Node<double> *node) //求导
{
    if (this == node)
    {
        return 1;
    }
    else
    {
        double result = double(cos(Operands[0]->GetVal()) * Operands[0]->grad(node));
        return result;
    }
}

template <>
Tensor<double> SinCNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //特化Tensor类型
{
    if (this == node)
    {
        vector<vector<double>> temp;
        temp.push_back(vector<double>(1));
        Tensor<double> result(temp);
        return result;
    }
    else
    {
        Tensor<double> result = Operands[0]->GetVal().cos() * Operands[0]->grad(node);
        return result;
    }
}

#endif //COMPUTATIONAL_GRAPH_SINCNODE_H
