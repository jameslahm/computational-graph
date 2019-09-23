#include "lib/comgraph.h"
#include "basic_calc_pack/basic_calc_pack.h"
#include "advanced_calc_pack/advanced_calc_pack.h"
#include "compare_calc_pack/compare_calc_pack.h"
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<time.h>
#include<random>
#include<fstream>
using namespace std;


int TRAINLEN=9000;//训练样本个数
int TESTLEN=1000;//测试集样本个数
int DLEN=45;//样本的维度，原本维度为784，数据通过python进行预处理，通过降维到了45维
int HIDDEN1=20;//隐藏层1的神经元个数
int HIDDEN2=10;//隐藏层2的神经元个数，其实也为输出层，该层的激活函数维softmax，输出10个类别的概率值
int EPOCHS=100;//训练总次数
int BATCHSIZE=100;//batch_size,批量小样本训练，增加收敛速度
double LEARNINGRATE=0.01;//学习率
Tensor<double> param(LEARNINGRATE/BATCHSIZE);//常数项

Tensor<double> optimw(Node<Tensor<double>>* ws,Tensor<double> errors,Node<Tensor<double>>* ap,Node<Tensor<double>>* zp,Node<Tensor<double>>* app){
    Tensor<double> errorp=(ws->GetVal().tranverse()*errors).pmul(ap->grad(zp));
    Tensor<double> grad=errorp*(app->GetVal().tranverse());
    return grad;//对中间层求取误差对w的梯度（反向传播）
}

Tensor<double> optimw(Node<Tensor<double>>* node,Node<Tensor<double>>* ap,Node<Tensor<double>>* z){
    Tensor<double> grad=node->grad(z);
    grad=grad*(ap->GetVal().tranverse());
    return grad;//对输出层求取误差对w的梯度（反向传播）
}


Tensor<double> optimb(Node<Tensor<double>>* ws,Tensor<double> errors,Node<Tensor<double>>* ap,Node<Tensor<double>>* zp,Node<Tensor<double>>* app){
    Tensor<double> errorp=(ws->GetVal().tranverse()*errors).pmul(ap->grad(zp));
    Tensor<double> grad=errorp;
    return grad;//对中间层求取误差对b的梯度（反向传播）
}

Tensor<double> optimb(Node<Tensor<double>>* node,Node<Tensor<double>>* z){
    Tensor<double> grad=node->grad(z);
    return grad;//对输出层求取误差对b的梯度（反向传播）
}

int main(){
    ComGraph<Tensor<double>>* graph=new ComGraph<Tensor<double>>();
    default_random_engine e(time(0));
    ifstream infile;
    infile.open("Examples/mnist.input");//数据文件，原本mnist数据集为60000张图片，数据预处理部分用python进行了处理，包括将图片转化为数组，进行降维操作
    uniform_real_distribution<double> u(-1.0,1.0);//对所有w，b的参数均采用(-1,1)均匀随机浮点数的方式进行初始化
    vector<vector<double>> TRAINX;//训练样本
    vector<vector<double>> TRAINY;//训练样本的LABEL
    for(int i=0;i<TRAINLEN;i++){
        vector<double> XI;
        vector<double> YI;
        for(int j=0;j<DLEN;j++){
            double temp;
            infile>>temp;
            XI.push_back(temp);
        }
        double temp2;
        infile>>temp2;
        YI.push_back(temp2);
        TRAINX.push_back(XI);
        TRAINY.push_back(YI);
    }
    vector<vector<double>> TESTX;//测试样本
    vector<vector<double>> TESTY;//测试样本的LABEL
    for(int i=0;i<TESTLEN;i++){
        vector<double> XI;
        vector<double> YI;
        for(int j=0;j<DLEN;j++){
            double temp;
            infile>>temp;
            XI.push_back(temp);
        }
        double temp2;
        infile>>temp2;
        YI.push_back(temp2);
        TESTX.push_back(XI);
        TESTY.push_back(YI);
    }

    Tensor<double> trainx(TRAINX[0]);
    trainx=trainx.tranverse();
    Tensor<double> trainy(TRAINY[0]);
    Tensor<double> testx(TESTX[0]);
    testx=testx.tranverse();
    Tensor<double> testy(TESTY[0]);
    graph->BuildVarNode("X",trainx);//建立X节点
    graph->BuildVarNode("Y",trainy);//建立Y节点
    vector<vector<double>> b1;
    vector<double> b1I;
    for(int i=0;i<HIDDEN1;i++){
        b1I.push_back(u(e));
    }
    b1.push_back(b1I);
    graph->BuildVarNode("b1",Tensor<double>(b1).tranverse());//建立隐藏层1的b
    vector<vector<double>> W1;
    for(int i=0;i<HIDDEN1;i++){
        vector<double> W1I;
        for(int j=0;j<DLEN;j++){
            W1I.push_back(u(e));
        }
        W1.push_back(W1I);
    }
    graph->BuildVarNode("W1",Tensor<double>(W1));//建立隐藏层1的w
    vector<string> s;
    s.push_back("W1");
    s.push_back("X");
    graph->BuildCalcNode<MulCNode<Tensor<double>>>("W1X",s);
    s.clear();
    s.push_back("W1X");
    s.push_back("b1");
    graph->BuildCalcNode<PluCNode<Tensor<double>>>("W1X+b1",s);
    s.clear();
    s.push_back("W1X+b1");
    graph->BuildCalcNode<SigCNode<Tensor<double>>>("OUT1",s);//隐藏层1的激活函数为sigmoid函数
    s.clear();
    vector<vector<double>> b2;
    vector<double> b2I;
    for(int i=0;i<HIDDEN2;i++){
        b2I.push_back(u(e));
    }
    b2.push_back(b2I);
    graph->BuildVarNode("b2",Tensor<double>(b2).tranverse());//建立隐藏层2的b
    vector<vector<double>> W2;
    for(int i=0;i<HIDDEN2;i++){
        vector<double> W2I;
        for(int j=0;j<HIDDEN1;j++){
            W2I.push_back(u(e));
        }
        W2.push_back(W2I);
    }
    graph->BuildVarNode("W2",Tensor<double>(W2));//建立隐藏层2的w
    s.push_back("W2");
    s.push_back("OUT1");
    graph->BuildCalcNode<MulCNode<Tensor<double>>>("W2OUT1",s);
    s.clear();
    s.push_back("W2OUT1");
    s.push_back("b2");
    graph->BuildCalcNode<PluCNode<Tensor<double>>>("OUT2",s);
    s.clear();
    s.push_back("OUT2");
    s.push_back("Y");
    graph->BuildCalcNode<SoftmaxCNode<Tensor<double>>>("PRO",s);//建立softmax节点
    for(int i=1;i<=EPOCHS;i++){
        Tensor<double> w2grad(0);
        Tensor<double> b2grad(0);
        Tensor<double> w1grad(0);
        Tensor<double> b1grad(0);
        Node<Tensor<double>>* w1=graph->GetNode("W1");
        Node<Tensor<double>>* w2=graph->GetNode("W2");
        Node<Tensor<double>>* b1=graph->GetNode("b1");
        Node<Tensor<double>>* b2=graph->GetNode("b2");
        for(int j=0;j<TRAINLEN;j++){
            graph->SetVarVal("X",Tensor<double>(TRAINX[j]).tranverse());
            Node<Tensor<double>>* x=graph->GetNode("X");
            graph->SetVarVal("Y",Tensor<double>(TRAINY[j]).tranverse());
            Node<Tensor<double>>* pro=graph->GetNode("PRO");
            Node<Tensor<double>>* out1=graph->GetNode("OUT1");
            Node<Tensor<double>>* out2=graph->GetNode("OUT2");
            Node<Tensor<double>>* w1b1=graph->GetNode("W1X+b1");
            w2grad=w2grad+optimw(pro,out1,out2);//获取w2的梯度
            Tensor<double> b2delta=optimb(pro,out2);//获取b2的梯度
            b2grad=b2grad+b2delta;
            w1grad=w1grad+optimw(w2,b2delta,out1,w1b1,x);//获取w1的梯度
            b1grad=b1grad+optimb(w2,b2delta,out1,w1b1,x);//获取b1的梯度
            pro->Clear();
            if(j%BATCHSIZE==99){//每100个样本，进行一次梯度下降
                w1->SetVal(w1->GetVal()-(param*w1grad));
                w2->SetVal(w2->GetVal()-(param*w2grad));
                b1->SetVal(b1->GetVal()-(param*b1grad));
                b2->SetVal(b2->GetVal()-(param*b2grad));
                w2grad=Tensor<double>(0);
                b2grad=Tensor<double>(0);
                w1grad=Tensor<double>(0);
                b1grad=Tensor<double>(0);
            }
        }
        if(i%10==0){//每10次EPOCH，进行测试集样本的测试，并输出准确率
            double temp=0;
            for(int j=0;j<TESTLEN;j++){
                graph->SetVarVal("X",Tensor<double>(TESTX[j]).tranverse());
                graph->SetVarVal("Y",Tensor<double>(TESTY[j]).tranverse());
                Tensor<double> res=graph->GetNode("PRO")->GetVal();
                graph->GetNode("PRO")->Clear();
                int max=0;
                for(int i=0;i<res.getshape0();i++){
                    if(res[i][0]>res[max][0])max=i;
                }
                if(max==TESTY[j][0]) temp++;
            }
            cout<<"EPOCH:"<<i<<" ACCURACY:"<< temp/TESTLEN<<endl;
        }
    }
}