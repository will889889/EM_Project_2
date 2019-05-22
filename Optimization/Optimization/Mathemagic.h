#pragma once
#include <vector>
#include <exception>
#include <string>

std::string Vector2String(std::vector<double> v);
std::vector<double> v_Multiply(std::vector<double> &v, double &coef);
std::vector<double> v_Sum(std::vector<double> &v1, std::vector<double> &v2);
std::vector<double> v_Subtract(std::vector<double> &v1, std::vector<double> &v2);
double DeltaVector(std::vector<double> &v1, std::vector<double> &v2);
double Length(std::vector<double> &V);

#pragma region Matrix
//定義資料結構
struct Matrix
{
public:
	Matrix();
	//	init the size of Rows and Columns
	Matrix(int row, int col);

	std::string Name = "";
	std::vector< std::vector<double>> Data;
};
inline Matrix::Matrix()
{

}
inline Matrix::Matrix(int row, int col)
{
	Data = std::vector< std::vector<double>>(row);
	for (int i = 0; i < row; i++)
		Data[i] = std::vector<double>(col);
}
//	Calc
static class CaCuMi
{
public:
	static std::vector<double> Matrix2Vector(const Matrix& M);
	static void PrintMatrix(const Matrix& M, std::vector<std::string>& Answer);

	static const Matrix Multiply(const Matrix& M1, const Matrix& M2);
	static const Matrix Multiply(const Matrix& M, const double& scale);
	//	行列式
	static const double Determinant(const Matrix& M);
	static const double Determinant(double **M, int size);
	static const double Laplace(double **M, const int& size);	//	Helper
	static const Matrix Cofactors(const Matrix& M);
	//trans
	static const Matrix Transpose(const Matrix& M);
	static const Matrix Adjoint(const Matrix& M);
	static const Matrix Inverse(const Matrix& M);

	static const Matrix Eigen(const Matrix& M);
	static const std::vector<double> MulV(const std::vector<double> &L, const std::vector<double> &R);
	static const std::vector<double> AddV(const std::vector<double> &L, const std::vector<double> &R, bool add);

	//	若R或C大小為零則丟例外
	static void ZeroCheck(const Matrix& M);
};
#pragma endregion