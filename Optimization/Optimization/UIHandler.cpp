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
