#include <iostream>
#include <sstream>
#include "lib/comgraph.h"
#include "basic_calc_pack/basic_calc_pack.h"
#include "advanced_calc_pack/advanced_calc_pack.h"
#include "compare_calc_pack/compare_calc_pack.h"
#include <string>
using namespace std;
ostream &ErrOut = cout; //默认错误输出流
ostream &PriOut = cout; //默认Print输出流
ostream &AnsOut = cout; //默认答案输出流
double NewtonMethod(ComGraph<double>& G,double* factors,double x,int maxExponent){
	double f0=0; //f(x0)
	double k=0;  //f'(x0)（切线斜率）
	vector<pair<string,double>> PHnode;
	string temp="x";
	for(int i=maxExponent;i>=0;i--){ //按次数依次计算f(x)对应次数的项的值和导数值，累加则得f(x0)和f'(x0)
		if(i==1){ //一次项
			f0+=G.GetNode(temp)->GetVal()*factors[maxExponent-i];
			k+=G.GetNode(temp)->grad(G.GetNode(temp))*factors[maxExponent-i];
		}
		else if(i==0){ //常数项
			f0+=factors[maxExponent];
		}
		else{ //其它次项
			f0+=G.GetNode(to_string(i))->GetVal()*factors[maxExponent-i];
			k+=G.GetNode(to_string(i))->grad(G.GetNode(temp))*factors[maxExponent-i];
		}
	}
	if(maxExponent>=2)
	G.GetNode(to_string(maxExponent))->Clear(); //清楚中间计算节点值
	return x-f0/k; //返回新迭代的x值
}

int main(){
    ComGraph<double> Sample_Graph(ErrOut, PriOut); 
	int m;
    cin>>m;
    string temp;
	getline(cin,temp);
	for(int i=0;i<m;i++){
		string str;
		getline(cin,str);
		stringstream ss(str);
		vector<string> OperandList;
		int maxExponent;
		ss>>maxExponent; //f(x)最高次数
		double* factors= new double[maxExponent+2]; //系数、常数
		for(int j=0;j<=maxExponent+1;j++){
			ss>>factors[j];
		}
		Sample_Graph.BuildVarNode("x",factors[maxExponent+1]);
		OperandList.push_back("x");
		OperandList.push_back("x");
		for(int k=2;k<=maxExponent;k++){ //按次数顺序创立计算节点，名字为对应次数的string形式，如“2”、“3”
			string temp2=to_string(k); 
			Sample_Graph.BuildCalcNode<MulCNode<double>>(temp2,2,OperandList);
			OperandList.pop_back();
			OperandList.push_back(temp2);
		}
		double x=factors[maxExponent+1];
		for(int i=0;i<5;i++){  //迭代五次
			x=NewtonMethod(Sample_Graph,factors,x,maxExponent);
			Sample_Graph.SetVarVal("x",x);
			cout<<x<<" ";
		}
		cout<<endl;
	}
	return 0;
}

