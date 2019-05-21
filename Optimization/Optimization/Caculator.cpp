#include"Caculator.h"
using namespace std;

Caculator::Caculator()
{
	currentFunc = NULL;
}

std::vector<char> Caculator::LoadFunction(string input)
{
	vector<char> output;

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
		{
			last->next = curTerm;
			last = curTerm;
		}
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
		bool has_coe = false;
		double coe = 1.0f;	// 預設為1.0
		if (input[i] >= 48 && input[i] <= 57)
		{
			// 是數字, 則把整串數字(含小數點)讀完
			has_coe = true;
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
		while (input[i] == '*' || (!has_coe))
		{
			if(has_coe)
				i++;
			has_coe = true;
			// 是變數, 則產生Var
			Variable* curVar = new Variable;
			curVar->name = input[i];
			curVar->exp = 1.0f;
			curVar->next = NULL;
			curVar->type = FuncType::nonFunc;
			curVar->FuncPara = NULL;

			if (lastvar != NULL)
				lastvar->next = curVar;
			else
				lastvar = curVar;

			// 同時記錄變數名稱
			if (find(output.begin(), output.end(), input[i]) == output.end())
				output.push_back(input[i]);

			// 連接Term
			curTerm->vars = curVar;

			// 檢查是否為特殊fuction
			if (i + 2 < inputLen)
			{
				if (input.substr(i, 3) == "sin")
				{
					curVar->type = FuncType::sinFunc;
				}
				else if (input.substr(i, i + 2) == "cos")
				{
					curVar->type = FuncType::cosFunc;
				}
				if (curVar->type != FuncType::nonFunc)
				{
					int endPos = input.find(')', i + 2);
					string infuncstr = input.substr(i + 4, endPos - i - 4);
					Term* infuncTerm = LoadTerm(infuncstr);
					curVar->FuncPara = infuncTerm;
				}
			}

			// 繼續取下一字元
			i++;

			// 檢查是否有次方
			if ((i < inputLen) && input[i] == '^')
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
	return output;
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

Term* Caculator::LoadTerm(string input)
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
		bool has_coe = false;
		double coe = 1.0f;	// 預設為1.0
		if (input[i] >= 48 && input[i] <= 57)
		{
			// 是數字, 則把整串數字(含小數點)讀完
			has_coe = true;
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
		while (input[i] == '*' || (!has_coe))
		{
			if (has_coe)
				i++;
			// 是變數, 則產生Var
			Variable* curVar = new Variable;
			curVar->name = input[i];
			curVar->exp = 1.0f;
			curVar->next = NULL;
			curVar->type = FuncType::nonFunc;
			curVar->FuncPara = NULL;

			if (lastvar != NULL)
				lastvar->next = curVar;
			else
				lastvar = curVar;

			// 連接Term
			curTerm->vars = curVar;

			// 不檢查是否為特殊fuction

			// 繼續取下一字元
			i++;

			// 檢查是否有次方
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
	return func;
}