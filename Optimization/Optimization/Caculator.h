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
	// �N��J���r���নTerm �æs�b myCurrentFunc
	std::vector<char> LoadFunction(std::string);
	// �N��J�� input �N�J myCurrentFunc �D��.
	double Caculate(std::map<char, double> input);
	// �N��J�� input �N�J caculatee �D��. ����k���|�ק�myCurrentFunc
	double Caculate(Term* caculatee, std::map<char, double> input);
	// �N��J�� input �N�J caculatee �D��. ����k���|�ק�myCurrentFunc(���ǥH caculatees �Ƨ�)
	std::vector<double> Caculate(std::vector<Term*> caculatees, std::map<char, double> input);
	// �N��J�� inFunc �� toDiff �L��
	Term* PartialDiff(Term* inFunc, char toDiff);
	// �N��J�� inFunc �� toDiff �L��, �åN�J�ܼƨD��.
	double PartialDerivative(Term* inFunc, char toDiff, std::map<char, double> input);
	// �N myCurrentFunc �� toDiff �L��, �åN�J�ܼƨD��.
	double PartialDerivative(char toDiff, std::map<char, double> input);
	// �N��J��inFunc�L��, �D�o���(���@��vector, ���ǥH order �Ƨ�)
	std::vector<Term*> Gradient(Term* inFunc, std::vector<char> order);
	// �N��J�� inFunc �L��, �åN�J input, �D�o��׭�(���@��vector, ���ǥH order �Ƨ�)
	std::vector<double> Gradient(Term* inFunc, std::vector<char> order, std::map<char, double> input);
	// �N myCurrentFunc �L��, �åN�J�ܼƨD��.(���@��vector, ���ǥH order �Ƨ�)
	std::vector<double> Gradient(std::vector<char> order, std::map<char, double> input);
	// delete ��J��Term, �Ψ�����O����
	void Destoryer(Term*);
	// delete ��J��Variable, �Ψ�����O����
	void Destoryer(Variable*);

private:
	Term* currentFunc;
	Term* LoadTerm(std::string input);
	std::vector<char> FindVarInTerm(Term*, std::vector<char>);
};