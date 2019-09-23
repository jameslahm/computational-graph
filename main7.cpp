#include <iostream>
#include <sstream>
#include "lib/comgraph.h"
#include "basic_calc_pack/basic_calc_pack.h"
#include "advanced_calc_pack/advanced_calc_pack.h"
#include "compare_calc_pack/compare_calc_pack.h"
using namespace std;
typedef string string;

ostream &ErrOut = cout; //默认错误输出流
ostream &PriOut = cout; //默认Print输出流
ostream &AnsOut = cout; //默认答案输出流

int main()
{

	ComGraph<Tensor<double> > Sample_Graph(ErrOut, PriOut); 
	int n;
	//常量变量占位符节点的创建-------BEGIN-------
	cin >> n;
	for (int i = 1; i <= n; ++i) {
		string NodeName;
		char NodeType;
		int shape0; //Tensor的第一个维数
		int shape1; //Tensor第二个维数
		vector<vector<double> > data;
		cin >> NodeName >> NodeType; //输入节点名和类型
		switch (NodeType) {
		case 'C': { //ConNode
			cin >> shape0 >> shape1 ; //输入维数
			for (int j = 0; j < shape0; j++) { //生成二维vector代表矩阵
				vector<double> temp;
				data.push_back(temp);
				for (int k = 0; k < shape1; k++) {
					double x;
					cin >> x;
					data[j].push_back(x);
				}
			}
			Tensor<double> val(data);
			Sample_Graph.BuildConNode(NodeName, val);
			break;
		}
		case 'P': //PHNode
			Sample_Graph.BuildPHNode(NodeName);
			break;
		case 'V': {//VarNode
			cin >> shape0 >> shape1; //输入维数
			for (int j = 0; j < shape0; j++) { //生成二维vector代表矩阵
				vector<double> temp;
				data.push_back(temp);
				for (int k = 0; k < shape1; k++) {
					double x;
					cin >> x;
					data[j].push_back(x);
				}
			}
			Tensor<double> val(data);
			Sample_Graph.BuildVarNode(NodeName, val);
			break; 
		}
		default: { 
			ErrOut << "ERROR: Failed to build " << NodeName << endl;
			exit(i);
		}
		}
	}
	//占位符常量变量节点的创建--------END--------
	//计算节点的创建-------BEGIN-------
	cin >> n;
	for (int i = 1; i <= n; ++i) {
		string NodeName, TempStr;
		vector<string> OperandsList;
		cin >> NodeName >> TempStr; //输入节点名和运算符
		cin >> TempStr;
		if (TempStr == "SIN" || TempStr == "LOG" || TempStr == "EXP" || TempStr == "TANH" ||
			TempStr == "SIGMOID") { //单目
			string OP;
			cin >> OP;
			OperandsList.push_back(OP);
			if (TempStr == "SIN")
				Sample_Graph.BuildCalcNode<SinCNode<Tensor<double> > >(NodeName, 1, OperandsList);
			else if (TempStr == "LOG")
				Sample_Graph.BuildCalcNode<LogCNode<Tensor<double> > >(NodeName, 1, OperandsList);
			else if (TempStr == "EXP")
				Sample_Graph.BuildCalcNode<ExpCNode<Tensor<double> > >(NodeName, 1, OperandsList);
			else if (TempStr == "TANH")
				Sample_Graph.BuildCalcNode<TanhCNode<Tensor<double> > >(NodeName, 1, OperandsList);
			else if (TempStr == "SIGMOID")
				Sample_Graph.BuildCalcNode<SigCNode<Tensor<double> > >(NodeName, 1, OperandsList);
		}else if (TempStr == "ASSERT"){
			cin >> TempStr;
			Sample_Graph.BuildAsserTNode(NodeName,TempStr);
		} else if (TempStr == "BIND") { //条件
            for (int j = 1; j <= 2; ++j) {
                cin >> TempStr;
                OperandsList.push_back(TempStr);
			}
			Sample_Graph.BuildCalcNode<BindNode<Tensor<double>>>(NodeName, 2, OperandsList);
		} else if(TempStr == "ASSIGN"){ 
            for (int j = 1; j <= 2; ++j) {
                cin >> TempStr;
                OperandsList.push_back(TempStr);
			}
			Sample_Graph.BuildCalcNode<AssignCNode<Tensor<double>>>(NodeName,2,OperandsList);
		} 
		else if (TempStr == "PRINT") { 
			cin >> TempStr;
			Sample_Graph.BuildPriNode(NodeName, TempStr);
		}
		else if (TempStr == "COND") { 
			for (int j = 1; j <= 3; ++j) {
				cin >> TempStr;
				OperandsList.push_back(TempStr);
			}
			Sample_Graph.BuildCalcNode<CondNode<Tensor<double> > >(NodeName, 3, OperandsList);
		}

		else if (TempStr == "GRAD") {
			cin >> TempStr;
			OperandsList.push_back(TempStr);
			Sample_Graph.BuildCalcNode<GradNode<Tensor<double> > >(NodeName, 1, OperandsList);
		}
		else if (Sample_Graph.FindNode(TempStr)) { //双目
			string OP1 = TempStr, OP2, OPRT;
			cin >> OPRT >> OP2;
			OperandsList.push_back(OP1);
			OperandsList.push_back(OP2);
			if(OPRT == "AT")
				Sample_Graph.BuildCalcNode<GradCNode<Tensor<double>>> (NodeName,2,OperandsList);
			if (OPRT == "+")
				Sample_Graph.BuildCalcNode<PluCNode<Tensor<double> > >(NodeName, 2, OperandsList);
			else if (OPRT == "-")
				Sample_Graph.BuildCalcNode<MinCNode<Tensor<double> > >(NodeName, 2, OperandsList);
			else if (OPRT == "*")
				Sample_Graph.BuildCalcNode<MulCNode<Tensor<double> > >(NodeName, 2, OperandsList);
			else if (OPRT == "/")
				Sample_Graph.BuildCalcNode<DivCNode<Tensor<double> > >(NodeName, 2, OperandsList);
			else if (OPRT == ">")
				Sample_Graph.BuildCalcNode<GTCNode<Tensor<double> > >(NodeName, 2, OperandsList);
			else if (OPRT == "<")
				Sample_Graph.BuildCalcNode<LSCNode<Tensor<double> > >(NodeName, 2, OperandsList);
			else if (OPRT == ">=")
				Sample_Graph.BuildCalcNode<GECNode<Tensor<double> > >(NodeName, 2, OperandsList);
			else if (OPRT == "<=")
				Sample_Graph.BuildCalcNode<LECNode<Tensor<double> > >(NodeName, 2, OperandsList);
			else if (OPRT == "==")
				Sample_Graph.BuildCalcNode<EQCNode<Tensor<double> > >(NodeName, 2, OperandsList);
		}
		else { //无可以匹配的节点类型
			ErrOut << "ERROR: Failed to build " << NodeName << endl;
			exit(i);
		}
	}
	//计算节点的创建--------END--------
	//执行指令-------------BEGIN-------
	cin >> n;
	for (int i = 1; i <= n; ++i) {
		string Command, NodeName;
		cin >> Command;
		Tensor<double> Res;
		bool failed = false;
		if (Command == "EVAL") {
			int InitPHNum = 0;
			string PHCommand;
			cin >> NodeName;
			getline(cin, PHCommand);
			istringstream PHC(PHCommand);
			PHC >> InitPHNum;
			vector<pair<string, Tensor<double>>> PHList;
			for (int j = 1; j <= InitPHNum; ++j) {
				string PHName;
				int shape0;
				int shape1;
				vector<vector<double> > data;
				PHC >> PHName;
				PHC >> shape0 >> shape1;
				for (int j = 0; j < shape0; j++) {
					vector<double> temp;
					data.push_back(temp);
					for (int k = 0; k < shape1; k++) {
						double x;
						PHC >> x;
						data[j].push_back(x);
					}
				}
				Tensor<double> val(data); //建立矩阵
				PHList.push_back(make_pair(PHName, val));
			}
			try {
				Res = Sample_Graph.Eval(NodeName, PHList);
			}
			catch (string& ErrMsg) {
				ErrOut << ErrMsg << endl;
				failed = true;
			}
			if (!failed) {
				for (int i = 0; i < Res.getshape0(); i++) { //顺序输出矩阵的每一个元素
					for (int j = 0; j < Res.getshape1(); j++) {
						cout << Res[i][j] << " ";
					}
					cout << endl;
				}
				cout<<"-------------------------"<<endl;
			}
		}
		Sample_Graph.RecInHistory(Res);
	}
	//执行指令--------------END--------
	return 0;
}