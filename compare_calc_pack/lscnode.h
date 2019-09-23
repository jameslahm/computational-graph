#ifndef COMPUTATIONAL_GRAPH_LSCNODE_H
#define COMPUTATIONAL_GRAPH_LSCNODE_H

#include "../lib/calcnode.h"

template <typename _T>
class LSCNode : public CalcNode<_T> //比较运算符LSCNode
{
protected:
    _T Calc(); //重载Calc，在这里进行计算
public:
    using CalcNode<_T>::Result;
    using CalcNode<_T>::OperandNum;
    using CalcNode<_T>::Operands; //Using 基类的操作元
    using CalcNode<_T>::CalcNode;
    virtual _T grad(Node<_T> *node);
};

template <>
double LSCNode<double>::grad(Node<double> *node)
{ ////LSCNode不可求导，直接返回0，并输出提示信息
    cout << "LSCNode "
         << "NO GRAD" << endl;
    double res = double(0);
    return res;
}
template <>
Tensor<double> LSCNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{ //特化Tensor类型
    cout << "LSCNode "
         << "NO GRAD" << endl;
    Tensor<double> res(0);
    return res;
}
template <>
double LSCNode<double>::Calc()
{
    Result = new double(Operands[0]->GetVal() < Operands[1]->GetVal());
    return *Result;
}

template <>
Tensor<double> LSCNode<Tensor<double>>::Calc() //特化Tensor类型
{
    Result = new Tensor<double>(Operands[0]->GetVal() < Operands[1]->GetVal());
    return *Result;
}
#endif //COMPUTATIONAL_GRAPH_LSCNODE_H
