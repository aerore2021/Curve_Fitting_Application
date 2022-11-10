#include <algorithm>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include <vector>

#define MAX_ITERATION 200
#define TOL 0.00001

double FindMax(std::vector<double>& num)
{
	if (num.empty())
		return 0;
	double m = num.front();
	for (int i = 1; i < num.size(); i++)
	{
		if (m < num[i])
			m = num[i];
	}
	return m;
}

double FindMin(std::vector<double>& num)
{
	if (num.empty())
		return 0;
	double m = num.front();
	for (int i = 1; i < num.size(); i++)
	{
		if (m > num[i])
			m = num[i];
	}
	return m;
}
double L_interpolation(int degree, std::vector<double>&num_x , std::vector<double>&num_fx, double val_x)
{
	using namespace std;
	vector<vector <double> > Q;
	vector<double> t;
	for (int i = 0; i < num_x.size(); i++)
		t.push_back(num_fx[i]);
	Q.push_back(t);
	t.clear();
	for (int row = 1; row <= degree; row++)
	{
		for (int col = 0; col <= degree-row; col++)
		{
			double m = ((val_x - num_x[col]) * Q[row - 1][col + 1] - (val_x - num_x[col + row]) * Q[row - 1][col]) / (num_x[row + col] - num_x[col]);
			t.push_back(m);
		}
		Q.push_back(t);
		t.clear();
	}
	return Q[degree][0];	
}

double Divided_Difference(int degree, std::vector<double>& num_x, std::vector<double>& num_fx, double val_x)
{
	using namespace std;
	vector<vector<double>> F;
	vector<double> t;
	for (int i = 0; i < num_x.size(); i++)
		t.push_back(num_fx[i]);
	F.push_back(t);
	t.clear();
	for (int row = 1; row <= degree; row++)
	{
		for (int col = 0; col <= degree - row; col++)
		{
			t.push_back((F[row - 1][col + 1] - F[row - 1][col]) / (num_x[row + col] - num_x[col]));
		}
		F.push_back(t);
		t.clear();
	}
	double res = F[0][0];
	double item = val_x - num_x[0];
	for (int i = 1; i < degree; i++)
	{
		res += F[i][0] * item;
		item *= (val_x - num_x[i]);
	}
	return res;
}

std::vector<std::vector<double>> Create_Cubic_Spline_Interpolation(std::vector<double>& num_x, std::vector<double>& num_fx)
{
	using namespace std;
	vector<vector <double>> res;
	vector <double> a, b, c, d;
	vector <double>h;
	int n = num_x.size() - 1;
	for (int i = 0; i <= n; i++)
	{
		if (i < n)
			h.push_back(num_x[i + 1] - num_x[i]);
		a.push_back(num_fx[i]);
	}

	vector <double> alpha;
	for (int i = 0; i <= n - 2; i++)
	{
		double t = 3 * (a[i + 2] - a[i + 1]) / h[i + 1] - 3 * (a[i + 1] - a[i]) / h[i];
		alpha.push_back(t);
	}

	vector <double> l, u, z;
	l.push_back(1); u.push_back(0); z.push_back(0);
	for (int i = 1; i <= n - 1; i++)
	{
		double t = 2 * (num_x[i + 1] - num_x[i - 1]) - h[i - 1] * u[i - 1];
		l.push_back(t);
		u.push_back(h[i] / l[i]);
		t = alpha[i - 1] - h[i - 1] * z[i - 1];
		t /= l[i];
		z.push_back(t);
	}

	l.push_back(1); c.push_back(0); z.push_back(0);

	for (int j = 0; j <= n - 1; j++)
	{
		c.push_back(z[n - 1 - j] - u[n - 1 - j] * c[j]);
		double t = (a[n - j] - a[n - j - 1]) / h[n - j - 1] - h[n - 1 - j] * (c[j] +2 * c[j + 1]) / 3;
		b.push_back(t);
		d.push_back((c[j] - c[j + 1]) / 3 / h[n - 1 - j]);
	}
	reverse(c.begin(), c.end());
	reverse(b.begin(), b.end());
	reverse(d.begin(), d.end());
	a.pop_back(); c.pop_back();

	res.push_back(a);
	res.push_back(b);
	res.push_back(c);
	res.push_back(d);

	return res;
}

double Compute_Cubic_Spline(std::vector<std::vector<double>> info, std::vector<double>& num_x, double val_x)
{
	int i = 0;
	while (val_x > num_x[i+1])
		i++;

	double t = info[0][i];
	double item = val_x - num_x[i];
	for (int j = 1; j < 4; j++)
	{
		t += info[j][i] * item;
		item *= item;
	}
	return t;
	
}

Eigen::VectorXd Compute_Coeffecient_A(int degree, std::vector<double>& num_x)
{
	using namespace Eigen;
	using namespace std;

	VectorXd res;
	res.resize(2 * degree - 1);
	res(0) = num_x.size();
	for (int i = 1; i < 2 * degree - 1; i++)
	{
		double t = 0;
		for (int j = 0; j < num_x.size(); j++)
		{
			t += pow(num_x[j], i);
		}
		res(i) = t;
	}
	return res;
}

Eigen::VectorXd Compute_Coeffecient_b(int degree, std::vector<double>& num_x, std::vector<double>& num_fx)
{
	using namespace Eigen;
	using namespace std;

	VectorXd res;
	res.resize(degree);
	for (int i = 0; i < degree; i++)
	{
		double t = 0;
		for (int j = 0; j < num_x.size(); j++)
		{
			t += num_fx[j] * pow(num_x[j], i);
		}
		res(i) = t;
	}
	return res;
}

std::vector<double> Create_Polynomial_Least_Squres(int degree, std::vector<double>& num_x, std::vector<double>& num_fx)
{
	using namespace std;
	using namespace Eigen;
	MatrixXd DL,U;
	DL.resize(degree,degree);
	U.resize(degree,degree);
	DL.setZero();
	U.setZero();

	VectorXd coef = Compute_Coeffecient_A(degree, num_x);
	for (int i = 0; i < degree; i++)
	{
		for (int j = 0; j < degree; j++)
		{
			if (i == j)
				DL(i, j) = coef(i + j);
			else if (i < j)
				U(i, j) = -coef(i + j);
			else
				DL(i, j) = coef(i + j);
		}
	}
	DL = DL.inverse();
	VectorXd b = Compute_Coeffecient_b(degree, num_x, num_fx);
	MatrixXd Tg = DL * U;
	b = DL * b;
	
	VectorXd x(degree);
	x.setOnes();
	int i = 0;
	while (i < MAX_ITERATION)
	{
		VectorXd t = Tg*x + b;
		double a = t.norm(), b = x.norm();
		if (abs(a-b) / b <= TOL)
			break;
		x = t;
		i++;
	}
	vector<double> res(&x[0], x.data() + x.cols() * x.rows());
	
	return res;
}

double Compute_Polynomial_Least_Squres(std::vector<double> info, double val_x)
{
	double t = 0;
	for (int i = 0; i < info.size(); i++)
	{
		t += pow(val_x, i) * info[i];
	}
	return t;
}

