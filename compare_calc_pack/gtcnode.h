#ifndef COMPUTATIONAL_GRAPH_GTCNODE_H
#define COMPUTATIONAL_GRAPH_GTCNODE_H

#include "../lib/calcnode.h"

template <typename _T>
class GTCNode : public CalcNode<_T> //比较运算符GTCNode
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
double GTCNode<double>::grad(Node<double> *node)
{ ////GTCNode不可求导，直接返回0，并输出提示信息
    cout << "GTCNode "
         << "NO GRAD" << endl;
    double res = double(0);
    return res;
}
template <>
Tensor<double> GTCNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{ //特化Tensor类型
    cout << "GTCNode "
         << "NO GRAD" << endl;
    Tensor<double> res(0);
    return res;
}
template <>
double GTCNode<double>::Calc()
{
    Result = new double(Operands[0]->GetVal() > Operands[1]->GetVal());
    return *Result;
}

template <>
Tensor<double> GTCNode<Tensor<double>>::Calc() //特化Tensor类型
{
    Result = new Tensor<double>(Operands[0]->GetVal() > Operands[1]->GetVal());
    return *Result;
}
//这里注意，由于第一阶段只要定义double，所以这个比较节点的Calc只特化double类型
//然后记得加GetVal

#endif //COMPUTATIONAL_GRAPH_GTCNODE_H
