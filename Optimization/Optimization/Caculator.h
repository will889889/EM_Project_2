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
	
	Term* myCurrentFunc();
	std::vector<char> LoadFunction(std::string);
	double Caculate(std::map<char, double>);
	double Caculate(Term*, std::map<char, double>);
	Term* PartialDiff(Term* , char);
	double PartialDerivative(Term*, char, std::map<char, double>);

private:
	Term* currentFunc;
	Term* LoadTerm(std::string input);
	std::vector<char> FindVarInTerm(Term*, std::vector<char>);
	void Destoryer(Term*);
	void Destoryer(Variable*);
};