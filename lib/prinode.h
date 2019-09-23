#ifndef COMPUTATIONAL_GRAPH_PRINODE_H
#define COMPUTATIONAL_GRAPH_PRINODE_H

#include <iostream>
#include "node.h"
#include "Tensor.h"
using namespace std;
template <typename _T>
class PriNode : public Node<_T>
{
protected:
    std::string WatchName; //需要被输出的节点名称
    Node<_T> *WatchNode;   //需要被输出的节点
    std::ostream &OUTPUT;

    _T Print();

public:
    using Node<_T>::Result;

    explicit PriNode(std::string _NtoWName, Node<_T> *_NtoW, std::ostream &_OSTR, string s)
        : WatchName(_NtoWName), WatchNode(_NtoW), OUTPUT(_OSTR) { Node<_T>::setname(s); };

    _T GetVal();

    virtual _T grad(Node<_T> *node);

    void Clear();
};

template <typename _T>
_T PriNode<_T>::grad(Node<_T> *node)
{ //PriNode不可求导，直接返回0，并输出提示信息
    cout << "PriNode "
         << "NO GRAD" << endl;
    return _T(0);
}

// template<>
// Tensor<double> PriNode<Tensor<double>>::grad(string &name)
// {
//     GetVal();
//     vector<vector<double>> temp;
//     temp.push_back(vector<double>(0));
//     Tensor<double>* result=new Tensor<double>(temp);
//     return *result;
// }

template <typename _T>
_T PriNode<_T>::Print()
{
    Result = new _T(WatchNode->GetVal());
    OUTPUT << "PRINT operator: " << WatchName << " = " << *Result << std::endl; //输出观察结果
    return *Result;
}
template <>
Tensor<double> PriNode<Tensor<double>>::Print()
{
    Result = new Tensor<double>(WatchNode->GetVal());
    OUTPUT << "PRINT operator: " << WatchName << " = " << std::endl;
    for (int i = 0; i < Result->getshape0(); i++)
    {
        for (int j = 0; j < Result->getshape1(); j++)
        {
            OUTPUT << (*Result)[i][j] << " ";
        }
        OUTPUT << endl;
    }
    return *Result;
}
template <typename _T>
_T PriNode<_T>::GetVal()
{
    if (!Result)
        Print();
    return *Result;
}

template <typename _T>
void PriNode<_T>::Clear()
{
    WatchNode->Clear();
    Node<_T>::Clear();
}

#endif //COMPUTATIONAL_GRAPH_PRINODE_H
