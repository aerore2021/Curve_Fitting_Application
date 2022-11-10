#pragma once
#include <iostream>
#include <vector>
#include <Eigen/Dense>

#ifndef algorithm_h
double FindMax(std::vector<double>& num);
double FindMin(std::vector<double>& num);
//�������ղ�ֵ�����ĳ��x��Ӧ��fx,
//�ǵ��ж�degree����߽׵Ĵ�С
double L_interpolation(int degree, std::vector<double>& num_x, std::vector<double>& num_fx, double val_x);
//ţ�ٲ�ֲ�ֵ
double Divided_Difference(int degree, std::vector<double>& num_x, std::vector<double>& num_fx, double val_x);
//������Ȼ������ֵ 
//���صĶ�ά��������a,b,c,d
std::vector<std::vector<double>> Create_Cubic_Spline_Interpolation(std::vector<double>& num_x, std::vector<double>& num_fx);
//����ĳ�������������ֵ
double Compute_Cubic_Spline(std::vector<std::vector<double>> info, std::vector<double>& num_x, double val_x);
//���ع��ڶ���ʽ��С���˷�ϵ��������
std::vector<double> Create_Polynomial_Least_Squres(int degree, std::vector<double>& num_x, std::vector<double>& num_fx);
//�������Է�����A��Ԫ��
Eigen::VectorXd Compute_Coeffecient_A(int degree, std::vector<double>& num_x);
//�������Է�����b��Ԫ��
Eigen::VectorXd Compute_Coeffecient_b(int degree, std::vector<double>& num_x, std::vector<double>& num_fx);
//�������ʽ��С���˷�
double Compute_Polynomial_Least_Squres(std::vector<double> info, double val_x);
#endif // !algorithm_h