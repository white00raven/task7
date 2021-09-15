#include <iostream>
#include <string>
#include <cmath>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
using namespace std;
vector<double> squares;
mutex Mutex;
double formula(const string str, double x)
{
	double sum = 0, sum2 = 0;
	bool flag = true;
	for (int i = 0; i < str.length(); i++)
	{
		string str_temp;
		if (str[i] >= '0'&&str[i] <= '9')
		{
			str_temp = "";
			for (; i < str.length() && ((str[i] >= '0'&&str[i] <= '9') || str[i] == '.'); i++)
			{
				str_temp += str[i];
			}
			sum2 = stod(str_temp);
			continue;
		}
		switch (str[i])
		{
		case 'x':
			sum2 = x;
			break;
		case '+':
			if (flag)
			{
				sum += sum2;
			}
			if (!flag)
			{
				sum -= sum2;
			}flag = true;
			sum2 = 0;
			break;
		case '-':
			if (flag)
			{
				sum += sum2;
			}
			if (!flag)
			{
				sum -= sum2;
			}
			flag = false;
			sum2 = 0;
			break;
		case '*':
			str_temp = "";
			for (; i < str.length() && ((str[i] >= '0'&&str[i] <= '9') || str[i] == '.'); i++)
			{
				str_temp += str[i];
			}
			sum2 *= stod(str_temp);
			break;
		case '/':str_temp = "";
			for (; i < str.length(); i++)
			{
				str_temp += str[i];
			}
			sum /= formula(str_temp, x);
			break;
		case 's':if (i + 3 < str.length() && str[i + 1] == 'i' && str[i + 2] == 'n' && str[i + 3] == '(')
		{
			str_temp = "";
			for (i += 4; i < str.length() && str[i] != ')'; i++)
			{
				str_temp += str[i];
			}
			if (i != str.length())
			{
				sum2 = sin(formula(str_temp, x));
			}
			else
			{
				cout << "formula not full" << endl;
				throw logic_error("formula not full");
			}
		}
				 break;
		case 'c':if (i + 3 < str.length() && str[i + 1] == 'o' && str[i + 2] == 's' && str[i + 3] == '(')
		{
			str_temp = "";
			for (i += 4; i < str.length() && str[i] != ')'; i++)
			{
				str_temp += str[i];
			}
			if (i != str.length())
			{
				sum2 = cos(formula(str_temp, x));
			}
			else
			{
				cout << "formula not full" << endl;
				throw logic_error("formula not full");
			}
		}break;
		case 't':if (i + 2 < str.length() && str[i + 1] == 'g' &&  str[i + 2] == '(')
		{
			str_temp = "";
			for (i += 3; i < str.length() && str[i] != ')'; i++)
			{
				str_temp += str[i];
			}
			if (i != str.length())
			{
				sum2 = tan(formula(str_temp, x));
			}
			else
			{
				cout << "formula not full" << endl;
				throw logic_error("formula not full");
			}
		}break;
		default:
			cout << "formula not correct" << endl;
			throw logic_error("formula not correct");
			break;
		}
	}
	if (flag)
	{
		sum += sum2;
	}
	if (!flag)
	{
		sum -= sum2;
	}
	return sum;
}
double met_rect(const string str, double a, double b)
{
	double sum = 0;
	for (double i = a; i < b; i += 0.00001)
		sum += formula(str, i)*0.00001;
	return sum;
}
void met_simp(const string str, double a, double b)
{
	double sum = 0;
	sum = formula(str, a);
	sum += 4 * formula(str, (a + b) / 2);
	sum += formula(str, b);
	sum *= (b - a) / 6;
	lock_guard<mutex> lock(Mutex);
	squares.push_back(sum);
}
double metodSimpson(const string str, double a, double b, int coltred, double eps)
{
	double pred = 100000, sum = 0, l, n = 1, beg = a;

	do
	{
		l = (b - beg) / n;
		vector <std::thread> threads;
		for (std::size_t i = 0; i < n; ++i, beg += l)
		{
			threads.emplace_back(met_simp, str, beg, beg + l);
			if (i + 1 == coltred)
			{
				
				for (auto &rThread : threads)
					rThread.join();
				threads.clear();
			}
		}
		for (auto &rThread : threads)
			rThread.join();
		for (auto in : squares)
		{
			sum += in;
		}
		if ((pred - sum) < eps)
			break;
		else
		{
			pred = sum;
			sum = 0;
			n *= 2;
			beg = a;
			threads.clear();
			squares.clear();
		}
	} while (true);
	return sum;
}
void main()
{
	string str ;
	double beg, end, eps, colthread;
	cout << "Write the formula" << endl;
	cin >> str;
	cout << "Write the begin and end segment" << endl;
	cin >> beg >> end;
	cout << "Write  quantity thread" << endl;
	cin >> colthread;
	cout << "Write epsilon" << endl;
	cin >> eps;
	double x = metodSimpson(str, beg, end, colthread, eps), x1 = met_rect(str, beg, end);
	cout << "comparison metod Simpsona and metod rectangle" << endl;
	cout << "metod Simpsona: "<<x << endl;
	cout << "metod rectangle: " << x1 << endl;
	system("pause");
}