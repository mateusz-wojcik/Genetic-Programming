//#include "stdafx.h"
#include "CNode.h"
#include <iostream>

CNode::CNode()
{
}

CNode::CNode(std::string val)
{
	value = val;
}

CNode::CNode(CNode& other)
{
	this->value = other.value;
	this->identity = other.identity;
	this->children.clear();
	for (int i = 0; i < other.getChildren().size(); i++)
	{
		this->children.push_back(new CNode(*(other.getChildren().at(i))));
	}
}

CNode::~CNode()
{
	for (int i = 0; i < getChildren().size(); i++)
	{
		delete getChildren().at(i);
	}
}

void CNode::operator = (CNode& other)
{
	this->value = other.value;
	this->identity = other.identity;
	this->children.clear();
	for (int i = 0; i < other.children.size(); i++)
	{
		children.push_back(new CNode(*(other.children.at(i))));
	}
}

std::ostream& operator << (std::ostream& output, CNode& node)
{
	output << "node value: " << node.getValue() << " ";
	for (int i = 0; i < node.getChildren().size(); i++)
	{
		output << node.getChildren().at(i)->getValue() << " ";
	}
	output << std::endl;
	return output;
}

void CNode::setIdentity(Identity id)
{
	identity = id;
}

void CNode::setValue(std::string newValue)
{
	this->value = newValue;
}

std::string CNode::fixIncorrectVariable(std::string str)
{
	std::string result;
	for (int i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]) || isdigit(str[i]))
		{
			result += str[i];
		}
	}
	if (result.length() == 0) result = "a";
	return result;
}

bool isOnlyDouble(const char* str)
{
	char* endptr = 0;
	strtod(str, &endptr);
	if (*endptr != '\0' || endptr == str) return false;
	return true;
}

bool CNode::isNumeric(const std::string& str)
{
	if (isOnlyDouble(str.c_str())) return true;
	std::string::const_iterator it = str.begin();
	while (it != str.end() && isdigit(*it)) ++it;
	return !str.empty() && it == str.end();
}

bool CNode::isOperator(const std::string& str)
{
	return str == "+" || str == "-" || str == "*" || str == "/";
}

bool CNode::isTrigonometric(const std::string& str)
{
	return str == "sin" || str == "cos";
}

bool CNode::isCorrectVariable(const std::string& str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && (isalpha(*it) || isdigit(*it))) ++it;
	return !str.empty() && it == str.end();
}

void CNode::createNode(std::vector<std::string>& expression, std::vector<std::string>::iterator& iterator, int& err)
{
	int childrenNumber;

	if (isNumeric(*iterator))
	{
		identity = NUMERIC;
		childrenNumber = 0;
	}
	else if (isOperator(*iterator))
	{
		identity = OPERATOR;
		childrenNumber = 2;
	}
	else if (isTrigonometric(*iterator))
	{
		identity = TRIGONOMETRIC;
		childrenNumber = 1;
	}
	else if (isCorrectVariable(*iterator))
	{
		identity = VARIABLE;
		childrenNumber = 0;
	}
	else
	{
		value = fixIncorrectVariable(*iterator);
		identity = VARIABLE;
		childrenNumber = 0;
	}
	for (int i = 0; i < childrenNumber; i++) //rekurencyjnie tworzy kolejne poddrzewa
	{
		if ((iterator != expression.end()) && next(iterator) == expression.end()) //iterator wskazuje na ostatni element
		{
			err = -1; //not enough operands
			children.push_back(new CNode(DEFAULT_VALUE));
			children.at(i)->setIdentity(NUMERIC);
		}
		else
		{
			iterator++;
			children.push_back(new CNode(*iterator));
			children.at(i)->createNode(expression, iterator, err);
		}
	}
}

//funkcja pomocnicza, ktora sprawdza, czy iterator wskazuje na ostatni element
std::vector<std::string>::iterator next(std::vector<std::string>::iterator iter)
{
	return ++iter;
}

std::string CNode::getValue()
{
	return value;
}

std::vector<CNode*>& CNode::getChildren()
{
	return children;
}

CNode::Identity CNode::getIdentity()
{
	return identity;
}
