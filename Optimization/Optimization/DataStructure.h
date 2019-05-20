#pragma once
typedef struct Variable
{
	char name;
	double exp;
	Variable* next;
}Variable;

typedef struct Term
{
	double coe;
	Variable* vars;
	Term* next;
}Term;