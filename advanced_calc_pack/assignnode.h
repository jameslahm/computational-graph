#ifndef COMPUTATIONAL_GRAPH_ASSIGNCNODE_H
#define COMPUTATIONAL_GRAPH_ASSIGNCNODE_H
#include "../lib/calcnode.h"

using namespace std;

template <typename _T>
class AssignCNode : public CalcNode<_T>
{
protected:
    _T Calc();

public:
    using CalcNode<_T>::Result;
    using CalcNode<_T>::OperandNum;
    using CalcNode<_T>::Operands;
    using CalcNode<_T>::CalcNode;
    void Clear();
    virtual _T grad(Node<_T> *node);
};

template <>
double AssignCNode<double>::Calc()
{
    Result = new double(Operands[1]->GetVal()); //先将第二个变量的值赋给Assign节点
    return *Result;
}
template <>
Tensor<double> AssignCNode<Tensor<double>>::Calc()
{
    Result = new Tensor<double>(Operands[1]->GetVal()); //先将第二个变量的值赋给Assign节点
    return *Result;
}
template <>
double AssignCNode<double>::grad(Node<double> *node)
{ //Assign节点无导数，输出提示并返回0
    cout << "AssignCNode "
         << "NO GRAD" << endl;
    double res = double(0);
    return res;
}
template <>
Tensor<double> AssignCNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{
    cout << "AssignNode "
         << "NO GRAD" << endl;
    Tensor<double> res(0);
    return res;
}
template <typename _T>
void AssignCNode<_T>::Clear()
{
    for (int i = 0; i < OperandNum; ++i)
        Operands[i]->Clear();                   //变量节点的clear实际为空
    Operands[0]->SetVal(Operands[1]->GetVal()); //将第二个变量的值赋给第一个
    Node<_T>::Clear();
}

#endif
