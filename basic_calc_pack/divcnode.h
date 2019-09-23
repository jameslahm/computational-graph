#ifndef COMPUTATIONAL_GRAPH_DIVCNODE_H
#define COMPUTATIONAL_GRAPH_DIVCNODE_H

#include <string>
#include "../lib/calcnode.h"


template <typename _T>
class DivCNode : public CalcNode<_T>
{
private:
    static const std::string ErrMsg; //需要输出的错误信息，设定为静态
protected:
    _T Calc(); //重载Calc，在这里进行计算
public:
    using CalcNode<_T>::Result;
    using CalcNode<_T>::OperandNum;
    using CalcNode<_T>::Operands;
    using CalcNode<_T>::CalcNode;
    virtual _T grad(Node<_T> *node);
};

template <typename _T>
const std::string DivCNode<_T>::ErrMsg = "ERROR: Division by zero";

template <>
double DivCNode<double>::Calc() //存在报错的情况
{
    double Res0 = Operands[0]->GetVal(), Res1 = Operands[1]->GetVal();
    if (Res1 == 0)
        throw ErrMsg; //被0除
    Result = new double(Res0 / Res1);
    return *Result;
}

template <>
Tensor<double> DivCNode<Tensor<double>>::Calc() //特化Tensor类型
{
    Result = new Tensor<double>(Operands[0]->GetVal() / Operands[1]->GetVal());
    return *Result;
}

template <>
double DivCNode<double>::grad(Node<double> *node) //求导
{
    if (this == node)
    {
        return 1;
    }
    else
    {
        double Res0 = Operands[0]->GetVal(), Res1 = Operands[1]->GetVal();
        if (Res1 == 0)
            throw ErrMsg;
        double result = double(1 / Res1 * Operands[0]->grad(node) + (-Res0 / (Res1 * Res1)) * Operands[1]->grad(node));
        return result;
    }
}

template <>
Tensor<double> DivCNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{ //特化Tensor类型
    if (this == node)
    {
        return Tensor<double>(1);
    }
    else
    {
        Tensor<double> Res0 = Operands[0]->GetVal(), Res1 = Operands[1]->GetVal();
        Tensor<double> result(Tensor<double>(1) / Res1 * Operands[0]->grad(node) - (Res0 / (Res1 * Res1)) * Operands[1]->grad(node));
        return result;
    }
}

#endif //COMPUTATIONAL_GRAPH_DIVCNODE_H
