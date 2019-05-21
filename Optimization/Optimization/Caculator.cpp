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
		// �C��while�Ͳ��@��term.
		Term* curTerm = new Term;
		curTerm->coe = 1.0f;
		curTerm->vars = NULL;
		curTerm->next = NULL;
		// �s���W�@��Term
		if (last != NULL) 
		{
			last->next = curTerm;
			last = curTerm;
		}
		else
			func = last = curTerm;

		// �i�઺term��: 
		// ���a��-�Lvar(���t�`��)	�L�a��-�Lvar(+�`��) 
		// ���a��-��var(���t�ܼ�)	�L�a��-��var(+�ܼ�)	--- �ܼƥi��coe��1, �ӵL�Ʀr.
		// ###�S��禡: sin() �M cos()

		// �ˬd�O�_�a��
		int sign = 1;
		if (input[i] == '+' || input[i] == '-')
		{
			// ���a��
			if (input[i] == '-')
				sign = -1;	// �ץ����t��
			i++;			// �~��Ū�U�@�r��
		}

		// �ˬd�O�_���Ʀr
		bool has_coe = false;
		double coe = 1.0f;	// �w�]��1.0
		if (input[i] >= 48 && input[i] <= 57)
		{
			// �O�Ʀr, �h����Ʀr(�t�p���I)Ū��
			has_coe = true;
			string str_coe = "";
			while ((input[i] >= 48 && input[i] <= 57) || input[i] == '.')
			{
				str_coe += input[i];
				i++;
				if (i == inputLen)
					break;
			}

			// ��string�নdouble
			istringstream iss(str_coe);
			iss >> coe;
		}
		curTerm->coe = coe * sign;

		// �ˬd�O�_���ܼ� ###�i��O�S��禡�Φ�?
		Variable* lastvar = NULL;
		while (input[i] == '*' || (!has_coe))
		{
			if(has_coe)
				i++;
			has_coe = true;
			// �O�ܼ�, �h����Var
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

			// �P�ɰO���ܼƦW��
			if (find(output.begin(), output.end(), input[i]) == output.end())
				output.push_back(input[i]);

			// �s��Term
			curTerm->vars = curVar;

			// �ˬd�O�_���S��fuction
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

			// �~����U�@�r��
			i++;

			// �ˬd�O�_������
			if ((i < inputLen) && input[i] == '^')
			{
				i++;
				// �N�Ʀr����Ū�� 
				string str_exp = "";
				while ((input[i] >= 48 && input[i] <= 57) || input[i] == '.')
				{
					str_exp += input[i];
					i++;
					if (i == inputLen)
						break;
				}

				// ��string�নdouble
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
		double curValue = curTerm->coe;	//�@���o�@���term����
		// �ˬd�O�_���ܼ�
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
		// �C��while�Ͳ��@��term.
		Term* curTerm = new Term;
		curTerm->coe = 1.0f;
		curTerm->vars = NULL;
		curTerm->next = NULL;
		// �s���W�@��Term
		if (last != NULL)
			last->next = curTerm;
		else
			func = last = curTerm;

		// �i�઺term��: 
		// ���a��-�Lvar(���t�`��)	�L�a��-�Lvar(+�`��) 
		// ���a��-��var(���t�ܼ�)	�L�a��-��var(+�ܼ�)	--- �ܼƥi��coe��1, �ӵL�Ʀr.
		// ###�S��禡: sin() �M cos()

		// �ˬd�O�_�a��
		int sign = 1;
		if (input[i] == '+' || input[i] == '-')
		{
			// ���a��
			if (input[i] == '-')
				sign = -1;	// �ץ����t��
			i++;			// �~��Ū�U�@�r��
		}

		// �ˬd�O�_���Ʀr
		bool has_coe = false;
		double coe = 1.0f;	// �w�]��1.0
		if (input[i] >= 48 && input[i] <= 57)
		{
			// �O�Ʀr, �h����Ʀr(�t�p���I)Ū��
			has_coe = true;
			string str_coe = "";
			while ((input[i] >= 48 && input[i] <= 57) || input[i] == '.')
			{
				str_coe += input[i];
				i++;
				if (i == inputLen)
					break;
			}

			// ��string�নdouble
			istringstream iss(str_coe);
			iss >> coe;
		}
		curTerm->coe = coe * sign;

		// �ˬd�O�_���ܼ� ###�i��O�S��禡�Φ�?
		Variable* lastvar = NULL;
		while (input[i] == '*' || (!has_coe))
		{
			if (has_coe)
				i++;
			// �O�ܼ�, �h����Var
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

			// �s��Term
			curTerm->vars = curVar;

			// ���ˬd�O�_���S��fuction

			// �~����U�@�r��
			i++;

			// �ˬd�O�_������
			if ((i >= inputLen) && input[i] == '^')
			{
				i++;
				// �N�Ʀr����Ū�� 
				string str_exp = "";
				while ((input[i] >= 48 && input[i] <= 57) || input[i] == '.')
				{
					str_exp += input[i];
					i++;
					if (i == inputLen)
						break;
				}

				// ��string�নdouble
				istringstream iss(str_exp);
				iss >> curVar->exp;
			}

			if (i >= inputLen)
				break;
		}
	}
	return func;
}