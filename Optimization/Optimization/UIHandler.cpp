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
double phi = (1 + std::sqrt(5)) / 2;
double resphi = 2 - phi;
double f(double x)
{
	return (x*(x - 1.0));
	//return ((21.0 / 4.0) + (x - (1.0 / 4.0)) * (x - (1.0 / 4.0)));
}
double goldenSectionSearch(double a, double b, double c, double tau)
{
	Answer.push_back(std::to_string(b));
	double x;
	if (c - b > b - a)
		x = b + resphi * (c - b);
	else
		x = b - resphi * (b - a);

	if (std::abs(c - a) < tau * (std::abs(b) + std::abs(x)))
		return (c + a) / 2;
	if (f(x) < f(b))
	{
		if (c - b > b - a)
			return goldenSectionSearch(b, x, c, tau);
		else
			return goldenSectionSearch(a, x, b, tau);
	}
	else
	{
		if (c - b > b - a)
			return goldenSectionSearch(a, b, x, tau);
		else
			return goldenSectionSearch(x, b, c, tau);
	}
}
#pragma endregion

std::vector<std::string> UIHandler::TEST()
{
	Answer.clear();

	Answer.push_back(std::to_string(goldenSectionSearch(0.0, (2.0*resphi), 2.0, 0.1)));
	
	return Answer;
}
