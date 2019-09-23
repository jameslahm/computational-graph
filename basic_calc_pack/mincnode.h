#ifndef COMPUTATIONAL_GRAPH_MINCNODE_H
#define COMPUTATIONAL_GRAPH_MINCNODE_H

#include "../lib/calcnode.h"


template <typename _T>
class MinCNode : public CalcNode<_T>
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
double MinCNode<double>::Calc()
{
    Result = new double(Operands[0]->GetVal() - Operands[1]->GetVal());
    return *Result;
}

template <>
double MinCNode<double>::grad(Node<double> *node) //求导
{
    if (this == node)
    {
        return 1;
    }
    else
    {
        double result = double(Operands[0]->grad(node) - Operands[1]->grad(node));
        return result;
    }
}

template <>
Tensor<double> MinCNode<Tensor<double>>::Calc() //特化Tensor类型
{
    Tensor<double> res = Operands[0]->GetVal() - Operands[1]->GetVal();
    Result = new Tensor<double>(res);
    return *Result;
}

template <>
Tensor<double> MinCNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //特化Tensor类型
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
        Tensor<double> result = Operands[0]->grad(node) - Operands[1]->grad(node);
        return result;
    }
}

#endif //COMPUTATIONAL_GRAPH_MINCNODE_H
