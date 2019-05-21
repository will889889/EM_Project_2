#include <vector>
#include <string>

std::string Vector2String(std::vector<double> v)
{
	std::string result = "[ ";
	for (int i = 0; i < v.size(); i++)
	{
		result += std::to_string(v[i]);

		if (i == (v.size() - 1))
		{
			result += " ]";
			continue;
		}
		result += " ,\t";
	}
	return result;
}

std::vector<double> v_Multiply(std::vector<double> &v, double &coef)
{
	std::vector<double> result = v;
	for (int i = 0; i < v.size(); i++)
	{
		result[i] *= coef;
	}
	return result;
}
std::vector<double> v_Sum(std::vector<double> &v1, std::vector<double> &v2)
{
	///	exception
	if (v1.size() != v2.size())
		std::vector<double>();

	std::vector<double> result = v1;
	for (int i = 0; i < v1.size(); i++)
	{
		result[i] += v2[i];
	}
	return result;
}
std::vector<double> v_Subtract(std::vector<double> &v1, std::vector<double> &v2)
{
	///	exception
	if (v1.size() != v2.size())
		std::vector<double>();

	std::vector<double> result = v1;
	for (int i = 0; i < v1.size(); i++)
	{
		result[i] -= v2[i];
	}
	return result;
}
double DeltaVector(std::vector<double> &v1, std::vector<double> &v2)
{
	///	exception
	if (v1.size() != v2.size())
		return 0;

	double sum = 0.0;
	for (int i = 0; i < v1.size(); i++)
	{
		sum += (v1[i] * v2[i]);
	}
	return sum;
}