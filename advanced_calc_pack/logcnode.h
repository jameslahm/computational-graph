#ifndef COMPUTATIONAL_GRAPH_LOGCNODE_H
#define COMPUTATIONAL_GRAPH_LOGCNODE_H

#include <string>
#include "../lib/calcnode.h"

template <typename _T>
class LogCNode : public CalcNode<_T>
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
const std::string LogCNode<_T>::ErrMsg = "ERROR: LOG operator's input must be positive";

template <>
double LogCNode<double>::Calc()
{
    double Res0 = Operands[0]->GetVal();
    if (Res0 <= 0)
        throw ErrMsg; //对非正数取对数
    Result = new double(log(Res0));
    return *Result;
}

template <>
double LogCNode<double>::grad(Node<double> *node) //特化double类型求导
{
    if (this == node)
    {
        return 1;
    }
    else
    {
        double Res0 = Operands[0]->GetVal();
        if (Res0 <= 0)
            throw ErrMsg; //对非正数取对数
        double result = double(1 / Res0 * Operands[0]->grad(node));
        return result;
    }
}

template <>
Tensor<double> LogCNode<Tensor<double>>::Calc() //特化Tensor类型计算
{
    Tensor<double> res = Operands[0]->GetVal().log();
    Result = new Tensor<double>(res);
    return *Result;
}

template <>
Tensor<double> LogCNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //特化Tensor类型求导
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
        Tensor<double> Res0 = Operands[0]->GetVal();
        vector<vector<double>> temp;
        temp.push_back(vector<double>(1));
        Tensor<double> temp1(temp);
        Tensor<double> result = temp1 / Res0 * Operands[0]->grad(node);
        return result;
    }
}

#endif //COMPUTATIONAL_GRAPH_LOGCNODE_H
