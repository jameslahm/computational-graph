#ifndef COMPUTATIONAL_GRAPH_VARNODE_H
#define COMPUTATIONAL_GRAPH_VARNODE_H

#include "phnode.h"
#include <string>
#include "Tensor.h"
using namespace std;
template <typename _T>
class VarNode : public PHNode<_T>
{
private:
    static const std::string ErrMsg; //需要输出的错误信息，设定为静态
public:
    using PHNode<_T>::Result;

    VarNode(string s)
    {
        Result = nullptr;
        Node<_T>::setname(s);
    } //第一种构造方式，建一个空变量

    explicit VarNode(_T _InitVal, string s)
    {
        Result = new _T(_InitVal);
        Node<_T>::setname(s);
    } //第二种构造方式，建立变量的同时赋初值

    _T GetVal(); //如果未被赋值或被强制清除过则抛出错误信息，否则返回*Result
    _T SetVal(_T val);
    void Clear() {}

    void ForcedClear();
    virtual _T grad(Node<_T> *node);
};

template <typename _T>
const std::string VarNode<_T>::ErrMsg = "ERROR: Variable missing";

template <typename _T>
void VarNode<_T>::ForcedClear()
{
    Node<_T>::Clear();
}

template <typename _T>
_T VarNode<_T>::grad(Node<_T> *node)
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
_T VarNode<_T>::GetVal()
{
    if (!Result)
        throw ErrMsg;
    return *Result;
}

template <typename _T>
_T VarNode<_T>::SetVal(_T _Val)
{
    if (Result)
        *Result = _Val;
    else
        Result = new _T(_Val);
    return *Result;
}
#endif //COMPUTATIONAL_GRAPH_VARNODE_H
