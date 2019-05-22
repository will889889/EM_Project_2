#include"Caculator.h"
using namespace std;

Caculator::Caculator()
{
	currentFunc = NULL;
}

Term* Caculator::myCurrentFunc()
{
	return currentFunc;
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
		Variable* totalvar = NULL;
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
			{
				lastvar->next = curVar;
				lastvar = curVar;
			}
			else
			{
				totalvar = lastvar = curVar;
			}

			// 同時記錄變數名稱
			if (find(output.begin(), output.end(), input[i]) == output.end())
				output.push_back(input[i]);

			// 檢查是否為特殊fuction
			if (i + 2 < inputLen)
			{
				if (input.substr(i, 3) == "sin")
				{
					curVar->type = FuncType::sinFunc;
				}
				else if (input.substr(i, 3) == "cos")
				{
					curVar->type = FuncType::cosFunc;
				}
				if (curVar->type != FuncType::nonFunc)
				{
					output.pop_back();
					int endPos = input.find(')', i + 2);
					string infuncstr = input.substr(i + 4, endPos - i - 4);
					Term* infuncTerm = LoadTerm(infuncstr);
					curVar->FuncPara = infuncTerm;
					i = endPos;
					output = FindVarInTerm(infuncTerm, output);
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
		// 連接Term
		curTerm->vars = totalvar;
	}
	Destoryer(currentFunc);
	currentFunc = func;
	return output;
}

double Caculator::Caculate(map<char, double> input)
{
	double output = 0.0f;
	Term* curTerm = currentFunc;
	while (curTerm != NULL)
	{
		//作為這一整個term的值
		double curValue = curTerm->coe;
		// 檢查是否為變數
		if (curTerm->vars != NULL)
		{
			// 乘上每個變數(帶入且計算過次方後的值) 例如x*y*z^0.5
			Variable* curVar = curTerm->vars;
			while (curVar != NULL)
			{
				// 檢查是否為函式
				if (curVar->type == FuncType::nonFunc)
				{
					// 非函式
					curValue *= pow(input[curVar->name], curVar->exp);
				}
				else
				{
					// 是函式, 將函式內的總Terms計算完, 之後帶入該函式.

					// 目前正在處理的Term
					Term* funcParas = curVar->FuncPara;
					// 總共Terms的值
					double paraValue = 0.0f;
					// 重複處理每一個Term, 每次轉成數值後加到paraValue
					while (funcParas != NULL)
					{
						//作為這一整個term的值
						double curVal = funcParas->coe;
						// 檢查是否為變數
						if (funcParas->vars != NULL)
						{
							// 乘上每個變數(帶入且計算過次方後的值) 例如x*y*z^0.5
							Variable* cv = funcParas->vars;
							while (cv != NULL)
							{
								// 預設為非函式
								curVal *= pow(input[cv->name], cv->exp);
								cv = cv->next;
							}
						}
						paraValue += curVal;
						funcParas = funcParas->next;
					}

					// paraValue已經整理好ㄌ 所以把它帶到函式:D
					if (curVar->type == FuncType::sinFunc)
						curValue *= sin(paraValue);
					else if (curVar->type == FuncType::cosFunc)
						curValue *= cos(paraValue);
				}

				curVar = curVar->next;
			}
		}
		output += curValue;
		curTerm = curTerm->next;
	}
	return output;
}

double Caculator::Caculate(Term* caculatee, std::map<char, double> input)
{
	Term* memoryFunc = currentFunc;
	currentFunc = caculatee;
	double output = Caculate(input);
	currentFunc = memoryFunc;
	return output;
}

Term* Caculator::PartialDiff(Term* inFunc, char toDiff)
{
	Term* output = new Term;
	output->coe = 0.0f;
	output->vars = NULL;
	output->next = NULL;
	Term* curOut = output;
	Term* curRead = inFunc;
	while (curRead != NULL)
	{
		// 按照每個Term檢查是否包含toDiff, 若沒有則不被寫入curOut.

		// 現在正在讀的var
		Variable* curReadVar = curRead->vars;
		// 讀完後保留的var
		Variable* diffVar = NULL;
		// 正在讀的var指標
		Variable* lastDiffVar = diffVar;
		// 檢查讀完後是否需要保留此Term
		bool isRemained = false;
		// 若保留(代表是微分), 則需乘上此參數
		double remained_coe = 1.0f;
		while (curReadVar != NULL)
		{
			// 讀取每一個變數(預設保留所有變數, 若到最後不是remained再刪掉)
			
			// 每個變數都要做為新增的(怕指到input的變數們)
			Variable* n_var = new Variable(*curReadVar);
			n_var->next = NULL;
			

			// 若微分, 且微分後的x係數為0, 則需要將x刪除
			bool is_turnToConst = false;

			// 根據var的性質做修改 (視作常數的var則不修改)
			if (curReadVar->name == toDiff)
			{
				// 代表此Term會保留
				isRemained = true;
				
				// 將現在的var微分
				remained_coe = n_var->exp;
				n_var->exp = n_var->exp - 1.0f;
				is_turnToConst = (n_var->exp == 0);
			}
			else
			{
				if (curReadVar->type == FuncType::sinFunc)
				{

				}
				else if (curReadVar->type == FuncType::cosFunc)
				{

				}
			}
			
			// 保留變數
			if (!is_turnToConst)
			{
				if (diffVar == NULL)
					diffVar = lastDiffVar = n_var;
				else
					lastDiffVar->next = n_var;
				lastDiffVar = lastDiffVar->next;
			}

			// 讀取下一個
			curReadVar = curReadVar->next;
		}

		// 有包含toDiff, 於是寫入
		if (isRemained)
		{
			curOut->next = new Term;
			curOut = curOut->next;
			curOut->coe = curRead->coe * remained_coe;
			curOut->vars = diffVar;
			curOut->next = NULL;
		}
		else
		{
			// 無包含, 毀掉全部:D(怕記憶體占用)
			Destoryer(diffVar);
		}

		// 準備讀下一個Term
		curRead = curRead->next;
	}
	curOut = output;
	output = output->next;
	delete curOut;
	return output;
}

double Caculator::PartialDerivative(Term* inFunc, char toDiff, std::map<char, double> input)
{
	Term* diffed = PartialDiff(inFunc, toDiff);
	double output = Caculate(diffed, input);
	Destoryer(diffed);
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

vector<char> Caculator::FindVarInTerm(Term* input, vector<char> original)
{
	vector<char> output = original;
	Term* curT = input;
	while (curT != NULL)
	{
		Variable* curVar = curT->vars;
		while (curVar != NULL)
		{
			// 同時記錄變數名稱
			if (find(output.begin(), output.end(), curVar->name) == output.end())
				output.push_back(curVar->name);
			curVar = curVar->next;
		}
		curT = curT->next;
	}
	return output;
}

void Caculator::Destoryer(Term* input)
{
	if (input != NULL)
	{
		Term* last = input;
		while (input->next != NULL)
		{
			input = input->next;
			if (last->vars != NULL)
			{
				Destoryer(last->vars);
			}
			delete last;
			last = input;
		}
		delete last;
	}
}

void Caculator::Destoryer(Variable* input)
{
	if (input != NULL)
	{
		Variable* last = input;
		while (input->next != NULL)
		{
			input = input->next;
			delete last;
			last = input;
		}
		delete last;
	}
}