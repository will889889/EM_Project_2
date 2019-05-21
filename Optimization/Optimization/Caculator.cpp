#include"Caculator.h"
using namespace std;

Caculator::Caculator()
{
	currentFunc = NULL;
}

void Caculator::LoadFunction(string input)
{
	int inputLen = input.length();
	int i = 0;
	Term* func = NULL;
	Term* last = NULL;
	while (i < inputLen)
	{
		// 每次while生產一個term.
		Term* curTerm = new Term;
		curTerm->coe = 1.0f;
		curTerm->vars = NULL;
		curTerm->next = NULL;
		// 連接上一個Term
		if (last != NULL)
			last->next = curTerm;
		else
			func = last = curTerm;

		// 可能的term有: 
		// 有帶號-無var(正負常數)	無帶號-無var(+常數) 
		// 有帶號-有var(正負變數)	無帶號-有var(+變數)	--- 變數可能coe為1, 而無數字.
		// ###特殊函式: sin() 和 cos()

		// 檢查是否帶號
		int sign = 1;
		if (input[i] == '+' || input[i] == '-')
		{
			// 有帶號
			if (input[i] == '-')
				sign = -1;	// 修正為負號
			i++;			// 繼續讀下一字元
		}

		// 檢查是否為數字
		double coe = 1.0f;	// 預設為1.0
		if (input[i] >= 48 && input[i] <= 57)
		{
			// 是數字, 則把整串數字(含小數點)讀完
			string str_coe = "";
			while ((input[i] >= 48 && input[i] <= 57) || input[i] == '.')
			{
				str_coe += input[i];
				i++;
				if (i == inputLen)
					break;
			}

			// 由string轉成double
			istringstream iss(str_coe);
			iss >> coe;
		}
		curTerm->coe = coe * sign;

		// 檢查是否為變數 ###可能是特殊函式形式?
		Variable* lastvar = NULL;
		while (input[i] == '*')
		{
			i++;
			// 是變數, 則產生Var
			Variable* curVar = new Variable;
			curVar->name = input[i];
			curVar->exp = 1.0f;
			curVar->next = NULL;

			if (lastvar != NULL)
				lastvar->next = curVar;
			else
				lastvar = curVar;

			// 連接Term
			curTerm->vars = curVar;

			// 檢查是否有次方
			i++;
			if ((i >= inputLen) && input[i] == '^')
			{
				i++;
				// 將數字全部讀完 
				string str_exp = "";
				while ((input[i] >= 48 && input[i] <= 57) || input[i] == '.')
				{
					str_exp += input[i];
					i++;
					if (i == inputLen)
						break;
				}

				// 由string轉成double
				istringstream iss(str_exp);
				iss >> curVar->exp;
			}

			if (i >= inputLen)
				break;
		}
	}

	currentFunc = func;
}

double Caculator::Caculate(map<char, double> input)
{
	double output = 0.0f;
	Term* curTerm = currentFunc;
	while (curTerm != NULL)
	{
		double curValue = curTerm->coe;	//作為這一整個term的值
		// 檢查是否為變數
		if (curTerm->vars != NULL)
		{
			Variable* curVar = curTerm->vars;
			while (curVar != NULL)
			{
				curValue *= pow(input[curVar->name], curVar->exp);
				curVar = curVar->next;
			}
		}
		output += curValue;
		curTerm = curTerm->next;
	}
	return output;
}