#ifndef COMPUTATIONAL_GRAPH_SOFTMAX_H
#define COMPUTATIONAL_GRAPH_SOFTMAX_H

#include "../lib/calcnode.h"

template <typename _T>
class SoftmaxCNode : public CalcNode<_T>
{
protected:
    _T Calc(); //重载Calc，在这里进行计算

public:
    using CalcNode<_T>::Result;
    using CalcNode<_T>::OperandNum;
    using CalcNode<_T>::Operands;
    using CalcNode<_T>::CalcNode;
    virtual _T grad(Node<_T> *node);
};

template <>
Tensor<double> SoftmaxCNode<Tensor<double>>::Calc() //softmax节点计算会输出各类别概率
{
    vector<vector<double>> res;
    res.push_back(vector<double>());
    double total = 0;
    Tensor<double> res1 = Operands[0]->GetVal();
    for (int i = 0; i < res1.getshape0(); i++)
    {
        total += std::exp(res1.getVector()[i][0]);
    }
    for (int i = 0; i < res1.getshape0(); i++)
    {
        res[0].push_back(std::exp(res1.getVector()[i][0]) / total);
    }
    Tensor<double> final(res);
    final = final.tranverse();
    Result = new Tensor<double>(final);
    return *Result;
}

// template<>
// double SoftmaxCNode<double>::grad(Node<double>* node)
// {
//     if (this==node){
//         return 1;
//     }
//     else{
//         double* result = new double(exp((-1)*Operands[0]->GetVal()) / pow((1 + exp((-1)*Operands[0]->GetVal())),2)*Operands[0]->grad(node));
//         return *result;
//     }
// }

template <>
Tensor<double> SoftmaxCNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //特化Tensor类型求导，该求导即为softmax输出层的分担误差
{
    vector<vector<double>> res;
    res.push_back(vector<double>());
    double total = 0;
    Tensor<double> res1 = Operands[0]->GetVal();
    for (int i = 0; i < res1.getshape0(); i++)
    {
        total += std::exp(res1.getVector()[i][0]);
    }
    for (int i = 0; i < res1.getshape0(); i++)
    {
        res[0].push_back(std::exp(res1.getVector()[i][0]) / total);
    }
    int label = Operands[1]->GetVal().getVector()[0][0];
    Tensor<double> prograd = Operands[0]->grad(node);
    vector<double> result;
    for (int i = 0; i < Operands[0]->GetVal().getshape0(); i++)
    {
        if (i != label)
        {
            result.push_back(-res[0][label] * res[0][i]);
        }
        else
        {
            result.push_back(res[0][label] * (1 - res[0][i]));
        }
        if (res[0][label] < 1E-5) //为防止计算精度问题，出现0，进行修正
            result[i] *= (-1E5);
        else
            result[i] *= (-1 / res[0][label]);
    }
    Tensor<double> result3(result);
    result3 = result3.tranverse();
    Tensor<double> final = result3 * prograd;
    Tensor<double> t(final);
    return t;
}
#endif //COMPUTATIONAL_GRAPH_SIGCNODE_H
