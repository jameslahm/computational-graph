#ifndef COMPUTATIONAL_GRAPH_MULCNODE_H
#define COMPUTATIONAL_GRAPH_MULCNODE_H

#include "../lib/calcnode.h"


template <typename _T>
class MulCNode : public CalcNode<_T>
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
double MulCNode<double>::Calc()
{
	Result = new double(Operands[0]->GetVal() * Operands[1]->GetVal());
	return *Result;
}

template <>
double MulCNode<double>::grad(Node<double> *node) //求导
{
	if (this == node)
	{
		return 1;
	}
	else
	{
		double result = double(Operands[0]->grad(node) * Operands[1]->GetVal() + Operands[1]->grad(node) * Operands[0]->GetVal());
		return result;
	}
}

template <>
Tensor<double> MulCNode<Tensor<double>>::Calc() //特化Tensor类型
{
	Tensor<double> res1 = Operands[0]->GetVal();
	Tensor<double> res2 = Operands[1]->GetVal();
	Tensor<double> res = res1 * res2;
	Result = new Tensor<double>(res);
	return *Result;
}

template <>
Tensor<double> MulCNode<Tensor<double>>::grad(Node<Tensor<double>> *node) //特化Tensor类型
{
	if (this == node)
	{
		Tensor<double> result(1);
		return result;
	}
	else
	{
		Tensor<double> res1 = (Operands[1]->GetVal().tranverse()) * Operands[0]->grad(node);
		Tensor<double> temp = Operands[0]->GetVal();
		Tensor<double> res2 = temp * Operands[1]->grad(node);
		Tensor<double> result = res1 + res2;
		return result;
	}
}

#endif //COMPUTATIONAL_GRAPH_MULCNODE_H