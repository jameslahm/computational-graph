#ifndef COMPUTATIONAL_GRAPH_RELUCNODE_H
#define COMPUTATIONAL_GRAPH_RELUCNODE_H

#include "../lib/calcnode.h"


template <typename _T>
class ReluCNode : public CalcNode<_T>
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
double ReluCNode<double>::Calc() //relu节点计算
{
    Result = new double(Operands[0]->GetVal());
    if (*Result < 0)
        Result = new double(0);
    return *Result;
}

template <>
Tensor<double> ReluCNode<Tensor<double>>::Calc()
{ //特化Tensor类型
    Result = new Tensor<double>(Operands[0]->GetVal());
    vector<vector<double>> &data = Result->getVector();
    for (int i = 0; i < Result->getshape0(); i++)
    {
        for (int j = 0; j < Result->getshape1(); j++)
        {
            if (data[i][j] <= 0)
                data[i][j] = 0;
        }
    }
    return *Result;
}

template <>
double ReluCNode<double>::grad(Node<double> *node)
{ //relu节点求导
    double value = Operands[0]->GetVal();
    if (value <= 0)
    {
        double res = double(0);
        return res;
    }
    else
    {
        if (GetVal() <= 0)
        {
            double res = double(0);
            return res;
        }
        else
        {
            double res = double(Operands[0]->grad(node));
            return res;
        }
    }
}

template <>
Tensor<double> ReluCNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{ //特化Tensor类型
    Tensor<double> value = Operands[0]->GetVal();
    vector<vector<double>> temp;
    Tensor<double> *res = new Tensor<double>(Operands[0]->grad(node));
    vector<vector<double>> &resdata = res->getVector();
    vector<vector<double>> &data = value.getVector();
    for (int i = 0; i < value.getshape0(); i++)
    {
        vector<double> tempI;
        for (int j = 0; j < value.getshape1(); j++)
        {
            if (data[i][j] < 0)
                tempI.push_back(0);
            else
                tempI.push_back(1);
        }
        temp.push_back(tempI);
    }
    Tensor<double> final = Tensor<double>(temp) * (*res);
    return final;
}

#endif
