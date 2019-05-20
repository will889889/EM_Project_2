#pragma once
#include"DataStructure.h"
#include<string>
#include<map>

class Caculator
{
public:
	Caculator();
	
	void LoadFunction(std::string);
	double Caculate(std::map<char, double>);

private:
	Term* currentFunc;
	
};