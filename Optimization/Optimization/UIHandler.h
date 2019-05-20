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
	std::string m_Input;
	Method m_Method;
	std::vector<std::string> equations;
	std::vector<std::string> DoMath();	//Method _method, double _initPoint, double _leftBound, double _rightBound
};