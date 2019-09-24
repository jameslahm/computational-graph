### 计算图
#### 一.	实现功能及原理

计算图库功能包括以下：
1. 基础功能：

  - 创建静态图并创建各种节点进行各种运算

  - 调试Assert运算和捆绑Bind运算:<br/>
  &emsp;&emsp;实现原理：派生节点类基类Node<T>，创建AssertNode<T>和BindNode<T>两节点类。Assert调用其监视的节点的GetVal()函数，若>0则输出始终为0，<0则报错。 Bind节点计算时会递归的调用其两个依赖节点的GetVal()函数，输出第一个节点值的同时计算第二个节点的值。

  - 运算符+、*的链式求导功能:<br/>
  &emsp;&emsp;实现原理：对GRAD节点建立GradNode，内部存放依赖的节点，对AT节点建立GradCNode,内部存放GradNode节点和要被求导的节点。具体计算时会调用GradCNode的Calc()函数，进行链式求导并返回值。

2. 拓展功能：
   - 为Variable赋值的Assign运算<br/>
    &emsp;&emsp;实现原理：建立AssignCNode节点类，继承计算节点CalcNode类。其依赖节点为两个变量节点，在AssignCNode的计算中，先将第二个变量节点的值赋给Assign节点本身，两个变量节点值不变。在AssignCNode的clear函数中，再将第二个变量值赋给第一个变量。在一次EVAL结束后，才会循环调用中间节点clear函数以清空节点值，如此便实现了Assign对变量赋值的延迟性，使其更改在本次Eval中不会生效。

    - 完整的链式求导功能<br/>
    &emsp;&emsp;实现原理：具体实现与基础求导功能相同，只需对不同的计算节点实现不同的grad函数即可。

   - 对所有运算符的求高阶导功能<br/>
   &emsp;&emsp;实现原理：求高阶导的方法为差分法，通过计算变量附近的导数值，运用差分计算即可求得任意高阶导以及偏导。

    - Tensor的二维版本<br/>
   &emsp;&emsp;实现内容：Tensor类内部的数据结构为二维的vector数组，同时存有其两个维度的数值。支持的操作包括计算图中所有的运算符。同时也实现了reshape、concat、broadcast等功能。在矩阵运算中会根据维数判断两矩阵大小是否匹配，并判断是否能够broadcast。同时支持常数到Tensor对象的隐式转换，以方便矩阵与常数的运算。矩阵的求导共支持标量和矩阵的相互求导，行向量与列向量的相互 求导。

    - 运算图的Session<br/>
    &emsp;&emsp;实现内容:在图ComGraph<T>类中，实现了Save和Load函数。Save和Load函数需要用户输入存入（读取）的文件名和路径，便可以文本形式存储、读取一个图中所有的变量名与其对应值。同时支持在创建节点和eval时带Session名指定当前操作所对应的session，如： Eval a 1 c 2 session1.若无Session名参数输入，主程序会默认在名为”default”的Session中进行操作。 

    - 梯度下降求优化最小二乘法<br/>
   &emsp;&emsp;实现原理：通过建立X，Y，W相应的计算节点，计算得到对W的梯度，并进行相应的参数更新即可。具体实现思路及原理详见说明文档(Notes.md)。

    - 基于神经网络的手写数字识别:
   &emsp;&emsp;实现原理：首先设计神经网络结构，通过建立相应的节点建立相应的输入层、隐藏层以及输出层。通过梯度下降可对神经网络层的参数进行优化。具体实现思路及原理详见说明文档(Notes.md)

#### 二．功能测试
1. 第二阶段基础功能<br/>
&emsp;&emsp;第二阶段基础功能均能通过OJ测试，下载样例在Windows, Linux环境下手动测试，均无错误。

2. 第二阶段拓展功能

    - Assign运算<br/>
	    测试程序：main2.exe 

	    测试内容：Assign节点的创建、Assign的赋值操作是否生效、Assign赋值的延迟性（对变量的改变从下一次EVAL生效）。

        测试样例及结果：<br/>
        样例1：[data1.input](../Examples/Assign_Example/data1.input)<br/>
        标准输出：[data1.output](../Examples/Assign_Example/data1.output)<br/>
        样例2：[data2.input](../Examples/Assign_Example/data2.input)<br/>			
        标准输出：[data2.output](../Examples/Assign_Example/data2.output)<br/>
        样例3：[data3.input](../Examples/Assign_Example/data3.input)<br/>			
        标准输出：[data3.output](../Examples/Assign_Example/data3.output)<br/>	
        测试使用文件：[checker.py](../Examples/Assign_Example/checker.py)

	    经测试，程序输出与标准输出相同无误，Assign测试通过。
    
    - 链式求导与求高阶导
	    测试程序：main2.exe

	    测试内容：样例1：所有运算符GRAD节点和AT节点的创建、所有运算符链式求导（一阶导）的实现。样例2：所有运算符高阶GRAD节点和AT节点的创建与链式求高阶导的实现。

	    测试样例及结果：<br/>
        样例1：[data1.input](../Examples/Grad_Example/data1.input)<br/>
        标准输出：[data1.output](../Examples/Grad_Example/data1.output)<br/>
        样例2：[data2.input](../Examples/Grad_Example/data2.input)<br/>			
        标准输出：[data2.output](../Examples/Grad_Example/data2.output)<br/>
        测试使用文件：[checker.py](../Examples/Grad_Example/checker.py)

	    经测试，程序输出与标准输出相同无误，链式求导和求高阶导测试通过。

    - 二维矩阵Tensor
	    测试程序：main7.exe

	    测试内容：Tensor所有类型运算符节点的创建、求值，Tensor的自动求导功能，Tensor在运算中的broadcast、concat操作。

	    测试样例及结果；
        样例1：[data1.input](../Examples/Tensor_Example/data1.input)<br/>
        标准输出：[data1.output](../Examples/Tensor_Example/data1.output)<br/>

	    经测试，程序输出与标准输出相同无误，二维Tensor的基本运算和求导测试通过。

    - 运算图的Session
	    测试程序：main5.exe 

	    测试内容：样例1：在指定Session中的节点创建和Eval，Session的存储（Save）功能。样例2:在默认Session中的节点创建和Eval，Session的读取(Load)功能。样例3：同一个程序中同时存在多个Session的兼容性。

	    测试样例及结果：<br/>
        样例1：[data1.input](../Examples/Session_Example/data1.input)<br/>
        标准输出：[data1.output](../Examples/Session_Example/data1.output)<br/>
        样例2：[data2.input](../Examples/Session_Example/data2.input)<br/>			
        标准输出：[data2.output](../Examples/Session_Example/data2.output)<br/>
        样例3：[data3.input](../Examples/Session_Example/data3.input)<br/>			
        标准输出：[data3.output](../Examples/Session_Example/data3.output)<br/>
        测试使用文件：[checker.py](../Examples/Grad_Example/checker.py)(注：由于报错先后顺序的问题，checker的输出不为ok)

	    经测试，程序输出与标准输出相同无误，Session测试通过。

    - 梯度下降求优化最小二乘法
	    测试程序： main4.exe

	    测试内容：构造多变量的样例输入，对迭代完后的MSE输出，并将最终的参数与最小二乘法直接求解的结果进行对比

	    测试样例及结果：<br/>
        样例1：[data1.input](../Examples/Session_Example/data1.input)<br/>
        标准输出：[data1.output](../Examples/Session_Example/data1.output)<br/>
        样例2：[data2.input](../Examples/Session_Example/data2.input)<br/>			
        标准输出：[data2.output](../Examples/Session_Example/data2.output)<br/>

        经测试，在100次迭代循环后，MSE均达到收敛，并且达到一个很小的值，测试效果良好。（注：在实际使用中由于收敛速度的不同，需要对LEARNING_RATE进行设置）。
        
    - 基于神经网络的手写数字识别：
        测试程序： main6.exe
    
        测试内容：对mnist数据集进行训练，并对测试集进行相应的测试，输出测试准确率
    
        测试样例及结果：<br/>
        样例：[mnist.input](../Examples/mnist.input) (数据使用python进行了预处理，详见说明文档)
        标准输出：[mnist.output](../Examples/mnist.output)
    
        经测试，在100次迭代训练后，准确率收敛，并达到$87.2\%$。由于本身限于数据经过降维部分信息丢失的原因，可得神经网络训练后识别效果较好。


		
