#ifndef COMPUTATIONAL_GRAPH_NODE_H
#define COMPUTATIONAL_GRAPH_NODE_H

#include <string>
#include "Tensor.h"
using namespace std;

template <typename _T>
class Node
{
protected:
    _T *Result;
    string name;

public:
    Node() : Result(nullptr){};

    virtual _T GetVal() = 0; //获取节点的计算结果
    virtual void Clear();    //清除节点的计算结果
    void setname(string s)
    {
        name = s;
    }
    virtual _T SetVal(_T);
    virtual ~Node() //析构时必须释放Result
    {
        Clear();
    }
    string getname()
    {
        return name;
    }
    virtual _T grad(Node<_T> *node);
};

template <typename _T>
_T Node<_T>::grad(Node<_T> *node)
{
    if (this == node)
    {
        return _T(1);
    }
    else
    {
        return _T(0);
    }
}

template <typename _T>
_T Node<_T>::SetVal(_T _Val)
{
    if (Result)
        *Result = _Val;
    else
        Result = new _T(_Val);
    return *Result;
}

template <typename _T>
void Node<_T>::Clear()
{
    if (Result)
        delete Result;
    Result = nullptr;
}

#endif //COMPUTATIONAL_GRAPH_NODE_H
