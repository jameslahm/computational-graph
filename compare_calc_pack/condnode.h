#ifndef COMPUTATIONAL_GRAPH_CONDNODE_H
#define COMPUTATIONAL_GRAPH_CONDNODE_H

#include "../lib/calcnode.h"

template <typename _T>
class CondNode : public CalcNode<_T>
{
protected:
    _T Calc(); //重载Calc
public:
    using CalcNode<_T>::Result;
    using CalcNode<_T>::OperandNum;
    using CalcNode<_T>::Operands;
    using CalcNode<_T>::CalcNode;
    virtual _T grad(Node<_T> *node);
};

template <>
double CondNode<double>::Calc()
{
    double Cond = Operands[0]->GetVal();
    double Res1 = Operands[1]->GetVal();
    double Res2 = Operands[2]->GetVal();
    //这样可以检查未被选择的操作元计算可能出现的错误
    Result = new double(Cond > 0 ? Res1 : Res2);
    return *Result;
}

template <>
double CondNode<double>::grad(Node<double> *node)
{ //CondNode无导数，直接返回0，并输出提示信息
    cout << "CondNode "
         << "NO GRAD" << endl;
    double res = double(0);
    return res;
}
template <>
Tensor<double> CondNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{ //特化Tensor类型
    cout << "CondNode "
         << "NO GRAD" << endl;
    Tensor<double> res(0);
    return res;
}
template <>
Tensor<double> CondNode<Tensor<double>>::Calc()
{
    Tensor<double> Cond = Operands[0]->GetVal();
    Tensor<double> Res1 = Operands[1]->GetVal();
    Tensor<double> Res2 = Operands[2]->GetVal();
    bool temp = Cond.gtzero();
    Result = new Tensor<double>(temp ? Res1 : Res2);
    return *Result;
}

#endif //COMPUTATIONAL_GRAPH_CONDNODE_H
