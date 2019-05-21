#include "UIHandler.h"
#include <sstream>

std::pair<double, double> FindInterval(std::vector<double> & _vector, std::vector<double> & _origin, std::vector<std::pair<double, double>> & _intervals)
{
	std::vector<std::pair<double, double>> bounds;
	bounds.clear();

	//	find each bound of unit
	for (int i = 0; i < _vector.size(); i++)
	{
		std::pair<double, double> bound;

		if (_vector[i] == 0)
		{
			bound.first = -DBL_MAX;
			bound.second = DBL_MAX;
		}
		else
		{
			bound.first = (_intervals[i].first - _origin[i]) / _vector[i];
			bound.second = (_intervals[i].second - _origin[i]) / _vector[i];

			if (bound.first > bound.second)
			{
				double temp = bound.first;
				bound.first = bound.second;
				bound.second = temp;
			}
		}

		bounds.push_back(bound);
	}

	std::pair<double, double> FixedBound;
	FixedBound = bounds[0];
	for (int i = 1; i < _vector.size(); i++)
	{
		if (FixedBound.first < bounds[i].first)
			FixedBound.first = bounds[i].first;
		if (FixedBound.second > bounds[i].second)
			FixedBound.second = bounds[i].second;
	}

	return FixedBound;
}

UIHandler::UIHandler()
{
	m_Input = "";
	equations.clear();
	m_Method = Powell;
}

std::vector<std::string> UIHandler::DoMath()
{
	std::stringstream sstream;

	sstream << m_Input;
	//	get equation index
	sstream >> equationIndex;
	//	get equation
	Variables = m_Calculator.LoadFunction(equations[equationIndex]);
	//	get varsCount (from equation)
	varsCount = Variables.size();
	//	get initPoint
	initPoint.clear();
	for (int i = 0; i < varsCount; i++)
	{
		double in;
		sstream >> in;
		initPoint.push_back(in);
	}
	//	get intervals
	intervals.clear();
	for (int i = 0; i < varsCount; i++)
	{
		std::pair<double, double> interval;
		sstream >> interval.first;
		sstream >> interval.second;
		intervals.push_back(interval);
	}


	Answer.clear();
	switch (m_Method)
	{
	case Powell:
		Answer.push_back("[Powell] : \t" + equations[equationIndex]);

		break;
	case Newton:
		Answer.push_back("[Newton] : \t" + equations[equationIndex]);
		break;
	case SteepDescent:
		Answer.push_back("[SteepDescent] : \t" + equations[equationIndex]);

		break;
	case QuasiNewton:
		Answer.push_back("[QuasiNewton] : \t" + equations[equationIndex]);

		break;
	case ConjugateGradient:
		Answer.push_back("[ConjugateGradient] : \t" + equations[equationIndex]);

		break;
	default:
		Answer.push_back("[Method Error]");
		std::cout << "[Method Error]";
		return Answer;
		break;
	}

	std::string initString;
	initString = "init : \t[ ";
	for (int i = 0; i < varsCount; i++)
	{
		initString += std::to_string(initPoint[i]);

		if (i == (varsCount - 1))
		{
			initString += " ]";
			continue;
		}
		initString += " ,\t";
	}

	Answer.push_back(initString);
	for (int i = 0; i < varsCount; i++)
	{
		Answer.push_back("bound : \t[ " + std::to_string(intervals[i].first) + " ,\t" + std::to_string(intervals[i].second) + " ]");
	}
	Answer.push_back("");
	Answer.push_back("ans...blahblahblah");

	return Answer;
}

#pragma region GoldenSection
double UIHandler::CalculateByCoef(double coef)
{
	////////	this needs [Variables], [Position], [Direction] from global
	std::map<char, double> vars;
	for (int i = 0; i < varsCount; i++)
	{
		vars.insert(std::pair<char, double>(Variables[i], Position[i] + Direction[i] * coef));
	}
	return m_Calculator.Caculate(vars);
}
double UIHandler::CalculateByCoordinate(std::vector<double> coor)
{
	std::map<char, double> vars;
	for (int i = 0; i < varsCount; i++)
	{
		vars.insert(std::pair<char, double>(Variables[i], coor[i]));
	}
	return m_Calculator.Caculate(vars);
}

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
//	here, tau means the threshold of delta( f(b1), f(b2) )
double UIHandler::goldenSectionSearch(double a, double b, double c, double tau)
{
	//	find x (b2)
	Bias_Left = (std::abs(c - b) > std::abs(b - a));	//	b is on left

	if (Bias_Left)
		x = c - resphi * (c - a);
	else
		x = a + resphi * (c - a);

	//Answer.push_back(" x: " + std::to_string(b) + " , b: " + std::to_string(x) + " , ca: " + std::to_string(std::abs(a - c)));

	//	calculate (get y)
	fof_x = CalculateByCoef(x);
	fof_b = CalculateByCoef(b);

	//	threshold - delta( f(b1), f(b2) )
	if ((std::abs(fof_x - fof_b) < tau))
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
#pragma endregion

std::vector<std::string> UIHandler::TEST()
{
	Answer.clear();

	std::stringstream sstream;

	sstream << m_Input;
	//	get equation index
	sstream >> equationIndex;
	//	get equation
	Variables = m_Calculator.LoadFunction(equations[equationIndex]);
	//	get varsCount (from equation)
	varsCount = Variables.size();
	//	get initPoint
	initPoint.clear();
	for (int i = 0; i < varsCount; i++)
	{
		double in;
		sstream >> in;
		initPoint.push_back(in);
	}

	Answer.push_back("Func: " + equations[equationIndex]);
	Answer.push_back("VarsCount = " + std::to_string(varsCount));
	Answer.push_back("Value = " + std::to_string(CalculateByCoordinate(initPoint)));

	//Answer.push_back(std::to_string(goldenSectionSearch(-0.5, (5.0*resphi - 0.5), 4.5, 0.00000001)) + " <- ans");
	//Answer.push_back(std::to_string(goldenSectionSearch(0.3, (2.7*resphi + 0.3), 3.0, 0.00000001)) + " <- ans");

	return Answer;
}

void UIHandler::FixEquation()
{
	Answer.clear();

	std::stringstream sstream;

	sstream << m_Input;
	//	get equation index
	sstream >> equationIndex;

	Answer.push_back("Equation [" + std::to_string(equationIndex) + "] fixed.");
	Answer.push_back("from\t" + equations[equationIndex]);

	sstream >> equations[equationIndex];

	Answer.push_back("to\t" + equations[equationIndex]);
	Answer.push_back("(Click 'Clear' to reload)");
}

//	Powell¡¦s "quadratically" convergent method
std::vector<std::string> UIHandler::Powell_method()
{
	std::vector<std::string> Info;

	std::vector<std::vector<double>> Directions;
	Directions.clear();
	//	set all directions to unit vectors
	for (int i = 0; i < varsCount; i++)
	{
		std::vector<double> direction;
		direction.clear();
		for (int j = 0; j < varsCount; j++)
		{
			if (i == j)
				direction.push_back(1.0);
			else
				direction.push_back(0.0);
		}
		Directions.push_back(direction);
	}
	//
	Position = initPoint;
	std::vector<double> LastPosition;
	double Value;
	double LastValue;

	double DELTA_X_THRESHOLD = 0.00000001;
	double DELTA_Y_THRESHOLD = 0.00000001;
	int ITER_THRESHOLD = 10000;
	for (int iter = 1; iter < ITER_THRESHOLD; iter++)
	{
		//	j = ?

		LastPosition = Position;

		//	The "Step 2"
		for (int i = 0; i < varsCount; i++)
		{
			double Coefficient;
			std::pair<double, double> Interval;
			Interval = FindInterval(Directions[i], Position, intervals);
			//	the calculation should know the: Direction, Position
			Direction = Directions[i];
			Coefficient = goldenSectionSearch(Interval.first, Interval.first + ((Interval.second - Interval.first)*resphi), Interval.second, DELTA_Y_THRESHOLD);
			
			///	have a vector
			///	have a origin coordinate
			///	have all intervals of all coordinates
			///	(get the interval of the coefficient of the vector)
			///	(find the exact coefficient)

		}
		//	threshold
		/*if ()
		{
			break;
		}*/
	}


	//	minimizer
	Answer.push_back("");
	Answer.push_back("minimizer:");
	std::string miniString;
	miniString = "[ ";
	for (int i = 0; i < varsCount; i++)
	{
		miniString += std::to_string(Position[i]);

		if (i == (varsCount - 1))
		{
			miniString += " ]";
			continue;
		}
		miniString += " ,\t";
	}
	Answer.push_back(miniString);
	//	value
	Answer.push_back("value:");
	///Answer.push_back(std::to_string(f(Position)));

	return Info;
}


#pragma region TrashCan
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
