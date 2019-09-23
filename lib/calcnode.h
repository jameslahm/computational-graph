#ifndef COMPUTATIONAL_GRAPH_CALCNODE_H
#define COMPUTATIONAL_GRAPH_CALCNODE_H

#include <string>
#include <vector>
#include <cmath>
#include "node.h"
#include "Tensor.h"
using namespace std;
template <typename _T>
class CalcNode : public Node<_T>
{
protected:
    //protected允许子类访问
    const int OperandNum; //操作数个数
    Node<_T> **Operands;  //操作数，指针的指针，第一层指针作为数组，第二层指针作为内容
    virtual _T Calc() = 0;
    //Calc函数用于计算该节点的答案，不允许从外部调用，只可以从GetVal()调用（因为所有的节点都有GetVal()，而Calc()不是）

public:
    using Node<_T>::Result;

    CalcNode(int _OpeNum, const std::vector<Node<_T> *> &OperandsList, string s) : OperandNum(_OpeNum) //推荐的构造函数
    {
        Node<_T>::setname(s);
        Operands = new Node<_T> *[OperandNum];
        for (int i = 0; i < OperandNum; ++i)
        {
            Operands[i] = OperandsList[i];
        }
    }
    //给出操作元个数，给出操作元列表来初始化一个CalcNode（的派生类），可以有效避免vector后面一不小心加入了多余的东西

    explicit CalcNode(const std::vector<Node<_T> *> &OperandsList, string s) : OperandNum(OperandsList.size())
    {
        Node<_T>::setname(s);
        Operands = new Node<_T> *[OperandNum];
        for (int i = 0; i < OperandNum; ++i)
        {
            Operands[i] = OperandsList[i];
        }
    }
    //仅给出操作元列表来初始化一个CalcNode（的派生类），请确保OperandsList仅仅只含有操作元的指针

    _T GetVal(); //实现Node的虚函数

    void Clear(); //实现Node的虚函数，所有的CalcNode的派生类共用一种清除函数

    ~CalcNode()
    {
        if (Operands)
            delete[] Operands;
        Operands = nullptr;
        //注意，这里不要释放Operands[]所指的东西，谁申请谁释放，这个申请是在构造前完成的，所以不由本类释放
    }

    virtual _T grad(Node<_T> *node);
    // virtual _T SetVal(_T);
};

// template<typename _T>
// _T CalcNode<_T>::SetVal(_T val){
//     Result=new _T(val);
//     return *Result;
// }

template <typename _T>
_T CalcNode<_T>::GetVal()
{
    if (Result)
        return *Result; //如果已经被计算过了，则不重复计算
    Calc();             //调用虚函数Calc()，结果将储存在*Result
    return *Result;
}

template <typename _T>
void CalcNode<_T>::Clear()
{
    for (int i = 0; i < OperandNum; ++i)
        Operands[i]->Clear();
    //递归地调用操作元的清除函数
    Node<_T>::Clear(); //清除自身的答案，回到未计算状态
}

template <typename _T>
_T CalcNode<_T>::grad(Node<_T> *node)
{
    return _T(0);
}

#endif //COMPUTATIONAL_GRAPH_CALCNODE_H
