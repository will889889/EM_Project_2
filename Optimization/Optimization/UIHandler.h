#pragma once
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<iostream>
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
	//	for calcu
	Method m_Method;
	std::vector<std::string> equations;
	int equationIndex;
	int varsCount;
	std::vector<double> initPoint;
	std::vector< std::pair<double, double> > intervals;

	std::vector<std::string> DoMath();	//Method _method, double _initPoint, double _leftBound, double _rightBound

	std::vector<std::string> Powell_method();
	std::vector<std::string> TEST();
};