#include "lib/comgraph.h"
#include "basic_calc_pack/basic_calc_pack.h"
#include "advanced_calc_pack/advanced_calc_pack.h"
#include "compare_calc_pack/compare_calc_pack.h"
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<random>
using namespace std;
ostream &ErrOut=cout;
ostream &PriOut=cout;
ostream &AnsOut=cout;
int EPOCHS=1000;//最小二乘法迭代次数
float LEARNING_RATE=0.001;//每次迭代下降的速率
int SHAPE1=0;//自变量的总数量
int SHAPE0=0;//自变量的维度

double MSE(Tensor<double> pred,Tensor<double> Y){//计算预测值与真实值的均方差
    double mse=0;
    mse+=(pred-Y).pow(2).sum();
    mse=mse/pred.getshape0();
    return mse;
}

void LsMethod(vector<vector<double>> &X,vector<vector<double>>&Y){
    SHAPE1=X[0].size();//自变量的维度
    SHAPE0=X.size();
    ComGraph<Tensor<double>> graph(ErrOut,PriOut);
    default_random_engine e;
    vector<std::string> s;
    uniform_real_distribution<double> u(-1.0,1.0);
    graph.BuildConNode("X",Tensor<double>(X));
    vector<vector<double>> W;
    for(int i=0;i<SHAPE1;i++){//生成(-1,1)均匀分布浮点数的W 其中W为SHAPE1维。前SHAPE1-1维为对应相关系数（斜率），后1维为对应偏置（截距）
        double temp=u(e);
        vector<double> WI;
        WI.push_back(temp);
        W.push_back(WI);
    }
    Node<Tensor<double>>* WP = graph.BuildVarNode("W",Tensor<double>(W));
    graph.BuildConNode("Y",Tensor<double>(Y));
    s.push_back("X");
    s.push_back("W");
    graph.BuildCalcNode<MulCNode<Tensor<double>>>("XW",s);//建立XW
    s.clear();
    s.push_back("XW");
    s.push_back("Y");
    graph.BuildCalcNode<MinCNode<Tensor<double>>>("XW_Y",s);//建立XW-Y
    s.clear();
    Tensor<double> XT=Tensor<double>(X).tranverse();
    graph.BuildConNode("XT",XT);//建立XT，为X的转置
    s.push_back("XT");
    s.push_back("XW_Y");
    graph.BuildCalcNode<MulCNode<Tensor<double>>>("GW",s);//建立GW即梯度矩阵，即XT(XW-Y)
    s.clear();
    Tensor<double> param(LEARNING_RATE);//学习率参数
    Tensor<double> other(2.0/SHAPE0);//常数项
    for(int i=0;i<EPOCHS;i++){
        Node<Tensor<double>>* gw=graph.GetNode("GW");
        Tensor<double> res=gw->GetVal();
        res=res*other;
        Tensor<double> L=graph.GetNode("W")->GetVal();
        Tensor<double> R=res*param;
        graph.SetVarVal("W",L-R);//梯度下降
        Tensor<double> pred=graph.GetNode("XW")->GetVal();
        if (i%10==0){
            cout<<"EPOCH:"<<i<<" MSE:"<<MSE(pred,Tensor<double>(Y))<<endl;//每10次EPOCH计算均方差
        }
        gw->Clear();
    }
    Node<Tensor<double>>* w=graph.GetNode("W");
    vector<vector<double>> wdata=w->GetVal().getVector();
    for(int i=0;i<wdata.size();i++){
        for(int j=0;j<wdata[0].size();j++){
            cout<<wdata[i][j]<<" ";
        }
        cout<<"\n";
    }
}

int main(){
    cin>>SHAPE0>>SHAPE1;
    vector<vector<double>> X;//自变量
    vector<vector<double>> Y;//因变量
    for(int i=0;i<SHAPE0;i++){
        vector<double> XI; 
        for(int j=0;j<SHAPE1;j++){
            double temp;
            cin>>temp;
            XI.push_back(temp);
        }
        XI.push_back(1);//为自变量增加一个维度，此维度用来计算偏置（b）
        X.push_back(XI);
    }
    for(int i=0;i<SHAPE0;i++){
        vector<double> YI;
        double temp;
        cin>>temp;
        YI.push_back(temp);
        Y.push_back(YI);
    }
    SHAPE1+=1;
    LsMethod(X,Y);//调用最小二乘法
}