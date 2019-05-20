#include "UIHandler.h"

std::vector<std::string> UIHandler::DoMath()
{
	std::vector<std::string> Answer;
	Answer.clear();

	switch (m_Method)
	{
	case Powell:

		break;
	case Newton:

		break;
	case SteepDescent:

		break;
	case QuasiNewton:

		break;
	case ConjugateGradient:

		break;
	default:
		std::cout << "[Method Error]";
		break;
	}

	return std::vector<std::string>();
}
