#pragma once
enum FuncType
{
	nonFunc,
	sinFunc,
	cosFunc,
};

struct Term;
struct Variable;

typedef struct Variable
{
	char name;
	double exp;
	Variable* next;
	FuncType type;
	Term* FuncPara;
}Variable;

typedef struct Term
{
	double coe;
	Variable* vars;
	Term* next;
}Term;