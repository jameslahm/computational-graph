#include <iostream>
#include <sstream>
#include "lib/comgraph.h"
#include "basic_calc_pack/basic_calc_pack.h"
#include "advanced_calc_pack/advanced_calc_pack.h"
#include "compare_calc_pack/compare_calc_pack.h"
using namespace std;
ostream &ErrOut = cout; //应项目要求，错误信息向cout输出
ostream &PriOut = cout; //应项目要求，PriNode信息向cout输出
ostream &AnsOut = cout; //答案输出至cout
int main() {
	map<string, ComGraph<double> > Graphs;
	Graphs["default"] = ComGraph<double>();
	int n;
	//独立节点的建立-------BEGIN-------
	cin >> n;
	for (int i = 1; i <= n; ++i) {
		string NodeName;
		char NodeType;
		string SessionName;
		double val;
		cin >> NodeName >> NodeType; //cin会默认跳过空字符，所以不用getchar()
		switch (NodeType) {
		case 'C':{//创建ConNode
			cin >> val;
			getline(cin,SessionName);
			if (SessionName == "") SessionName = "default";
			if (!Graphs.count(SessionName)) { Graphs[SessionName] = ComGraph<double>(); }
			ComGraph<double>& localSession = Graphs[SessionName];
			localSession.BuildConNode(NodeName, val);
			break;
        }
		case 'P':{ //创建PHNode
			getline(cin,SessionName);
			if (SessionName == "") SessionName = "default";
			if (!Graphs.count(SessionName)) { Graphs[SessionName] = ComGraph<double>(); }
			ComGraph<double>& localSession = Graphs[SessionName];
			localSession.BuildPHNode(NodeName);
			break;
        }
		case 'V': {//创建VarNode
			cin >> val;
			getline(cin,SessionName);
			if (SessionName == "") SessionName = "default";
			if (!Graphs.count(SessionName)) { Graphs[SessionName] = ComGraph<double>(); }
			ComGraph<double>& localSession = Graphs[SessionName];
			localSession.BuildVarNode(NodeName, val);
			break;
        }
		default: { //异常输入，输出错误信息，强行终止程序
			ErrOut << "ERROR: Failed to build " << NodeName << endl;
			exit(i);
        }
		}
	}
	//独立节点的建立--------END--------
	//读取已有的配置文件
	cout << "Load the existing session from files?(Y/N)" << endl;
	char ans;
	cin >> ans;
	if (ans == 'Y' || ans == 'y') {
		cout << "Enter the session name you are working on and file path" << endl;
		string name;
		string path;
		cin >> name;
		cin >> path;
		Graphs[name].Load(path);
	}
	//读取完成
    //依赖节点的建立-------BEGIN-------
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        string NodeName, TempStr;
        vector<string> OperandsList;
        string SessionName;
        cin >> NodeName >> TempStr; //多读一个等号
        cin >> TempStr;
        if (TempStr == "SIN" || TempStr == "LOG" || TempStr == "EXP" || TempStr == "TANH" ||
            TempStr == "SIGMOID") { //单目
            string OP;
            cin >> OP;
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
            OperandsList.push_back(OP);
            if (TempStr == "SIN")
                localSession.BuildCalcNode<SinCNode<double>>(NodeName, 1, OperandsList);
            else if (TempStr == "LOG")
                localSession.BuildCalcNode<LogCNode<double>>(NodeName, 1, OperandsList);
            else if (TempStr == "EXP")
                localSession.BuildCalcNode<ExpCNode<double>>(NodeName, 1, OperandsList);
            else if (TempStr == "TANH")
                localSession.BuildCalcNode<TanhCNode<double>>(NodeName, 1, OperandsList);
            else if (TempStr == "SIGMOID")
                localSession.BuildCalcNode<SigCNode<double>>(NodeName, 1, OperandsList);
        } else if (TempStr == "PRINT") { //调试输出
            cin >> TempStr;
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
            localSession.BuildPriNode(NodeName, TempStr);
        } else if (TempStr == "COND") { //条件
            for (int j = 1; j <= 3; ++j) {
                cin >> TempStr;
                OperandsList.push_back(TempStr);
            }
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
            localSession.BuildCalcNode<CondNode<double>>(NodeName, 3, OperandsList);
        }  else if (TempStr == "ASSERT"){
			cin >> TempStr;
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
			localSession.BuildAsserTNode(NodeName,TempStr);
		} else if (TempStr == "BIND") { //条件
            for (int j = 1; j <= 2; ++j) {
                cin >> TempStr;
                OperandsList.push_back(TempStr);
			}
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
			localSession.BuildCalcNode<BindNode<double>>(NodeName, 2, OperandsList);
		} else if(TempStr == "ASSIGN"){
            for (int j = 1; j <= 2; ++j) {
                cin >> TempStr;
                OperandsList.push_back(TempStr);
			}
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
			localSession.BuildCalcNode<AssignCNode<double>>(NodeName,2,OperandsList);
		} else if(TempStr=="GRAD"){
            cin>>TempStr;
            OperandsList.push_back(TempStr);
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
            localSession.BuildCalcNode<GradNode<double>>(NodeName,1,OperandsList);
        }
        else if (1) { //双目
            string OP1 = TempStr, OP2, OPRT;
            cin >> OPRT >> OP2;
            OperandsList.push_back(OP1);
            OperandsList.push_back(OP2);
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
            if (OPRT=="AT")
                localSession.BuildCalcNode<GradCNode<double>>(NodeName,2,OperandsList);
            if (OPRT == "+")
                localSession.BuildCalcNode<PluCNode<double>>(NodeName, 2, OperandsList);
            else if (OPRT == "-")
                localSession.BuildCalcNode<MinCNode<double>>(NodeName, 2, OperandsList);
            else if (OPRT == "*")
                localSession.BuildCalcNode<MulCNode<double>>(NodeName, 2, OperandsList);
            else if (OPRT == "/")
                localSession.BuildCalcNode<DivCNode<double>>(NodeName, 2, OperandsList);
            else if (OPRT == ">")
                localSession.BuildCalcNode<GTCNode<double>>(NodeName, 2, OperandsList);
            else if (OPRT == "<")
                localSession.BuildCalcNode<LSCNode<double>>(NodeName, 2, OperandsList);
            else if (OPRT == ">=")
                localSession.BuildCalcNode<GECNode<double>>(NodeName, 2, OperandsList);
            else if (OPRT == "<=")
                localSession.BuildCalcNode<LECNode<double>>(NodeName, 2, OperandsList);
            else if (OPRT == "==")
                localSession.BuildCalcNode<EQCNode<double>>(NodeName, 2, OperandsList);
        } else { //异常输入，输出错误信息，强行终止程序
            ErrOut << "ERROR: Failed to build " << NodeName << endl;
            exit(i);
        }
    }

    //依赖节点的建立--------END--------
    //命令执行-------------BEGIN-------
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        string Command, NodeName;
        cin >> Command;
        double Res = 0;
        bool failed = false;
        string SessionName="default";
        if (Command == "EVAL") {
            int InitPHNum = 0;
            string PHCommand;
            cin >> NodeName;
            getline(cin, PHCommand);
            istringstream PHC(PHCommand);
            PHC >> InitPHNum;
            vector<pair<string, double>> PHList;
            for (int j = 1; j <= InitPHNum; ++j) {
                string PHName;
                double Val;
                PHC >> PHName >> Val;
                PHList.push_back(make_pair(PHName, Val));
            }
            PHC >> SessionName;
            if(SessionName!="default") SessionName=" "+SessionName;
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
            try {
                Res = localSession.Eval(NodeName, PHList);
            }
            catch (string &ErrMsg) {
                ErrOut << ErrMsg << endl;
                failed = true;
            }
            if (!failed) AnsOut << Res << endl;
            localSession.RecInHistory(Res);
        } else if (Command == "SETANSWER") {
            cin >> NodeName;
            int AnsPos;
            cin >> AnsPos;
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
            localSession.SetVarVal(NodeName, localSession.ReadFromHistory(AnsPos));
            localSession.RecInHistory(Res);
        } else if (Command == "SETCONSTANT") {
            cin >> NodeName;
            double Val;
            cin >> Val;
            getline(cin,SessionName);
            if(SessionName=="") SessionName="default";
            if(!Graphs.count(SessionName)){Graphs[SessionName]=ComGraph<double>();}
            ComGraph<double>& localSession=Graphs[SessionName];
            localSession.SetVarVal(NodeName, Val);
            localSession.RecInHistory(Res);
        }
    }
    //命令执行--------------END--------
	//保存Session配置
	cout << "Save the session from this program? (Y/N)" << endl;
	cin >> ans;
	if (ans == 'Y' || ans == 'y') {
		cout << "Enter the session name you want to save and file path" << endl;
		string name;
		string path;
		cin >> name;
		cin >> path;
		Graphs[name].Save(path);
	}
	//保存Session配置完成
    return 0;
}

