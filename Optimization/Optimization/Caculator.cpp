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
		double coe = 1.0f;	// �w�]��1.0
		if (input[i] >= 48 && input[i] <= 57)
		{
			// �O�Ʀr, �h����Ʀr(�t�p���I)Ū��
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
		while (input[i] == '*')
		{
			i++;
			// �O�ܼ�, �h����Var
			Variable* curVar = new Variable;
			curVar->name = input[i];
			curVar->exp = 1.0f;
			curVar->next = NULL;

			if (lastvar != NULL)
				lastvar->next = curVar;
			else
				lastvar = curVar;

			// �s��Term
			curTerm->vars = curVar;

			// �ˬd�O�_������
			i++;
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

	currentFunc = func;
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