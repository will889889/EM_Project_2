#include "UIHandler.h"
#include <sstream>
#include "Mathemagic.h"

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

	Answer.push_back("init : \t" + Vector2String(initPoint));
	for (int i = 0; i < varsCount; i++)
	{
		Answer.push_back("bound : \t[ " + std::to_string(intervals[i].first) + " ,\t" + std::to_string(intervals[i].second) + " ]");
	}

	switch (m_Method)
	{
	case Powell:
		Answer.push_back("[Powell] : \t" + equations[equationIndex]);
		Powell_method();
		break;
	case Newton:
		Answer.push_back("[Newton] : \t" + equations[equationIndex]);
		Newton_method();
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

	Answer.push_back("");
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
double UIHandler::CalculateByCoordinate(std::vector<double>& coor, Term * term)
{
	std::map<char, double> vars;
	for (int i = 0; i < varsCount; i++)
	{
		vars.insert(std::pair<char, double>(Variables[i], coor[i]));
	}
	return m_Calculator.Caculate(term, vars);
}
double UIHandler::CalculateByCoordinate(std::vector<double>& coor)
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
	std::cout << b << "\n";

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

	///	NAN case:
	if (fof_x == NAN)
	{
		if (Bias_Left)
			return goldenSectionSearch(a, b, x, tau);
		else
			return goldenSectionSearch(x, b, c, tau);
	}
	if (fof_b == NAN)
	{
		if (Bias_Left)
			return goldenSectionSearch(b, x, c, tau);
		else
			return goldenSectionSearch(a, x, b, tau);
	}

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

	std::cout << "Hello\n";

	//	get equation
	Variables = m_Calculator.LoadFunction(equations[equationIndex]);
	
	// WILL: for test diff
	/*std::map<char, double> will_input;
	will_input['x'] = 1.0f;
	will_input['y'] = 2.0f;
	will_input['z'] = 3.0f;
	will_input['a'] = 4.0f;
	double test_term = m_Calculator.PartialDerivative(m_Calculator.myCurrentFunc(), Variables[0], will_input);
	*/
	// WILL end

	std::cout << "Hello 2\n";

	//	get varsCount (from equation)
	varsCount = Variables.size();

	std::cout << "Hello 3\n";

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

	while (equationIndex >= equations.size())
	{
		equations.push_back("[new equation]");
	}

	Answer.push_back("Equation [" + std::to_string(equationIndex) + "] fixed.");
	Answer.push_back("from\t" + equations[equationIndex]);

	sstream >> equations[equationIndex];

	Answer.push_back("to\t" + equations[equationIndex]);
	Answer.push_back("(Click 'Clear' to reload)");
}

//	Powell¡¦s "quadratically" convergent method
void UIHandler::Powell_method()
{
	std::vector<std::string> Info;
	Answer.push_back("");
	Answer.push_back("");

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
	double Value = CalculateByCoordinate(Position);
	double LastValue;

	double DELTA_X_THRESHOLD = 0.00000001;
	double DELTA_Y_THRESHOLD = 0.00000001;
	int ITER_THRESHOLD = 10000;
	for (int iter = 1; iter < ITER_THRESHOLD; iter++)
	{
		Answer.push_back("j = " + std::to_string(iter));

		LastPosition = Position;
		LastValue = Value;

		//	The "Step 2"
		for (int i = 0; i < varsCount; i++)
		{
			Answer.push_back("i = " + std::to_string(i));
			Answer.push_back("X: " + Vector2String(Position));

			double Coefficient;
			std::pair<double, double> Interval;

			Interval = FindInterval(Directions[i], Position, intervals);
			//	the calculation should know the: Direction, Position
			Direction = Directions[i];
			Coefficient = goldenSectionSearch(Interval.first, Interval.first + ((Interval.second - Interval.first)*resphi), Interval.second, DELTA_Y_THRESHOLD);
			//
			std::cout << "EO goldenSectionSearch\n";
			Position = v_Sum(Position, v_Multiply(Direction, Coefficient));
		}
		Answer.push_back("i = " + std::to_string(varsCount));
		Answer.push_back("X: " + Vector2String(Position));
		///
		std::cout << "CCC\n";
		Value = CalculateByCoordinate(Position);

		//	quadra scheme~
		for (int i = 0; i < (varsCount-1); i++)
		{
			Directions[i] = Directions[i + 1];
		}
		Direction = v_Subtract(Position, LastPosition);
		Directions[varsCount - 1] = Direction;
		
		//	Golden again
		double Coefficient;
		std::pair<double, double> Interval;
		Interval = FindInterval(Direction, Position, intervals);
		///
		std::cout << "GGG\n";
		Coefficient = goldenSectionSearch(Interval.first, Interval.first + ((Interval.second - Interval.first)*resphi), Interval.second, DELTA_Y_THRESHOLD);
		Position = v_Sum(Position, v_Multiply(Direction, Coefficient));

		Answer.push_back("");
		Answer.push_back("Alpha: " + std::to_string(Coefficient));
		Answer.push_back("S: " + Vector2String(Direction));
		Answer.push_back("X: " + Vector2String(Position));
		Answer.push_back("");

		//	threshold ("Step 3"?)
		if (DeltaVector(Position, LastPosition) < DELTA_X_THRESHOLD || std::abs(Value-LastValue) < DELTA_Y_THRESHOLD)
		{
			break;
		}
	}

	//	minimizer
	Answer.push_back("");
	Answer.push_back("minimizer:");
	Answer.push_back(Vector2String(Position));
	//	value
	Answer.push_back("value:");
	Answer.push_back(std::to_string(CalculateByCoordinate(Position)));
}

void UIHandler::Newton_method()
{
	Position = initPoint;

	std::vector<Term*> First_Order(varsCount);
	std::vector<std::vector<Term*>> Second_Order(varsCount);
	for (int i = 0; i < varsCount; i++)
		Second_Order[i] = std::vector<Term*>(varsCount);
	
	//	get 1st and 2nd partial derivertives
	for (int i = 0; i < varsCount; i++)
	{
		First_Order[i] = m_Calculator.PartialDiff(m_Calculator.myCurrentFunc(), Variables[i]);
		for (int j = i; j < varsCount; j++)
		{
			Second_Order[i][j] = m_Calculator.PartialDiff(First_Order[i], Variables[j]);
		}
	}

	//	vars
	Matrix Gradient(varsCount, 1);
	Matrix Hessian(varsCount, varsCount);
	//	thresholds
	std::vector<double> LastPosition;
	double DELTA_X_THRESHOLD = 0.00000001;
	double DELTA_Y_THRESHOLD = 0.00000001;
	int ITER_THRESHOLD = 1000;
	for (int iter = 1; iter < ITER_THRESHOLD; iter++)
	{
		LastPosition = Position;

		//	get Gradient vals
		//	get Hessian vals
		for (int i = 0; i < varsCount; i++)
		{
			Gradient.Data[i][0] = CalculateByCoordinate(Position, First_Order[i]);
			for (int j = i; j < varsCount; j++)
			{
				Hessian.Data[i][j] = Hessian.Data[j][i] = CalculateByCoordinate(Position, Second_Order[i][j]);
			}
		}
		//	print Hessian
		Answer.push_back("Hessian: ");
		CaCuMi::PrintMatrix(Hessian, Answer);
		//	calc Hessian inverse
		Hessian = CaCuMi::Inverse(Hessian);
		//	print Hessian inverse
		Answer.push_back("Hessian Inverse: ");
		CaCuMi::PrintMatrix(Hessian, Answer);
		
		//	calc delta X, update position
		std::vector<double> deltaX = CaCuMi::Matrix2Vector(CaCuMi::Multiply(Hessian, Gradient));

		double Coefficient;
		//	the calculation should know the: Direction, Position
		Direction = deltaX;
		Coefficient = goldenSectionSearch(-1.0, -resphi, 0.0, DELTA_Y_THRESHOLD);
		deltaX = v_Multiply(deltaX, Coefficient);
		Position = v_Sum(Position, deltaX);
		//	print position
		Answer.push_back("X: ");
		Answer.push_back(Vector2String(Position));
		Answer.push_back("");

		//	threshold
		if (Length(deltaX) < DELTA_X_THRESHOLD || DeltaVector(Position, LastPosition) < DELTA_X_THRESHOLD)
		{
			break;
		}
	}
	//	minimizer
	Answer.push_back("");
	Answer.push_back("minimizer:");
	Answer.push_back(Vector2String(Position));
	//	value
	Answer.push_back("value:");
	Answer.push_back(std::to_string(CalculateByCoordinate(Position)));
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
