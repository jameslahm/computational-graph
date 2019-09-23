#ifndef COMPUTATIONAL_GRAPH_AsserTNode_H
#define COMPUTATIONAL_GRAPH_AsserTNode_H

#include <iostream>
#include "node.h"
#include "Tensor.h"

template <typename _T>
class AsserTNode : public Node<_T>
{
protected:
    std::string AssertName;
    Node<_T> *WatchNode; //要Assert的节点
    static const std::string ErrMsg;

    _T Assert();

public:
    using Node<_T>::Result;

    explicit AsserTNode(std::string _NtoAName, Node<_T> *_NtoA, std::string s)
        : AssertName(_NtoAName), WatchNode(_NtoA) { Node<_T>::setname(s); };

    _T GetVal();
    virtual _T grad(Node<_T> *node);
    void Clear();
};

template <typename _T>
const std::string AsserTNode<_T>::ErrMsg = "ERROR: Assertion failed ";

template <>
double AsserTNode<double>::Assert() //Assert的double特化版本
{
    Result = new double(0); //Assert节点的值始终为0
    double temp = WatchNode->GetVal();
    if (temp <= 0)
    { //如果被Assert的节点小于0，抛出错误
        throw ErrMsg;
    }
    return *Result;
}
template <>
Tensor<double> AsserTNode<Tensor<double>>::Assert() //Assert的Tensor<double>特化版本
{
    Result = new Tensor<double>(0);
    Tensor<double> temp = WatchNode->GetVal();
    if ((temp <= 0).lezero())
    {
        throw ErrMsg;
    }
    return *Result;
}
template <typename _T>
_T AsserTNode<_T>::GetVal()
{
    if (!Result)
        Assert();
    return *Result;
}

template <>
double AsserTNode<double>::grad(Node<double> *node)
{ //Assert节点严格意义上没有导数，输出提示并返回0
    cout << "AsserTNode "
         << "NO GRAD" << endl;
    double res = double(0);
    return res;
}
template <>
Tensor<double> AsserTNode<Tensor<double>>::grad(Node<Tensor<double>> *node)
{ //Assert节点严格意义上没有导数，输出提示并返回0
    cout << "AsserTNode "
         << "NO GRAD" << endl;
    Tensor<double> res(0);
    return res;
}
template <typename _T>
void AsserTNode<_T>::Clear()
{
    WatchNode->Clear();
    Node<_T>::Clear();
}

#endif //COMPUTATIONAL_GRAPH_PRINODE_H
