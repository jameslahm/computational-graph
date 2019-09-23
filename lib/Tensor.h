#ifndef TENSOR_H
#define TENSOR_H
#include <vector>
#include <iostream>
#include <cmath>
#include <iterator>
using namespace std;
template <typename T>
class Tensor
{
private:
    int shape0;             //维数1
    int shape1;             //维数2
    vector<vector<T>> data; //二维vector数组
    string ErrorMsg;        //报错信息
public:
    Tensor(vector<vector<T>> v) : data(v)
    { //以一个二维数组创建矩阵
        shape0 = v.size();
        shape1 = v[0].size();
    }
    Tensor(vector<T> v)
    { //以行向量创建矩阵
        data.push_back(v);
        shape0 = 1;
        shape1 = v.size();
    }
    Tensor()
    { //创建空矩阵
        shape0 = 0;
        shape1 = 0;
    }
    Tensor(T n)
    { //以常数创建矩阵（实现隐式转换）
        vector<vector<T>> vec;
        vector<T> vecI;
        vecI.push_back(n);
        vec.push_back(vecI);
        data = vec;
        shape0 = 1;
        shape1 = 1;
    }
    int getshape0() { return shape0; }
    int getshape1() { return shape1; }
    void setshape0(int s) { shape0 = s; }
    void setshape1(int s) { shape1 = s; }
    T sum();
    Tensor<T> operator+(Tensor<T> t);
    Tensor<T> operator-(Tensor<T> t);
    Tensor<T> operator*(Tensor<T> t);
    Tensor<T> operator/(Tensor<T> t);
    vector<T> &operator[](int index);
    Tensor<T> operator==(Tensor<T> t);
    Tensor<T> operator>=(Tensor<T> t);
    Tensor<T> operator<=(Tensor<T> t);
    Tensor<T> operator>(Tensor<T> t);
    Tensor<T> operator<(Tensor<T> t);
    bool iszero();
    bool gtzero();
    bool lszero();
    bool lezero();
    bool gezero();
    Tensor<T> pmul(Tensor<T> t);
    Tensor<T> sin();
    Tensor<T> log();
    Tensor<T> exp();
    Tensor<T> sig();
    Tensor<T> tanh();
    Tensor<T> tranverse();
    Tensor<T> broadcast(Tensor<T> t);
    Tensor<T> pow(int n)
    {
        vector<vector<T>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = std::pow(temp[i][j], n);
            }
        }
        Tensor<T> t(temp);
        return t;
    }
    Tensor<T> cos()
    {
        vector<vector<T>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = std::cos(temp[i][j]);
            }
        }
        Tensor<T> t(temp);
        return t;
    }
    void Shape()
    {
        shape0 = data.size();
        shape1 = data[0].size();
    }
    Tensor<T> reshape(int shape0, int shape1)
    { //改变矩阵维数
        vector<vector<T>> temp;
        typename vector<vector<T>>::iterator it = data.begin();
        typename vector<T>::iterator itI = (*it).begin();
        for (int i = 0; i < shape0; i++)
        {
            vector<T> tempI;
            for (int j = 0; j < shape1; j++)
            {
                tempI.push_back(*itI);
                itI++;
                if (itI == (*it).end())
                {
                    it++;
                    itI = (*it).begin();
                }
            }
            temp.push_back(tempI);
        }
        Tensor<T> t(temp);
        return t;
    }
    vector<vector<T>> &getVector() { return data; }
    Tensor<T> concat(Tensor<T> R, int axis)
    { //矩阵的拼接
        vector<vector<T>> temp = data;
        if (axis == 0)
        {
            for (int i = 0; i < R.getshape0(); i++)
            {
                temp.push_back(R[i]);
            }
        }
        else
        {
            for (int i = 0; i < data.size(); i++)
            {
                for (int j = 0; j < R.getshape1(); j++)
                {
                    temp[i].push_back(R[i][j]);
                }
            }
        }
        Tensor<T> t(temp);
        return t;
    }
    ~Tensor()
    {
        data.clear();
    }
};

template <>
Tensor<double> Tensor<double>::pmul(Tensor<double> t)
{ //矩阵的按位乘法
    vector<vector<double>> x;
    for (int i = 0; i < shape0; i++)
    {
        vector<double> xI;
        for (int j = 0; j < shape1; j++)
        {
            xI.push_back(data[i][j] * t[i][j]);
        }
        x.push_back(xI);
    }
    Tensor<double> res(x);
    return res;
}

template <>
bool Tensor<double>::iszero()
{ //矩阵是否全部为0
    int count = 0;
    for (int i = 0; i < getshape0(); i++)
    {
        for (int j = 0; j < getshape1(); j++)
        {
            if (data[i][j] != 0 && data[i][j] != (-0))
                break;
            count++;
        }
    }
    if (count == getshape0() * getshape1())
        return true;
    else
        return false;
}

template <>
bool Tensor<double>::gtzero()
{ //矩阵元素是否全大于0
    int count = 0;
    for (int i = 0; i < getshape0(); i++)
    {
        for (int j = 0; j < getshape1(); j++)
        {
            if (data[i][j] <= 0)
                break;
            count++;
        }
    }
    if (count == getshape0() * getshape1())
        return true;
    else
        return false;
}

template <>
bool Tensor<double>::lszero()
{ //矩阵元素是否全小于0
    int count = 0;
    for (int i = 0; i < getshape0(); i++)
    {
        for (int j = 0; j < getshape1(); j++)
        {
            if (data[i][j] >= 0)
                break;
            count++;
        }
    }
    if (count == getshape0() * getshape1())
        return true;
    else
        return false;
}

template <>
bool Tensor<double>::gezero()
{ //矩阵元素是否全大于等于0
    int count = 0;
    for (int i = 0; i < getshape0(); i++)
    {
        for (int j = 0; j < getshape1(); j++)
        {
            if (data[i][j] < 0)
                break;
            count++;
        }
    }
    if (count == getshape0() * getshape1())
        return true;
    else
        return false;
}

template <>
bool Tensor<double>::lezero()
{ //矩阵元素是否全小于等于0
    int count = 0;
    for (int i = 0; i < getshape0(); i++)
    {
        for (int j = 0; j < getshape1(); j++)
        {
            if (data[i][j] > 0)
                break;
            count++;
        }
    }
    if (count == getshape0() * getshape1())
        return true;
    else
        return false;
}

template <>
double Tensor<double>::sum()
{ //求矩阵所有元素的和
    double res = 0;
    for (int i = 0; i < getshape0(); i++)
    {
        for (int j = 0; j < getshape1(); j++)
        {
            res += data[i][j];
        }
    }
    return res;
}

template <>
Tensor<double> Tensor<double>::operator==(Tensor<double> t)
{ //矩阵按位判等
    if (getshape0() == 1 && getshape1() == 1)
    {
        vector<vector<double>> temp = t.getVector();
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[0][0] == t[i][j]);
            }
        }
        return Tensor<double>(temp);
    }
    if (t.getshape0() == 1 && t.getshape1() == 1)
    {
        vector<vector<double>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[i][j] == t[0][0]);
            }
        }
        return Tensor<double>(temp);
    }
    vector<vector<double>> temp = data;
    for (int i = 0; i < temp.size(); i++)
    {
        for (int j = 0; j < temp[0].size(); j++)
        {
            temp[i][j] = double(data[i][j] == t[i][j]);
        }
    }
    Tensor<double> t1(temp);
    return t1;
}

template <>
Tensor<double> Tensor<double>::operator>=(Tensor<double> t)
{ //矩阵按位判>=
    if (getshape0() == 1 && getshape1() == 1)
    {
        vector<vector<double>> temp = t.getVector();
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[0][0] >= t[i][j]);
            }
        }
        return Tensor<double>(temp);
    }
    if (t.getshape0() == 1 && t.getshape1() == 1)
    {
        vector<vector<double>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[i][j] >= t[0][0]);
            }
        }
        return Tensor<double>(temp);
    }
    vector<vector<double>> temp = data;
    for (int i = 0; i < temp.size(); i++)
    {
        for (int j = 0; j < temp[0].size(); j++)
        {
            temp[i][j] = double(data[i][j] >= t[i][j]);
        }
    }
    Tensor<double> t1(temp);
    return t1;
}

template <>
Tensor<double> Tensor<double>::operator<=(Tensor<double> t)
{ //矩阵按位判<=
    if (getshape0() == 1 && getshape1() == 1)
    {
        vector<vector<double>> temp = t.getVector();
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[0][0] <= t[i][j]);
            }
        }
        return Tensor<double>(temp);
    }
    if (t.getshape0() == 1 && t.getshape1() == 1)
    {
        vector<vector<double>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[i][j] <= t[0][0]);
            }
        }
        return Tensor<double>(temp);
    }
    vector<vector<double>> temp = data;
    for (int i = 0; i < temp.size(); i++)
    {
        for (int j = 0; j < temp[0].size(); j++)
        {
            temp[i][j] = double(data[i][j] <= t[i][j]);
        }
    }
    Tensor<double> t1(temp);
    return t1;
}

template <>
Tensor<double> Tensor<double>::operator<(Tensor<double> t)
{ //矩阵按位判<
    if (getshape0() == 1 && getshape1() == 1)
    {
        vector<vector<double>> temp = t.getVector();
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[0][0] < t[i][j]);
            }
        }
        return Tensor<double>(temp);
    }
    if (t.getshape0() == 1 && t.getshape1() == 1)
    {
        vector<vector<double>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[i][j] < t[0][0]);
            }
        }
        return Tensor<double>(temp);
    }
    vector<vector<double>> temp = data;
    for (int i = 0; i < temp.size(); i++)
    {
        for (int j = 0; j < temp[0].size(); j++)
        {
            temp[i][j] = double(data[i][j] < t[i][j]);
        }
    }
    Tensor<double> t1(temp);
    return t1;
}

template <>
Tensor<double> Tensor<double>::operator>(Tensor<double> t)
{ // 矩阵按位判>
    if (getshape0() == 1 && getshape1() == 1)
    {
        vector<vector<double>> temp = t.getVector();
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[0][0] > t[i][j]);
            }
        }
        return Tensor<double>(temp);
    }
    if (t.getshape0() == 1 && t.getshape1() == 1)
    {
        vector<vector<double>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                temp[i][j] = double(data[i][j] > t[0][0]);
            }
        }
        return Tensor<double>(temp);
    }
    vector<vector<double>> temp = data;
    for (int i = 0; i < temp.size(); i++)
    {
        for (int j = 0; j < temp[0].size(); j++)
        {
            temp[i][j] = double(data[i][j] > t[i][j]);
        }
    }
    Tensor<double> t1(temp);
    return t1;
}
template <typename T>
vector<T> &Tensor<T>::operator[](int index)
{
    return data[index];
}

template <typename T>
Tensor<T> Tensor<T>::operator/(Tensor<T> t)
{
    if (getshape0() == 1 && getshape1() == 1)
    { //矩阵本身为1x1标量
        vector<vector<T>> temp = t.getVector();
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                if (t[i][j] < 1E-5)
                    temp[i][j] = data[0][0] / t[i][j];
                else
                    temp[i][j] = data[0][0] / t[i][j];
            }
        }
        return Tensor<T>(temp);
    }
    else if (t.getshape0() == 1 && t.getshape1() == 1)
    { //除数为标量
        vector<vector<T>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                if (t[0][0] < 1E-5)
                    temp[i][j] = data[i][j] / t[0][0];
                else
                    temp[i][j] = data[i][j] / t[0][0];
            }
        }
        return Tensor<T>(temp);
    }
    else if (data[0].size() == t.getVector()[0].size() && data.size() == t.getVector().size())
    { //两矩阵维数对应
        vector<vector<T>> temp = data;
        for (int i = 0; i < temp.size(); i++)
        {
            for (int j = 0; j < temp[0].size(); j++)
            {
                if (t[i][j] < 1E-5)
                    temp[i][j] = data[i][j] / (1E-5);
                else
                    temp[i][j] = data[i][j] / t[i][j];
            }
        }
        Tensor<T> t1(temp);
        return t1;
    }
    else
    { //否则报错
        ErrorMsg = "/ ERROR";
        throw ErrorMsg;
    }
}

template <typename T>
Tensor<T> Tensor<T>::operator+(Tensor<T> t)
{
    Tensor<T> broad;
    Tensor<T> x;
    if (shape0 == 1 && shape1 == 1)
    { //若矩阵为标量
        for (int i = 0; i < t.getshape0(); i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < t.getshape1(); j++)
            {
                x[i].push_back(t[i][j] + data[0][0]);
            }
        }
        x.Shape();
        return x;
    }
    if (t.getshape0() == 1 && t.getshape1() == 1)
    { //若另一加数为标量
        for (int i = 0; i < getshape0(); i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < getshape1(); j++)
            {
                x[i].push_back(t[0][0] + data[i][j]);
            }
        }
        x.Shape();
        return x;
    }
    if (shape0 == t.getshape0() && shape1 == t.getshape1())
    { //维数相同
        for (int i = 0; i < this->shape0; i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < shape1; j++)
            {
                x[i].push_back(data[i][j] + t.data[i][j]);
            }
        }
        x.Shape();
        return x;
    }
    else if (shape0 == t.getshape0())
    {
        if (shape1 == 1)
        { //广播矩阵本身
            broad = this->broadcast(t);
            for (int i = 0; i < this->shape0; i++)
            {
                vector<T> temp;
                x.getVector().push_back(temp);
                for (int j = 0; j < broad.getshape1(); j++)
                {
                    x[i].push_back(broad[i][j] + t[i][j]);
                }
            }
            x.Shape();
            return x;
        }
        else if (t.getshape1() == 1)
        { //广播另一加数矩阵
            broad = t.broadcast(*this);
            for (int i = 0; i < this->shape0; i++)
            {
                vector<T> temp;
                x.getVector().push_back(temp);
                for (int j = 0; j < shape1; j++)
                {
                    x[i].push_back(data[i][j] + broad[i][j]);
                }
            }
            x.Shape();
            return x;
        }
        else
        {
            x.Shape();
            return x;
        }
    }
    else if (shape1 == t.getshape1())
    {
        if (shape0 == 1)
        {
            broad = this->broadcast(t); //广播矩阵本身
            for (int i = 0; i < broad.getshape0(); i++)
            {
                vector<T> temp;
                x.getVector().push_back(temp);
                for (int j = 0; j < shape1; j++)
                {
                    x[i].push_back(broad[i][j] + t.data[i][j]);
                }
            }
            x.Shape();
            return x;
        }
        else if (t.getshape0() == 1)
        {
            broad = t.broadcast(*this); //广播另一矩阵
            for (int i = 0; i < this->shape0; i++)
            {
                vector<T> temp;
                x.getVector().push_back(temp);
                for (int j = 0; j < shape1; j++)
                {
                    x[i].push_back(data[i][j] + broad[i][j]);
                }
            }
            x.Shape();
            return x;
        }
        else
        {
            x.Shape();
            return x;
        }
    }
    else
    { //无法相加，报错
        ErrorMsg = "+ ERROR";
        throw ErrorMsg;
    }
}

template <typename T>
Tensor<T> Tensor<T>::operator-(Tensor<T> t)
{
    Tensor<T> broad;
    Tensor<T> x;
    if (shape0 == 1 && shape1 == 1)
    { //矩阵本身为标量
        for (int i = 0; i < t.getshape0(); i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < t.getshape1(); j++)
            {
                x[i].push_back(data[0][0] - t[i][j]);
            }
        }
        x.Shape();
        return x;
    }
    if (t.getshape0() == 1 && t.getshape1() == 1)
    { //另一矩阵为标量
        for (int i = 0; i < getshape0(); i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < getshape1(); j++)
            {
                x[i].push_back(data[i][j] - t[0][0]);
            }
        }
        x.Shape();
        return x;
    }
    if (shape0 == t.getshape0() && shape1 == t.getshape1())
    { //维数相同
        for (int i = 0; i < this->shape0; i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < shape1; j++)
            {
                x[i].push_back(data[i][j] - t[i][j]);
            }
        }
        x.Shape();
        return x;
    }
    else if (shape0 == t.getshape0())
    {
        if (shape1 == 1)
        { //矩阵本身可广播
            broad = this->broadcast(t);
            for (int i = 0; i < this->shape0; i++)
            {
                vector<T> temp;
                x.getVector().push_back(temp);
                for (int j = 0; j < broad.getshape1(); j++)
                {
                    x[i].push_back(broad[i][j] - t[i][j]);
                }
            }
            x.Shape();
            return x;
        }
        else if (t.getshape1() == 1)
        {
            broad = t.broadcast(*this); //另一矩阵可广播
            for (int i = 0; i < this->shape0; i++)
            {
                vector<T> temp;
                x.getVector().push_back(temp);
                for (int j = 0; j < shape1; j++)
                {
                    x[i].push_back(data[i][j] - broad[i][j]);
                }
            }
            x.Shape();
            return x;
        }
        else
        {
            x.Shape();
            return x;
        }
    }
    else if (shape1 == t.getshape1())
    {
        if (shape0 == 1)
        { //矩阵本身可广播
            broad = this->broadcast(t);
            for (int i = 0; i < broad.getshape0(); i++)
            {
                vector<T> temp;
                x.getVector().push_back(temp);
                for (int j = 0; j < shape1; j++)
                {
                    x[i].push_back(broad[i][j] - t[i][j]);
                }
            }
            x.Shape();
            return x;
        }
        else if (t.getshape0() == 1)
        {
            broad = t.broadcast(*this); //另一矩阵可广播
            for (int i = 0; i < this->shape0; i++)
            {
                vector<T> temp;
                x.getVector().push_back(temp);
                for (int j = 0; j < shape1; j++)
                {
                    x[i].push_back(data[i][j] - broad[i][j]);
                }
            }
            x.Shape();
            return x;
        }
        else
        {
            x.Shape();
            return x;
        }
    }
    else
    { //无法相减，报错
        ErrorMsg = "- ERROR";
        throw ErrorMsg;
    }
}
template <typename T>
Tensor<T> Tensor<T>::operator*(Tensor<T> t)
{
    if (t.getshape0() == 1 && t.getshape1() == 1)
    { //如果另一矩阵为标量
        Tensor<T> x;
        for (int i = 0; i < this->shape0; i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < this->shape1; j++)
            {
                x[i].push_back(t[0][0] * data[i][j]);
            }
        }
        x.Shape();
        return x;
    }
    else if (getshape0() == 1 && getshape1() == 1)
    { //如果该矩阵为标量
        Tensor<T> x;
        for (int i = 0; i < t.getshape0(); i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < t.getshape1(); j++)
            {
                x[i].push_back(t[i][j] * data[0][0]);
            }
        }
        x.Shape();
        return x;
    }
    else if (data[0].size() == t.getVector().size())
    { //如果维数满足相乘条件
        Tensor<T> x;
        for (int i = 0; i < this->shape0; i++)
        {
            vector<T> temp;
            for (int j = 0; j < t.getshape1(); j++)
            {
                double temp1 = 0;
                for (int k = 0; k < this->shape1; k++)
                {
                    temp1 += data[i][k] * t[k][j];
                }
                temp.push_back(temp1);
            }
            x.getVector().push_back(temp);
            temp.clear();
        }
        x.Shape();
        return x;
    }
    else
    { //否则报错
        ErrorMsg = "* ERROR";
        throw ErrorMsg;
    }
}
template <typename T>
Tensor<T> Tensor<T>::sin()
{ //按位sin
    Tensor<T> x;
    for (int i = 0; i < this->shape0; i++)
    {
        vector<T> temp;
        x.getVector().push_back(temp);
        for (int j = 0; j < shape1; j++)
        {
            x[i].push_back(std::sin(data[i][j]));
        }
    }
    x.Shape();
    return x;
}
template <typename T>
Tensor<T> Tensor<T>::log()
{ //按位求log
    Tensor<T> x;
    for (int i = 0; i < this->shape0; i++)
    {
        vector<T> temp;
        x.getVector().push_back(temp);
        for (int j = 0; j < shape1; j++)
        {
            x[i].push_back(std::log(data[i][j]));
        }
    }
    x.Shape();
    return x;
}
template <typename T>
Tensor<T> Tensor<T>::exp()
{ //按位求exp
    Tensor<T> x;
    for (int i = 0; i < this->shape0; i++)
    {
        vector<T> temp;
        x.getVector().push_back(temp);
        for (int j = 0; j < shape1; j++)
        {
            x[i].push_back(std::exp(data[i][j]));
        }
    }
    x.Shape();
    return x;
}
template <typename T>
Tensor<T> Tensor<T>::tanh()
{ //按位求tanh
    Tensor<T> x;
    for (int i = 0; i < this->shape0; i++)
    {
        vector<T> temp;
        x.getVector().push_back(temp);
        for (int j = 0; j < shape1; j++)
        {
            x[i].push_back(std::tanh(data[i][j]));
        }
    }
    x.Shape();
    return x;
}
template <typename T>
Tensor<T> Tensor<T>::sig()
{ //按位求sigmold
    Tensor<T> x;
    for (int i = 0; i < this->shape0; i++)
    {
        vector<T> temp;
        x.getVector().push_back(temp);
        for (int j = 0; j < this->shape1; j++)
        {
            x[i].push_back(1.0 / (1 + std::exp(-data[i][j])));
        }
    }
    x.Shape();
    return x;
}
template <typename T>
Tensor<T> Tensor<T>::tranverse()
{ //返回矩阵的转置
    Tensor<T> x;
    vector<T> temp;
    for (int i = 0; i < this->shape1; i++)
    {
        for (int j = 0; j < this->shape0; j++)
        {
            temp.push_back(data[j][i]);
        }
        vector<vector<T>> &xi = x.getVector();
        xi.push_back(temp);
        temp.clear();
    }
    x.Shape();
    return x;
}

template <typename T>
Tensor<T> Tensor<T>::broadcast(Tensor<T> t)
{ //矩阵的广播，可按行或按列
    Tensor<T> x;
    x.setshape0(t.getshape0());
    x.setshape1(t.getshape1());
    if (shape0 == 1 && shape1 == 1)
    {
        for (int i = 0; i < t.getshape0(); i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < t.getshape1(); j++)
            {
                x[i].push_back(data[0][0]);
            }
        }
    }
    else if (shape0 == 1)
    {
        for (int i = 0; i < t.getshape0(); i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
            for (int j = 0; j < shape1; j++)
            {
                x[i].push_back(data[0][j]);
            }
        }
    }
    else if (shape1 == 1)
    {
        for (int i = 0; i < t.getshape0(); i++)
        {
            vector<T> temp;
            x.getVector().push_back(temp);
        }
        for (int j = 0; j < t.getshape1(); j++)
        {
            for (int i = 0; i < t.getshape0(); i++)
            {
                x[i].push_back(data[i][0]);
            }
        }
    }
    return x;
}

#endif