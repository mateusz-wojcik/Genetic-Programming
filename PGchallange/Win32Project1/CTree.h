#pragma once
#include "CNode.h"
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#define DEFAULT_FIXING_VALUE 1

class CTree
{
public:
	CTree();
	CTree(std::string);
	CTree(CTree&);
	~CTree();

	CTree operator + (CTree&);
	void CTree:: operator = (CTree&);

	//mutation
	void mutate();
	void changeLeafToNew();
	void changeLeafToNode();
	void changeNodeToLeaf();
	void hybridize(CTree&);

	//mutation - help function
	int countElements(CNode&); //liczy elementy w drzewie zaczynaj¹c od danego wezla
	CNode* createRandomNode();
	CNode* createRandomLeaf(int);

	void clearExpression();
	std::string expressionToString();
	std::string treeToString();
	CNode& findLeaf(); //zwraca pierwszy znaleziony lisc
	CNode& findRandomNode(CNode&); //zwraca losowo wybrany wezel
	void parse(std::string); //tworzy vector stringow (wyrazen) - expression
	void parseToValues(std::string, std::vector<double>&); //tworzy vector wartosci int na podstawie sczytanej linii
	void createTree(); //tworzy korzen i rozpoczyna tworzenie drzewa
	void preorder(); //przechodzi drzewo preorderem i wypisuje wyrazenie w postaci prefiksowej
	void printInformation(int);
	void printVariables(CNode*, std::vector<std::string>&); //wypisuje wszystkie zmienne
	void createVectorWithVariables(CNode* node, std::vector<std::string>&); //tworzy wektor ze zmiennymi w kolejnosci prefiksowej (tak jak w vars)
	void CTree::createExpression(CNode* node); //tworzy expression na podstawie drzewa (odwrotnie)
	void connectVariablesAndValues(std::vector<std::string>& variables, std::vector<double>& values, std::vector<std::pair<std::string, double>>& pairs); //tworzy wektor par (laczy zmienne z ich wartosciami)
	double compute(CNode*, std::vector<std::pair<std::string, double>>&, int&); //obliczanie wartosci
	CNode* getRoot(); //zwraca weze³, ktory jest korzeniem
	double getAdaptationValue();
	void setAdaptationValue(double);
	std::vector<std::string>& getExpression(); //zwraca vector stringow (wyrazen)
	int nodesInLevel(int);

private:
	CNode * root; //korzen
	std::vector<std::string> expression; //vector wyrazen

	double adaptationValue;
};


