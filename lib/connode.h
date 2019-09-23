#ifndef COMPUTATIONAL_GRAPH_CONNODE_H
#define COMPUTATIONAL_GRAPH_CONNODE_H

#include "node.h"
#include <string>
#include "Tensor.h"
using namespace std;
template <typename _T>
class ConNode : public Node<_T>
{
private:
    const _T *ConVal; //指向常量的指证，用来储存初始化的答案，不可更改
public:
    explicit ConNode(const _T _InitData, string s)
    {
        ConVal = new const _T(_InitData);
        Node<_T>::setname(s);
    } //从常量构造节点

    _T GetVal(); //求值

    void Clear(); //清除（虽然能被调用，但实际上什么都不做）
    virtual _T grad(Node<_T> *node);
    ~ConNode()
    {
        delete ConVal;
        ConVal = nullptr;
    }
};

template <typename _T>
_T ConNode<_T>::GetVal() //直接返回常值
{
    return *ConVal;
}

template <>
double ConNode<double>::grad(Node<double> *node) //求导
{
    if (this == node)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

template <>
Tensor<double> ConNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //特化Tensor类型
{
    if (this == node)
    {
        vector<vector<double>> temp;
        temp.push_back(vector<double>(1));
        Tensor<double> *result = new Tensor<double>(temp);
        return *result;
    }
    else
    {
        vector<vector<double>> temp;
        temp.push_back(vector<double>(0));
        Tensor<double> *result = new Tensor<double>(temp);
        return *result;
    }
}

template <typename _T>
void ConNode<_T>::Clear() {} //清除时什么都不做

#endif //COMPUTATIONAL_GRAPH_CONNODE_H
