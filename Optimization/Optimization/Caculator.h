#pragma once
#include"DataStructure.h"
#include<string>
#include<vector>
#include<map>
#include<sstream>

class Caculator
{
public:
	Caculator();
	
	std::vector<char> LoadFunction(std::string);
	double Caculate(std::map<char, double>);

private:
	Term* currentFunc;
	
};