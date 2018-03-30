//#include "stdafx.h"
#include "CTree.h"

CTree::CTree()
{
}

CTree::CTree(std::string input)
{
	adaptationValue = INT_MAX;
	parse(input);
	createTree();
}

CTree::CTree(CTree& other)
{
	this->adaptationValue = other.adaptationValue;
	this->expression = other.expression;
	this->root = new CNode(*other.getRoot());
}

CTree::~CTree()
{
	delete this->root;
}

CTree CTree:: operator + (CTree& other)
{
	CTree newTree(*this);
	CNode& leaf = newTree.findLeaf();
	leaf = *(other.getRoot());
	return newTree;
}

void CTree:: operator = (CTree& other)
{
	delete this->root;
	this->adaptationValue = other.adaptationValue;
	this->expression = other.expression;
	this->root = new CNode(*other.getRoot());
}

//mutate options
void CTree::changeLeafToNew()
{
	int range = 10;
	CNode& leafToFound = this->findLeaf();
	leafToFound = *createRandomLeaf(range);
}

void CTree::changeLeafToNode()
{
	CNode& leafToFound = this->findLeaf();
	leafToFound = *createRandomNode();
}

void CTree::changeNodeToLeaf()
{
	int range = 10;
	CNode& foundNode = findRandomNode(*this->root);
	foundNode = *createRandomLeaf(range);
}

void CTree::mutate()
{
	int option = rand() % 3 + 1;
	switch (option)
	{
	case 1:
		changeLeafToNew();
		break;
	case 2:
		changeLeafToNode();
		break;
	case 3:
		changeNodeToLeaf();
		break;
	}
}

void CTree::hybridize(CTree& other)
{
	CTree tmpThis(*this);
	CTree tmpOther(other);
	CNode& foundNodeInThis = findRandomNode(*tmpThis.getRoot());
	CNode& foundNodeInOther = findRandomNode(*tmpOther.getRoot());
	CNode& tmp = foundNodeInOther;
	foundNodeInOther = foundNodeInThis;
	foundNodeInThis = tmp;
	*this = tmpThis;
	other = tmpOther;
}

void CTree::clearExpression()
{
	expression.clear();
}

//parsuje stringa i tworzy na jego podstawie vector stringow (wyra¿eñ do drzewa)
void CTree::parse(std::string input)
{
	clearExpression();
	std::istringstream iss(input);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(expression));
}

//tworzy wektor wartosci na podstawie sczytanej linii
void CTree::parseToValues(std::string input, std::vector<double>& values)
{
	std::istringstream iss(input);
	double value;
	while (iss >> value)
	{
		values.push_back(value);
	}
}

//ustawia wartosc korzenia i rozpoczyna tworzenie drzewa
void CTree::createTree()
{
	int err = 0;
	std::vector<std::string>::iterator iterator = expression.begin();
	root = new CNode(*iterator);
	root->createNode(expression, iterator, err);
}

CNode& findLeafHelp(CNode& node)
{
	if (node.getIdentity() == node.NUMERIC || node.getIdentity() == node.VARIABLE)
	{
		return node;
	}
	else
	{
		return findLeafHelp(*(node.getChildren().at(0)));
	}
}

//znajduje pierwszy lisc przechodzac preorderem
CNode& CTree::findLeaf()
{
	return findLeafHelp(*root);
}

CNode& CTree::findRandomNode(CNode& node)
{
	int randomNodeNumber = rand() % countElements(node);
	if (randomNodeNumber == 0)
	{
		return node;
	}
	else
	{
		return findRandomNode(*node.getChildren().at(rand() % node.getChildren().size()));
	}
}

//funkcja pomocnicza dla preorder
void preorderHelp(CNode* node)
{
	std::cout << node->getValue() << " ";
	for (int i = 0; i < node->getChildren().size(); i++)
	{
		preorderHelp(node->getChildren().at(i));
	}
}

//przechodzi drzewo preorderem
void CTree::preorder()
{
	preorderHelp(root);
}

//wypisuje wszystkie zmienne w drzewie
void CTree::printVariables(CNode* node, std::vector<std::string>& printed)
{
	if (node->getIdentity() == node->VARIABLE)
	{
		if (std::find(printed.begin(), printed.end(), node->getValue()) == printed.end())
		{
			printed.push_back(node->getValue());
			std::cout << node->getValue() << " ";
		}
	}
	for (int i = 0; i < node->getChildren().size(); i++)
	{
		printVariables(node->getChildren().at(i), printed);
	}
}

//tworzy vector ze zmiennymi
void CTree::createVectorWithVariables(CNode* node, std::vector<std::string>& variables)
{
	if (node->getIdentity() == node->VARIABLE)
	{
		if (std::find(variables.begin(), variables.end(), node->getValue()) == variables.end())
		{
			variables.push_back(node->getValue());
		}
	}
	for (int i = 0; i < node->getChildren().size(); i++)
	{
		createVectorWithVariables(node->getChildren().at(i), variables);
	}
}

//tworzy vector, w ktorym sa wartosci wezlow danego drzewa (kopiuje je do expression)
void CTree::createExpression(CNode* node)
{
	expression.push_back(node->getValue());
	for (int i = 0; i < node->getChildren().size(); i++)
	{
		createExpression(node->getChildren().at(i));
	}
}

//tworzy wektor par
void CTree::connectVariablesAndValues(std::vector<std::string>& variables, std::vector<double>& values, std::vector<std::pair<std::string, double>>& pairs)
{
	pairs.clear(); // new
	for (int i = 0; i < variables.size(); i++)
	{
		pairs.push_back(std::make_pair(variables.at(i), values.at(i)));
	}
}

//oblicza wyrazenie   wezel, vector(zmienna,wartosc zmiennej)
double CTree::compute(CNode* node, std::vector<std::pair<std::string, double>>& variables, int& error)
{
	if (node->getIdentity() == node->VARIABLE)
	{
		for (std::vector<std::pair<std::string, double>>::const_iterator i = variables.begin(); i != variables.end(); ++i)
		{
			if (std::get<0>(*i) == node->getValue())
			{
				return std::get<1>(*i);
			}
		}
	}
	else if (node->getIdentity() == node->NUMERIC)
	{
		double result = strtod((node->getValue()).c_str(), 0);
		return result;
	}
	else if (node->getIdentity() == node->OPERATOR)
	{
		std::string op = node->getValue();
		if (op == "+") return compute(node->getChildren().at(0), variables, error) + compute(node->getChildren().at(1), variables, error);
		if (op == "-") return compute(node->getChildren().at(0), variables, error) - compute(node->getChildren().at(1), variables, error);
		if (op == "*") return compute(node->getChildren().at(0), variables, error) * compute(node->getChildren().at(1), variables, error);
		if (op == "/")
		{
			if (node->getChildren().at(1)->getValue() == "0")
			{
				error = -1;
				return 1;
			}
			else
			{
				if (compute(node->getChildren().at(1), variables, error) == 0)
				{
					error = -1;
					return 1;
				}
				else
				{
					return compute(node->getChildren().at(0), variables, error) / compute(node->getChildren().at(1), variables, error);
				}
			}
		}
	}
	else if (node->getIdentity() == node->TRIGONOMETRIC)
	{
		std::string fun = node->getValue();
		double argument = compute(node->getChildren().at(0), variables, error);
		if (fun == "sin") return sin(argument);
		if (fun == "cos") return cos(argument);
	}
}

//zwraca korzen drzewa
CNode* CTree::getRoot()
{
	return root;
}

double CTree::getAdaptationValue()
{
	return adaptationValue;
}

void CTree::setAdaptationValue(double newValue)
{
	this->adaptationValue = newValue;
}

//zwraca przez referencje vector wyra¿eñ
std::vector<std::string>& CTree::getExpression()
{
	return expression;
}

void CTree::printInformation(int err)
{
	if (err == -1) std::cout << "Not enough operands - default tree fixing (value: " << DEFAULT_VALUE << ")" << std::endl;
}

std::string CTree::expressionToString()
{
	std::ostringstream oss;
	if (!expression.empty())
	{
		std::copy(expression.begin(), expression.end() - 1, std::ostream_iterator<std::string>(oss, " "));
		oss << expression.back();
	}
	return oss.str();
}

//funkcja pomocnicza dla preorder
void treeToStringHelp(CNode* node, std::ostringstream& result)
{
	result << node->getValue() << " ";
	for (int i = 0; i < node->getChildren().size(); i++)
	{
		treeToStringHelp(node->getChildren().at(i), result);
	}
}

std::string CTree::treeToString()
{
	std::ostringstream result;
	treeToStringHelp(this->root, result);
	return result.str();

}

int countNodesInLevel(int start, int level, CNode& node)
{
	if (level == start)
	{
		return 1;
	}
	else if (node.getChildren().size() > 0)
	{
		int result = 0;
		start++;
		for (int i = 0; i < node.getChildren().size(); i++)
		{
			result += countNodesInLevel(start, level, *node.getChildren().at(i));
		}
		return result;
	}
	else return 0;
}

int CTree::nodesInLevel(int level)
{
	return countNodesInLevel(1, level, *this->root);
}

int CTree::countElements(CNode& node)
{
	int current = 1;
	for (int i = 0; i < node.getChildren().size(); i++)
	{
		current += countElements(*node.getChildren().at(i));
	}
	return current;
}

CNode* CTree::createRandomNode()
{
	CNode* randomNode = new CNode();
	int childrenNumber;
	int valueOption = rand() % 6 + 1;
	switch (valueOption)
	{
	case 1:
		(*randomNode).setValue("+");
		(*randomNode).setIdentity((*randomNode).OPERATOR);
		break;
	case 2:
		(*randomNode).setValue("-");
		(*randomNode).setIdentity((*randomNode).OPERATOR);
		break;
	case 3:
		(*randomNode).setValue("*");
		(*randomNode).setIdentity((*randomNode).OPERATOR);
		break;
	case 4:
		(*randomNode).setValue("/");
		(*randomNode).setIdentity((*randomNode).OPERATOR);
		break;
	case 5:
		(*randomNode).setValue("sin");
		(*randomNode).setIdentity((*randomNode).TRIGONOMETRIC);
		break;
	case 6:
		(*randomNode).setValue("cos");
		(*randomNode).setIdentity((*randomNode).TRIGONOMETRIC);
		break;
	}
	if ((*randomNode).getIdentity() == (*randomNode).TRIGONOMETRIC)
	{
		childrenNumber = 1;
	}
	else if ((*randomNode).getIdentity() == (*randomNode).OPERATOR)
	{
		childrenNumber = 2;
	}
	for (int i = 0; i < childrenNumber; i++)
	{
		int option = rand() % 2 + 1;
		if (option == 1)
		{
			(*randomNode).getChildren().push_back(new CNode(*createRandomLeaf(10))); // bardzo nieoptymalnie
		}
		else if (option == 2)
		{
			(*randomNode).getChildren().push_back(new CNode(*createRandomNode()));
		}
	}
	return randomNode;
}

CNode* CTree::createRandomLeaf(int range)
{
	CNode* randomLeaf = new CNode();
	int numOrVar = rand() % 2 + 1;
	if (numOrVar == 1)
	{
		int newLeafValue = rand() % range + 1;
		(*randomLeaf).setIdentity((*randomLeaf).NUMERIC);
		(*randomLeaf).setValue(std::to_string(newLeafValue));
	}
	else
	{
		int newVarValue = rand() % 2 + 1;
		(*randomLeaf).setIdentity((*randomLeaf).VARIABLE);
		if (newVarValue == 1) (*randomLeaf).setValue("x");
		else (*randomLeaf).setValue("y");
	}
	return randomLeaf;
}

