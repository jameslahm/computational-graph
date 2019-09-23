#ifndef COMPUTATIONAL_GRAPH_TANHCNODE_H
#define COMPUTATIONAL_GRAPH_TANHCNODE_H

#include "../lib/calcnode.h"


template <typename _T>
class TanhCNode : public CalcNode<_T>
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
double TanhCNode<double>::Calc()
{
    Result = new double(tanh(Operands[0]->GetVal()));
    return *Result;
}

template <>
Tensor<double> TanhCNode<Tensor<double>>::Calc() //特化Tensor类型
{
    Tensor<double> res = Operands[0]->GetVal().tanh();
    Result = new Tensor<double>(res);
    return *Result;
}

template <>
double TanhCNode<double>::grad(Node<double> *node) //求导
{
    if (this == node)
    {
        return 1;
    }
    else
    {
        double result = double(4 * exp(2 * Operands[0]->GetVal()) / pow(exp(2 * Operands[0]->GetVal()) + 1, 2) * Operands[0]->grad(node));
        return result;
    }
}

template <>
Tensor<double> TanhCNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //特化Tensor类型
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
        vector<vector<double>> temp1;
        temp1.push_back(vector<double>(2));
        Tensor<double> temp11(temp1);
        vector<vector<double>> temp2;
        temp1.push_back(vector<double>(4));
        Tensor<double> temp21(temp2);
        vector<vector<double>> temp3;
        temp1.push_back(vector<double>(1));
        Tensor<double> temp31(temp3);
        Tensor<double> result = temp21 * (temp11 * Operands[0]->GetVal().exp()) / ((temp11 * Operands[0]->GetVal()).exp() + temp31).pow(2) * Operands[0]->grad(node);
        return result;
    }
}

#endif //COMPUTATIONAL_GRAPH_TANHCNODE_H
