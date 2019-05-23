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
	// 將輸入的字串轉成Term 並存在 myCurrentFunc
	std::vector<char> LoadFunction(std::string);
	// 將輸入的 input 代入 myCurrentFunc 求值.
	double Caculate(std::map<char, double> input);
	// 將輸入的 input 代入 caculatee 求值. 此方法不會修改myCurrentFunc
	double Caculate(Term* caculatee, std::map<char, double> input);
	// 將輸入的 input 代入 caculatee 求值. 此方法不會修改myCurrentFunc(順序以 caculatees 排序)
	std::vector<double> Caculate(std::vector<Term*> caculatees, std::map<char, double> input);
	// 將輸入的 inFunc 對 toDiff 微分
	Term* PartialDiff(Term* inFunc, char toDiff);
	// 將輸入的 inFunc 對 toDiff 微分, 並代入變數求值.
	double PartialDerivative(Term* inFunc, char toDiff, std::map<char, double> input);
	// 將 myCurrentFunc 對 toDiff 微分, 並代入變數求值.
	double PartialDerivative(char toDiff, std::map<char, double> input);
	// 將輸入的inFunc微分, 求得梯度(為一個vector, 順序以 order 排序)
	std::vector<Term*> Gradient(Term* inFunc, std::vector<char> order);
	// 將輸入的 inFunc 微分, 並代入 input, 求得梯度值(為一個vector, 順序以 order 排序)
	std::vector<double> Gradient(Term* inFunc, std::vector<char> order, std::map<char, double> input);
	// 將 myCurrentFunc 微分, 並代入變數求值.(為一個vector, 順序以 order 排序)
	std::vector<double> Gradient(std::vector<char> order, std::map<char, double> input);
	// delete 輸入的Term, 用來釋放記憶體
	void Destoryer(Term*);
	// delete 輸入的Variable, 用來釋放記憶體
	void Destoryer(Variable*);

private:
	Term* currentFunc;
	Term* LoadTerm(std::string input);
	std::vector<char> FindVarInTerm(Term*, std::vector<char>);
};