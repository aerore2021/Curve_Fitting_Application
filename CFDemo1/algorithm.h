#pragma once
#include <iostream>
#include <vector>
#include <Eigen/Dense>

#ifndef algorithm_h
double FindMax(std::vector<double>& num);
double FindMin(std::vector<double>& num);
//拉格朗日插值，算出某个x对应的fx,
//记得判断degree和最高阶的大小
double L_interpolation(int degree, std::vector<double>& num_x, std::vector<double>& num_fx, double val_x);
//牛顿差分插值
double Divided_Difference(int degree, std::vector<double>& num_x, std::vector<double>& num_fx, double val_x);
//三次自然样条插值 
//返回的二维向量包含a,b,c,d
std::vector<std::vector<double>> Create_Cubic_Spline_Interpolation(std::vector<double>& num_x, std::vector<double>& num_fx);
//计算某个点的三次样条值
double Compute_Cubic_Spline(std::vector<std::vector<double>> info, std::vector<double>& num_x, double val_x);
//返回关于多项式最小二乘法系数的向量
std::vector<double> Create_Polynomial_Least_Squres(int degree, std::vector<double>& num_x, std::vector<double>& num_fx);
//计算线性方程组A的元素
Eigen::VectorXd Compute_Coeffecient_A(int degree, std::vector<double>& num_x);
//计算线性方程组b的元素
Eigen::VectorXd Compute_Coeffecient_b(int degree, std::vector<double>& num_x, std::vector<double>& num_fx);
//计算多项式最小二乘法
double Compute_Polynomial_Least_Squres(std::vector<double> info, double val_x);
#endif // !algorithm_h