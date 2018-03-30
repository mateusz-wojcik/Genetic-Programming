#include  "PGAlg.h"


std::string createRandomElement()
{
	std::string element;
	int option = rand() % 11 + 1;
	switch (option)
	{
	case 1:
		element = "+";
		break;
	case 2:
		element = "-";
		break;
	case 3:
		element = "/";
		break;
	case 4:
		element = "*";
		break;
	case 5:
		element = "sin";
		break;
	case 6:
		element = "cos";
		break;
	case 7:
		element = "x";
		break;
	case 8:
		element = "y";
		break;
	default:
		element = std::to_string(rand() % VALUE_RANGE);
		break;
	}

	return element;
}

void incrementIterator(int& iter, std::string argument)
{
	if (argument == "+" || argument == "-" || argument == "*" || argument == "/")
	{
		iter += 2;
	}
	else if (argument == "sin" || argument == "cos")
	{
		iter++;
	}
}

bool isTooLong(int length)
{
	if (length > 50) return true;
	return false;
}

std::string createRandomExpression()
{
	std::stringstream expression;
	std::string element;
	int iter = 1;
	do
	{
		element = createRandomElement();
		expression << element << " ";
		incrementIterator(iter, element);
		iter--;
		if (isTooLong(expression.str().length())) iter = 0;
	} while (iter > 0);

	return expression.str();
}


bool CPGAlg::bInitialize(CString  sTest)
{
	CT2CA convertedAnsiString(sTest);
	std::string path(convertedAnsiString);
	setPath(path);
	initiation();
	evaluation();
	return(true);
}//bool CPGAlg::bInitialize(CString  sTest)


void CPGAlg::vRunIteration()
{
	runIteration();
}//void CPGAlg::vRunIteration()


CString CPGAlg::sGetCurrentBestTree()
{
	std::string result(getBestTree(population).treeToString());
	CString s(result.c_str());
	return s;
}//CString  CPGAlg::sGetCurrentBestTree()

CTree CPGAlg::createRandomTree()
{
	CTree tree(createRandomExpression());
	return tree;
}

void CPGAlg::initiation()
{
	clearPopulation();
	clearParentsPopulation();
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		population.push_back(new CTree(createRandomExpression()));
	}
}

void split(const std::string& str, std::vector<std::string>& fileLineVector, char delimiter)
{
	std::stringstream ss(str);
	std::string token;
	fileLineVector.clear();
	while (std::getline(ss, token, delimiter))
	{
		fileLineVector.push_back(token);
	}
}

void CPGAlg::clearPopulation()
{
	for (int i = 0; i < population.size(); i++)
	{
		delete population.at(i);
	}
	population.clear();
}

void CPGAlg::clearParentsPopulation()
{
	for (int i = 0; i < parentsPopulation.size(); i++)
	{
		delete parentsPopulation.at(i);
	}
	parentsPopulation.clear();
}

void initializeVectors(std::vector<double>& values, std::vector<std::string>& variables)
{
	values.push_back(0);
	values.push_back(0);
	variables.push_back("x");
	variables.push_back("y");
}

void CPGAlg::evaluation()
{
	std::string line;
	std::vector<double> values;
	std::vector<std::string> variables;
	initializeVectors(values, variables);
	std::vector<std::pair<std::string, double>> variablesWithValues;
	std::vector<std::string> fileLineVector;
	double adaptationValue = 0;
	int error = 0;
	std::ifstream dataFile(path);
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		if (dataFile.is_open())
		{
			population.at(i)->setAdaptationValue(0);
			while (std::getline(dataFile, line))
			{
				split(line, fileLineVector, ';');
				values.at(0) = strtod(fileLineVector.at(0).c_str(), 0);
				values.at(1) = strtod(fileLineVector.at(1).c_str(), 0);
				population.at(i)->connectVariablesAndValues(variables, values, variablesWithValues);
				adaptationValue += pow(strtod(fileLineVector.at(2).c_str(), 0) - ((*population.at(i)).compute(population.at(i)->getRoot(), variablesWithValues, error)), 2);
			}
			population.at(i)->setAdaptationValue(adaptationValue);
			adaptationValue = 0;
		}
		dataFile.clear();
		dataFile.seekg(0, std::iostream::beg);
	}
	dataFile.close();
}

void CPGAlg::selection()
{
	for (int i = 0; i < TOURNAMENT_NUMBER; i++)
	{
		int tree1Index = rand() % population.size();
		int tree2Index = rand() % population.size();
		if (population.at(tree1Index)->getAdaptationValue() >= population.at(tree2Index)->getAdaptationValue())
		{
			parentsPopulation.push_back(new CTree(*population.at(tree1Index)));
		}
		else
		{
			parentsPopulation.push_back(new CTree(*population.at(tree2Index)));
		}
	}
}

void CPGAlg::hybridize()
{
	for (int i = 0; i < parentsPopulation.size() / 2; i++)
	{
		if (rand() % 100 + 1 < HYBRIDIZE_PROBABILITY)
		{
			parentsPopulation.at(i * 2)->hybridize(*parentsPopulation.at(std::abs(i * 2 - 1)));
			if ((parentsPopulation.at(i * 2)->treeToString()).length() > 100)
			{
				delete parentsPopulation.at(i * 2);
				parentsPopulation.at(i * 2) = new CTree(createRandomExpression());
			}
			if ((parentsPopulation.at(std::abs(i * 2 - 1))->treeToString()).length() > 100)
			{
				delete parentsPopulation.at(std::abs(i * 2 - 1));
				parentsPopulation.at(std::abs(i * 2 - 1)) = new CTree(createRandomExpression());
			}
		}
		population.push_back(new CTree(*parentsPopulation.at(i * 2)));
		population.push_back(new CTree(*parentsPopulation.at(std::abs(i * 2 - 1))));
	}
}

void CPGAlg::mutation()
{
	for (int i = 0; i < population.size(); i++)
	{
		if (rand() % 100 + 1 < MUTATION_PROBABILITY)
		{
			population.at(i)->mutate();
		}
	}
}

CTree CPGAlg::getBestTree(std::vector<CTree*>& population)
{
	int minTreeIndex = 0;
	for (int i = 0; i < population.size(); i++)
	{
		if (population.at(i)->getAdaptationValue() < population.at(minTreeIndex)->getAdaptationValue())
		{
			minTreeIndex = i;
		}
	}
	return *population.at(minTreeIndex);
}

void CPGAlg::printAll()
{
	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		population.at(i)->preorder();
	}
}

void CPGAlg::runIteration()
{
	initiation();
	evaluation();
	for (int i = 0; i < ITEARTIONS_NUMBER; i++)
	{
		selection();
		hybridize();
		mutation();
		evaluation();
	}
}

void CPGAlg::setPath(std::string path)
{
	this->path = path;
}

std::string CPGAlg::getPath()
{
	return path;
}


