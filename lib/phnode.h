#ifndef COMPUTATIONAL_GRAPH_PHNODE_H
#define COMPUTATIONAL_GRAPH_PHNODE_H

#include <string>
#include "Tensor.h"
#include "node.h"
using namespace std;
template <typename _T>
class PHNode : public Node<_T>
{
private:
    static const std::string ErrMsg; //需要输出的错误信息，设定为静态
public:
    using Node<_T>::Result;
    PHNode(string s) { Node<_T>::setname(s); }
    PHNode(){};
    _T GetVal();
    virtual _T grad(Node<_T> *node);
    _T SetVal(_T);
    using Node<_T>::Clear;
};

template <typename _T>
_T PHNode<_T>::grad(Node<_T> *node)
{ //求导
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
const std::string PHNode<_T>::ErrMsg = "ERROR: Placeholder missing"; //类外定义

template <typename _T>
_T PHNode<_T>::GetVal()
{
    if (!Result)
        throw ErrMsg; //如果未被赋值则出错，抛出错误信息
    return *Result;
}

template <typename _T>
_T PHNode<_T>::SetVal(_T _Val) //在Eval的时候用来预先赋值
{
    if (Result)
        *Result = _Val;
    else
        Result = new _T(_Val);
    return *Result;
}

#endif //COMPUTATIONAL_GRAPH_PHNODE_H
