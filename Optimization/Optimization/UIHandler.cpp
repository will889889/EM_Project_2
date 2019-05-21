#include "UIHandler.h"
#include <sstream>

UIHandler::UIHandler()
{
	m_Input = "";
	equations.clear();
	m_Method = Powell;
}

std::vector<std::string> UIHandler::DoMath()
{
	std::vector<std::string> Answer;
	Answer.clear();

	std::stringstream sstream;
	///	get these
	int equationIndex;
	double initPoint;
	std::vector< std::pair<double, double> > intervals;

	sstream << m_Input;
	sstream >> equationIndex;
	/// variable amount needed
	sstream >> initPoint;

	///	Equation analysis >> get variable amount
	int varsCount = 2; ///
	intervals.clear();
	for (int i = 0; i < varsCount; i++)
	{
		std::pair<double, double> interval;
		sstream >> interval.first;
		sstream >> interval.second;
		intervals.push_back(interval);
	}

	switch (m_Method)
	{
	case Powell:
		Answer.push_back("[Powell] : " + equations[equationIndex]);
		
		break;
	case Newton:
		Answer.push_back("[Newton] : " + equations[equationIndex]);
		break;
	case SteepDescent:
		Answer.push_back("[SteepDescent] : " + equations[equationIndex]);

		break;
	case QuasiNewton:
		Answer.push_back("[QuasiNewton] : " + equations[equationIndex]);

		break;
	case ConjugateGradient:
		Answer.push_back("[ConjugateGradient] : " + equations[equationIndex]);

		break;
	default:
		Answer.push_back("[Method Error]");
		std::cout << "[Method Error]";
		return Answer;
		break;
	}

	Answer.push_back("init : " + std::to_string(initPoint));
	for (int i = 0; i < varsCount; i++)
	{
		Answer.push_back("bound : [" + std::to_string(intervals[i].first) + " , " + std::to_string(intervals[i].first) + "]");
	}
	Answer.push_back("");
	Answer.push_back("ans...\nblahblahblah");

	return Answer;
}


std::vector<std::string> Answer;

#pragma region GoldenSection
//	The function
double f(double x)
{
	return (std::sin(3.0*x) + std::cos(x));
	//return (x*(x - 1.0));
	//return ((21.0 / 4.0) + (x - (1.0 / 4.0)) * (x - (1.0 / 4.0)));
}
///	boost
double phi = (1 + std::sqrt(5)) / 2;	// 1.6xx
double resphi = 2 - phi;	// 0.3xx
double fof_x, fof_b, x;
bool Bias_Left;
///	boost
int Iter_Threshold = 1000;
int Iter_Count;
//	here, tau means the threshold of delta( f(b1), f(b2) )
double goldenSectionSearch(double a, double b, double c, double tau)
{
	//	find x (b2)
	Bias_Left = (std::abs(c - b) > std::abs(b - a));	//	b is on left

	if (Bias_Left)
		x = c - resphi * (c - a);
	else
		x = a + resphi * (c - a);

	//Answer.push_back(" x: " + std::to_string(b) + " , b: " + std::to_string(x) + " , ca: " + std::to_string(std::abs(a - c)));

	//	calculate (get y)
	fof_x = f(x);
	fof_b = f(b);

	//	threshold - delta( f(b1), f(b2) )
	if ((std::abs(fof_x - fof_b) < tau) || (Iter_Count > Iter_Threshold))
		return (x + b) / 2;
	////	threshold - delta( b1, b2 )
	//if (std::abs(a - c) < tau)
	//	return (x + b) / 2;

	//	compare
	if (fof_x < fof_b)
	{
		if (Bias_Left)	//	if b1 is on left
			return goldenSectionSearch(b, x, c, tau);
		else
			return goldenSectionSearch(a, x, b, tau);
	}
	else
	{
		if (Bias_Left)
			return goldenSectionSearch(a, b, x, tau);
		else
			return goldenSectionSearch(x, b, c, tau);
	}
}
//double goldenSectionSearch(double a, double b, double c, double tau)
//{
//	//Answer.push_back(std::to_string(b));
//	//	find x (b2)
//	double x;
//	if (c - b > b - a)
//		x = b + resphi * (c - b);
//	else
//		x = b - resphi * (b - a);
//
//	//	threshold
//	if (std::abs(c - a) < tau * (std::abs(b) + std::abs(x)))
//		return (c + a) / 2;
//
//	//	compare
//	if (f(x) < f(b))
//	{
//		if (c - b > b - a)
//			return goldenSectionSearch(b, x, c, tau);
//		else
//			return goldenSectionSearch(a, x, b, tau);
//	}
//	else
//	{
//		if (c - b > b - a)
//			return goldenSectionSearch(a, b, x, tau);
//		else
//			return goldenSectionSearch(x, b, c, tau);
//	}
//}
#pragma endregion

std::vector<std::string> UIHandler::TEST()
{
	Answer.clear();
	Iter_Count = 0;

	//Answer.push_back(std::to_string(goldenSectionSearch(-0.5, (5.0*resphi - 0.5), 4.5, 0.00000001)) + " <- ans");
	Answer.push_back(std::to_string(goldenSectionSearch(0.3, (2.7*resphi + 0.3), 3.0, 0.00000001)) + " <- ans");

	return Answer;
}
