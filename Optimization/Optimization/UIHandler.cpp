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
		SteepestDescent();
		break;
	case QuasiNewton:
		Answer.push_back("[QuasiNewton] : \t" + equations[equationIndex]);
		QuasiNewton_method();
		break;
	case ConjugateGradient:
		Answer.push_back("[ConjugateGradient] : \t" + equations[equationIndex]);
		ConjugateGradient_method();
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
#pragma region GoldenSection
///	boost
double phi = (1 + std::sqrt(5)) / 2;	// 1.6xx
double resphi = 2 - phi;	// 0.3xx
double fof_x, fof_b, x;
bool Bias_Left;
int GOLDEN_ITER_THRESHOLD = 1000;
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

	///std::cout << b << "\t" << x << "\n";

	//Answer.push_back(" x: " + std::to_string(b) + " , b: " + std::to_string(x) + " , ca: " + std::to_string(std::abs(a - c)));

	//	calculate (get y)
	fof_x = CalculateByCoef(x);
	fof_b = CalculateByCoef(b);

	//	threshold - delta( f(b1), f(b2) )
	if ((std::abs(fof_x - fof_b) < tau) || (std::abs(x - b) < tau))
		return (x + b) / 2;

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
	Term* test_term = m_Calculator.PartialDiff(m_Calculator.myCurrentFunc(), 'x');
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

//	Powell��s "quadratically" convergent method
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

	double DELTA_X_THRESHOLD = 0.0000001;
	double DELTA_Y_THRESHOLD = 0.0000001;
	int ITER_THRESHOLD = 100;
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
			Answer.push_back("Alpha: " + std::to_string(Coefficient));
			//
			Position = v_Sum(Position, v_Multiply(Direction, Coefficient));
		}
		Answer.push_back("i = " + std::to_string(varsCount));
		Answer.push_back("X: " + Vector2String(Position));

		Value = CalculateByCoordinate(Position);

		//	quadra scheme~
		for (int i = 0; i < (varsCount - 1); i++)
		{
			Directions[i] = Directions[i + 1];
		}
		Direction = v_Subtract(Position, LastPosition);
		Directions[varsCount - 1] = Direction;

		//	Golden again
		double Coefficient;
		std::pair<double, double> Interval;
		Interval = FindInterval(Direction, Position, intervals);
		Coefficient = goldenSectionSearch(Interval.first, Interval.first + ((Interval.second - Interval.first)*resphi), Interval.second, DELTA_Y_THRESHOLD);
		Position = v_Sum(Position, v_Multiply(Direction, Coefficient));

		Answer.push_back("");
		Answer.push_back("Alpha: " + std::to_string(Coefficient));
		Answer.push_back("S: " + Vector2String(Direction));
		Answer.push_back("X: " + Vector2String(Position));
		Answer.push_back("");

		//	threshold ("Step 3"?)
		if (DeltaVector(Position, LastPosition) < DELTA_X_THRESHOLD || std::abs(Value - LastValue) < DELTA_Y_THRESHOLD)
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
	double DELTA_X_THRESHOLD = 0.00001;
	double DELTA_Y_THRESHOLD = 0.00000001;
	int ITER_THRESHOLD = 100;
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

// Will's Func
void UIHandler::SteepestDescent()
{
	// 第 i 個迭代
	int index = 0;
	std::map<char, double> inputX;
	std::vector<double> outputX = initPoint;
	std::vector<double> h;
	double lambda;

	// 整理 X
	for (int i = 0; i < Variables.size(); i++)
	{
		inputX[Variables[i]] = outputX[i];
	}

	// 整理Func 微分
	Term* myFunc = m_Calculator.myCurrentFunc();
	std::vector<Term*> DiffedFunc = m_Calculator.Gradient(myFunc, Variables);
	std::vector<std::vector<Term*>> A(DiffedFunc.size());

	// 產生 A (Func)
	for (int i = 0; i < A.size(); i++)
	{
		A[i] = m_Calculator.Gradient(DiffedFunc[i], Variables);
	}

	// 檢查何時停止
	bool stop = true;
	h = m_Calculator.Caculate(DiffedFunc, inputX);
	for (int i = 0; i < h.size(); i++)
	{
		h[i] = -h[i];
		if (abs(h[i]) >= 0.0000001f)
			stop = false;
	}
	while (!stop && index < 50)
	{
		// 顯示 i :
		Answer.push_back("");
		Answer.push_back("i = " + std::to_string(index));

		// 1.取得 h
		Answer.push_back("h = " + Vector2String(h));

		// 2.取得 lambda

		// 將 h 轉成 matrix
		Matrix h_mat(h.size(), 1);
		Matrix h_matT(1, h.size());
		for (int k = 0; k < h.size(); k++)
		{
			h_mat.Data[k][0] = h[k];
			h_matT.Data[0][k] = h[k];
		}
		Matrix temp = CaCuMi::Multiply(h_matT, h_mat);
		// lambda = hT * h
		lambda = temp.Data[0][0];
		// 取得 A (值)
		Matrix A_mat(A.size(), A[0].size());
		for (int i = 0; i < A.size(); i++)
		{
			A_mat.Data[i] = m_Calculator.Caculate(A[i], inputX);
		}
		temp = CaCuMi::Multiply(h_matT, A_mat);
		temp = CaCuMi::Multiply(temp, h_mat);
		// lambda = hT * h / hT * A * h
		lambda = lambda / temp.Data[0][0];

		double Coefficient;
		//	the calculation should know the: Direction, Position
		Direction = v_Multiply(h, lambda);
		Position = outputX;
		Coefficient = goldenSectionSearch(0.0, resphi, 1.0, 0.0000001);
		lambda *= Coefficient;
		Answer.push_back("lambda = " + std::to_string(lambda));

		// 3.取得 X
		// outputX = outputX + lambda * h
		bool bounded = false;
		for (int i = 0; i < Variables.size(); i++)
		{
			outputX[i] = outputX[i] + lambda * h[i];
			if (intervals[i].first > outputX[i])
				outputX[i] = intervals[i].first;
			else if (intervals[i].second < outputX[i])
				outputX[i] = intervals[i].second;
		}

		// 整理 X
		for (int i = 0; i < Variables.size(); i++)
		{
			inputX[Variables[i]] = outputX[i];
		}
		// InputX->outputX
		Answer.push_back("X = " + Vector2String(outputX));

		// 檢查是否停止
		stop = true;
		h = m_Calculator.Caculate(DiffedFunc, inputX);
		for (int i = 0; i < h.size(); i++)
		{
			h[i] = -h[i];
			if (abs(h[i]) >= 0.0000001f)
				stop = false;
		}
		index++;
	}

	// 結束
	// 輸出 X 和 min
	// "[x, y]= "
	Answer.push_back("");
	Answer.push_back("ANSWER:");
	std::string finalOutput = "";
	for (int i = 0; i < Variables.size(); i++)
	{

		if (Variables.size() == 1)
		{
			Answer.push_back("[" + std::string(1, Variables[i]) + "]= ");
		}
		else
		{
			if (i != Variables.size() - 1 && i != 0)
				finalOutput += std::string(1, Variables[i]) + ", ";
			else if (i == 0)
				finalOutput += "[" + std::string(1, Variables[i]) + ", ";
			else
				finalOutput += std::string(1, Variables[i]) + "]= ";
		}
	}
	finalOutput += Vector2String(outputX);
	Answer.push_back(finalOutput);
	double min = m_Calculator.Caculate(inputX);
	Answer.push_back("min = " + std::to_string(min));
}

void UIHandler::QuasiNewton_method()
{
	//	(DFP applied) (I used the true Hessian for init mimic instead of matrix_i)

	Position = initPoint;

	std::vector<Term*> First_Order(varsCount);

	///
	std::vector<std::vector<Term*>> Second_Order(varsCount);
	for (int i = 0; i < varsCount; i++)
		Second_Order[i] = std::vector<Term*>(varsCount);
	Matrix Hessian(varsCount, varsCount);
	//	get 1st and 2nd partial derivertives
	for (int i = 0; i < varsCount; i++)
	{
		First_Order[i] = m_Calculator.PartialDiff(m_Calculator.myCurrentFunc(), Variables[i]);
		for (int j = i; j < varsCount; j++)
		{
			Second_Order[i][j] = m_Calculator.PartialDiff(First_Order[i], Variables[j]);
			Hessian.Data[i][j] = Hessian.Data[j][i] = CalculateByCoordinate(Position, Second_Order[i][j]);
		}
	}

	Answer.push_back("init Hessian: ");
	CaCuMi::PrintMatrix(Hessian, Answer);
	Hessian = CaCuMi::Inverse(Hessian);
	Answer.push_back("init Hessian_inverse: ");
	CaCuMi::PrintMatrix(Hessian, Answer);

	////std::cout << "(Hessian):\n";
	////CaCuMi::PrintMatrix(Hessian);
	////Hessian = CaCuMi::Inverse(Hessian);
	////std::cout << "(Hessian_inv):\n";
	////CaCuMi::PrintMatrix(Hessian);
	///

	//////	get 1st order's partial derivertives
	////for (int i = 0; i < varsCount; i++)
	////{
	////	First_Order[i] = m_Calculator.PartialDiff(m_Calculator.myCurrentFunc(), Variables[i]);
	////}

	//	vars
	Matrix Gradient(varsCount, 1);
	Matrix LastGradient(varsCount, 1);
	Matrix Mimic_Hessian(varsCount, varsCount);
	Matrix Delta_Gradient(varsCount, 1);
	//	init mimic (with matrix I)
	Mimic_Hessian = Hessian;
	//////for (int i = 0; i < varsCount; i++)
	//////{
	//////	for (int j = 0; j < varsCount; j++)
	//////	{
	//////		if(i == j)
	//////			Mimic_Hessian.Data[i][j] = 1.0;
	//////		else
	//////			Mimic_Hessian.Data[i][j] = 0;
	//////	}
	//////}

	//	init Gradient vals
	for (int i = 0; i < varsCount; i++)
	{
		Gradient.Data[i][0] = CalculateByCoordinate(Position, First_Order[i]);
	}
	//	print init mimic
	//////Answer.push_back("init Hessian: ");
	//////CaCuMi::PrintMatrix(Mimic_Hessian, Answer);

	//	thresholds
	std::vector<double> LastPosition;
	double DELTA_X_THRESHOLD = 0.01;
	double DELTA_Y_THRESHOLD = 0.01;
	int ITER_THRESHOLD = 100;
	//	the optimization loop
	for (int iter = 1; ; iter++)
	{
		LastPosition = Position;

		//	output
		//	print Hessian
		Answer.push_back("Quasi_Hessian: ");
		CaCuMi::PrintMatrix(Mimic_Hessian, Answer);
		//	print position
		Answer.push_back("X: ");
		Answer.push_back(Vector2String(Position));
		Answer.push_back("");

		//	threshold
		double length = Length(CaCuMi::Matrix2Vector(Gradient));
		std::cout << "\ni: " << iter << "\tm: " << length;
		if (length*length < DELTA_X_THRESHOLD)	//Length(deltaX) < DELTA_X_THRESHOLD || 
		{
			break;
		}
		if (iter > ITER_THRESHOLD)
		{
			Answer.push_back("");
			Answer.push_back("I GIVE UP!");
			Answer.push_back("");
			break;
		}

		///
		////std::cout << "\nMimic_Hessian:\n";
		////CaCuMi::PrintMatrix(Mimic_Hessian);
		////std::cout << "Gradient:\n";
		////CaCuMi::PrintMatrix(Gradient);
		///

		//	calc delta X, update position
		std::vector<double> deltaX = CaCuMi::Matrix2Vector(CaCuMi::Multiply(Mimic_Hessian, Gradient));

		//	argmin f(x + ad)
		double Coefficient;
		//	the calculation should know the: Direction, Position
		Direction = deltaX;
		Coefficient = goldenSectionSearch(-1.0, -resphi, 0.0, 0.00000001);
		//	apply delta x to position
		///
		std::cout << "coef: " << Coefficient << "\n";
		deltaX = v_Multiply(deltaX, Coefficient);
		///
		Position = v_Sum(Position, deltaX);

		///
		////std::cout << "Position:\t" << Vector2String(Position) << "\n";
		///

		//	calc new gradient with new position
		LastGradient = Gradient;
		for (int i = 0; i < varsCount; i++)
		{
			Gradient.Data[i][0] = CalculateByCoordinate(Position, First_Order[i]);
		}
		//	calc delta_Gradient
		Delta_Gradient = CaCuMi::Sub(Gradient, LastGradient);

		//	calc new Mimic
		Matrix Matrix_dX = CaCuMi::Vector2Matrix(deltaX);
		/// n*n:	CaCuMi::Multiply(Matrix_dX, CaCuMi::Transpose(Matrix_dX));
		///	1*1:	CaCuMi::Multiply(CaCuMi::Transpose(Matrix_dX), Delta_Gradient);
		///	n*n:
		//CaCuMi::Multiply(CaCuMi::Multiply(Matrix_dX, CaCuMi::Transpose(Matrix_dX)), CaCuMi::Multiply(CaCuMi::Transpose(Matrix_dX), Delta_Gradient).Data[0][0]);
		/// n*1:	CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient);
		///	n*n:	CaCuMi::Multiply(CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient), CaCuMi::Transpose(CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient)));
		//CaCuMi::Multiply(CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient), CaCuMi::Transpose(CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient)));
		//	1*1:	
		//CaCuMi::Multiply(CaCuMi::Transpose(Matrix_dX), CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient));


		Mimic_Hessian = CaCuMi::Sum(
			Mimic_Hessian,
			CaCuMi::Sub(
				CaCuMi::Multiply(
					CaCuMi::Multiply(
						Matrix_dX,
						CaCuMi::Transpose(Matrix_dX)
					),
					(1.0 / CaCuMi::Multiply(CaCuMi::Transpose(Matrix_dX), Delta_Gradient).Data[0][0])
				),
				CaCuMi::Multiply(
					CaCuMi::Multiply(
						CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient),
						CaCuMi::Transpose(CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient))
					),
					(1.0 / CaCuMi::Multiply(CaCuMi::Transpose(Matrix_dX), CaCuMi::Multiply(Mimic_Hessian, Delta_Gradient)).Data[0][0])
				)
			)
		);
	}
	//	minimizer
	Answer.push_back("");
	Answer.push_back("minimizer:");
	Answer.push_back(Vector2String(Position));
	//	value
	Answer.push_back("value:");
	Answer.push_back(std::to_string(CalculateByCoordinate(Position)));
}

void UIHandler::ConjugateGradient_method()
{
	Position = initPoint;

	//	1st partial derivatives
	std::vector<Term*> First_Order(varsCount);
	for (int i = 0; i < varsCount; i++)
		First_Order[i] = m_Calculator.PartialDiff(m_Calculator.myCurrentFunc(), Variables[i]);

	//	vars
	Matrix Gradient(varsCount, 1);		//	(n*1)
	Matrix LastGradient(varsCount, 1);	//	(n*1)
	double Value;
	double LastValue;
	//	calc value
	Value = CalculateByCoordinate(Position);

	//	thresholds
	std::vector<double> LastPosition;
	double ERROR_1 = 0.001;
	double ERROR_2 = 0.001;
	double ERROR_3 = 0.001;
	double DELTA_Y_THRESHOLD = 0.000001;
	int ITER_THRESHOLD = 100;
	for (int iter = 1; ; iter++)
	{
		//	print i
		Answer.push_back("i = " + std::to_string(iter));

		//	update lasts
		LastPosition = Position;
		LastGradient = Gradient;

		//	get new Gradient
		for (int i = 0; i < varsCount; i++)
			Gradient.Data[i][0] = CalculateByCoordinate(Position, First_Order[i]);
		if (iter != 1)
		{
			//	calc beta
			double Beta =
				CaCuMi::Multiply(CaCuMi::Transpose(Gradient), Gradient).Data[0][0] /
				CaCuMi::Multiply(CaCuMi::Transpose(LastGradient), LastGradient).Data[0][0];
			//	fix Gradient
			Gradient = CaCuMi::Sum(Gradient, CaCuMi::Multiply(LastGradient, Beta));
			//	print Beta
			Answer.push_back("beta = " + std::to_string(Beta));
		}

		//	print Si(Gradient)
		Answer.push_back("Si:");
		CaCuMi::PrintMatrix(Gradient, Answer);

		//	threshold_3
		if (CaCuMi::Multiply(CaCuMi::Transpose(Gradient), Gradient).Data[0][0] < ERROR_3)
			break;

		//	calc delta X, update position
		std::vector<double> deltaX = CaCuMi::Matrix2Vector(Gradient);
		Direction = CaCuMi::Matrix2Vector(Gradient);

		//	find Alpha
		double Alpha;
		Direction = deltaX;
		std::pair<double, double> Interval;
		Interval = FindInterval(Direction, Position, intervals);
		Alpha = goldenSectionSearch(Interval.first, Interval.first + ((Interval.second - Interval.first)*resphi), Interval.second, 0.00000001);
		deltaX = v_Multiply(deltaX, Alpha);
		Position = v_Sum(Position, deltaX);
		//	print Alpha
		Answer.push_back("alpha = " + std::to_string(Alpha));

		//	calc Value
		Value = CalculateByCoordinate(Position);

		//	print position
		Answer.push_back("Xi: ");
		Answer.push_back(Vector2String(Position));
		Answer.push_back("");

		//	threshold
		if (abs(Value - LastValue) < ERROR_1)
			break;
		if (Length(deltaX)*Length(deltaX) < ERROR_2)
			break;
		if (iter > ITER_THRESHOLD)
		{
			Answer.push_back("");
			Answer.push_back("I GIVE UP!");
			Answer.push_back("");
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
