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
		Variable* totalvar = NULL;
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
			{
				lastvar->next = curVar;
				lastvar = curVar;
			}
			else
			{
				totalvar = lastvar = curVar;
			}

			// �P�ɰO���ܼƦW��
			if (find(output.begin(), output.end(), input[i]) == output.end())
				output.push_back(input[i]);

			// �ˬd�O�_���S��fuction
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
		// �s��Term
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
		//�@���o�@���term����
		double curValue = curTerm->coe;
		// �ˬd�O�_���ܼ�
		if (curTerm->vars != NULL)
		{
			// ���W�C���ܼ�(�a�J�B�p��L����᪺��) �Ҧpx*y*z^0.5
			Variable* curVar = curTerm->vars;
			while (curVar != NULL)
			{
				// �ˬd�O�_���禡
				if (curVar->type == FuncType::nonFunc)
				{
					// �D�禡
					curValue *= pow(input[curVar->name], curVar->exp);
				}
				else
				{
					// �O�禡, �N�禡�����`Terms�p�⧹, ����a�J�Ө禡.

					// �ثe���b�B�z��Term
					Term* funcParas = curVar->FuncPara;
					// �`�@Terms����
					double paraValue = 0.0f;
					// ���ƳB�z�C�@��Term, �C���ন�ƭȫ�[��paraValue
					while (funcParas != NULL)
					{
						//�@���o�@���term����
						double curVal = funcParas->coe;
						// �ˬd�O�_���ܼ�
						if (funcParas->vars != NULL)
						{
							// ���W�C���ܼ�(�a�J�B�p��L����᪺��) �Ҧpx*y*z^0.5
							Variable* cv = funcParas->vars;
							while (cv != NULL)
							{
								// �w�]���D�禡
								curVal *= pow(input[cv->name], cv->exp);
								cv = cv->next;
							}
						}
						paraValue += curVal;
						funcParas = funcParas->next;
					}

					// paraValue�w�g��z�n�{ �ҥH�⥦�a��禡:D
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
		// ���ӨC��Term�ˬd�O�_�]�ttoDiff, �Y�S���h���Q�g�JcurOut.

		// �{�b���bŪ��var
		Variable* curReadVar = curRead->vars;
		// Ū����O�d��var
		Variable* diffVar = NULL;
		// ���bŪ��var����
		Variable* lastDiffVar = diffVar;
		// �ˬdŪ����O�_�ݭn�O�d��Term
		bool isRemained = false;
		// �Y�O�d(�N��O�L��), �h�ݭ��W���Ѽ�
		double remained_coe = 1.0f;
		while (curReadVar != NULL)
		{
			// Ū���C�@���ܼ�(�w�]�O�d�Ҧ��ܼ�, �Y��̫ᤣ�Oremained�A�R��)
			
			// �C���ܼƳ��n�����s�W��(�ȫ���input���ܼƭ�)
			Variable* n_var = new Variable(*curReadVar);
			n_var->next = NULL;
			

			// �Y�L��, �B�L���᪺x�Y�Ƭ�0, �h�ݭn�Nx�R��
			bool is_turnToConst = false;

			// �ھ�var���ʽ谵�ק� (���@�`�ƪ�var�h���ק�)
			if (curReadVar->name == toDiff)
			{
				// �N��Term�|�O�d
				isRemained = true;
				
				// �N�{�b��var�L��
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
			
			// �O�d�ܼ�
			if (!is_turnToConst)
			{
				if (diffVar == NULL)
					diffVar = lastDiffVar = n_var;
				else
					lastDiffVar->next = n_var;
				lastDiffVar = lastDiffVar->next;
			}

			// Ū���U�@��
			curReadVar = curReadVar->next;
		}

		// ���]�ttoDiff, ��O�g�J
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
			// �L�]�t, ��������:D(�ȰO����e��)
			Destoryer(diffVar);
		}

		// �ǳ�Ū�U�@��Term
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

vector<char> Caculator::FindVarInTerm(Term* input, vector<char> original)
{
	vector<char> output = original;
	Term* curT = input;
	while (curT != NULL)
	{
		Variable* curVar = curT->vars;
		while (curVar != NULL)
		{
			// �P�ɰO���ܼƦW��
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