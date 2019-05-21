#pragma once
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<iostream>
#include<map>
//using namespace std;

enum Method
{
	Powell,
	Newton,
	SteepDescent,
	QuasiNewton,
	ConjugateGradient
};

class UIHandler
{
public:
	UIHandler();

	std::string m_Input;
	std::vector<std::string> Answer;
	//	for calc
	Method m_Method;
	std::vector<std::string> equations;
	int equationIndex;
	int varsCount;
	std::vector<double> initPoint;
	std::vector< std::pair<double, double> > intervals;
	std::vector<double> Position;
	std::vector<double> Direction;

	std::vector<std::string> DoMath();	//Method _method, double _initPoint, double _leftBound, double _rightBound

	std::vector<std::string> Powell_method();
	double goldenSectionSearch(double a, double b, double c, double tau);
	double CalculateByCoef(double coef);
	std::vector<std::string> TEST();
};