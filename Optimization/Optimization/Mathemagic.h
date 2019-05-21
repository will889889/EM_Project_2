#pragma once
#include <vector>
#include <string>

std::string Vector2String(std::vector<double> v);
std::vector<double> v_Multiply(std::vector<double> &v, double &coef);
std::vector<double> v_Sum(std::vector<double> &v1, std::vector<double> &v2);
std::vector<double> v_Subtract(std::vector<double> &v1, std::vector<double> &v2);
double DeltaVector(std::vector<double> &v1, std::vector<double> &v2);
