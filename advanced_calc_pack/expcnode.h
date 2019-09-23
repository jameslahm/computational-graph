#ifndef COMPUTATIONAL_GRAPH_EXPCNODE_H
#define COMPUTATIONAL_GRAPH_EXPCNODE_H

#include "../lib/calcnode.h"

#include <vector>
using namespace std;

template <typename _T>
class ExpCNode : public CalcNode<_T>
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
double ExpCNode<double>::Calc()
{
    Result = new double(exp(Operands[0]->GetVal()));
    return *Result;
}

template <>
double ExpCNode<double>::grad(Node<double> *node) //对double类型特化求导
{
    if (this == node)
    {
        return 1;
    }
    else
    {
        double result = double(exp(Operands[0]->GetVal()) * Operands[0]->grad(node));
        return result;
    }
}

template <>
Tensor<double> ExpCNode<Tensor<double>>::Calc() //对Tensor类型特化计算
{
    Tensor<double> res = Operands[0]->GetVal().exp();
    Result = new Tensor<double>(res);
    return *Result;
}

template <>
Tensor<double> ExpCNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //对Tensor类型特化求导
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
        Tensor<double> result = Operands[0]->GetVal().exp() * Operands[0]->grad(node);
        return result;
    }
}

#endif //COMPUTATIONAL_GRAPH_EXPCNODE_H
