#pragma once
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <atlstr.h>
#include "CTree.h"
#include <fstream>
#include <cstdlib>
#define VALUE_RANGE 10
#define POPULATION_SIZE 1000
#define ITEARTIONS_NUMBER 1
#define TOURNAMENT_NUMBER 500
#define HYBRIDIZE_PROBABILITY 30 //%
#define MUTATION_PROBABILITY 30 //%

using namespace std;


class  CPGAlg
{
public:
	bool bInitialize(CString  sTest);
	void vRunIteration();
	CString  sGetCurrentBestTree();

	CTree createRandomTree();
	void clearPopulation();
	void clearParentsPopulation();
	CTree getBestTree(std::vector<CTree*>&);
	void printAll();
	void initiation();
	void evaluation();
	void selection();
	void hybridize();
	void mutation();
	void runIteration();
	std::string getPath();
	void setPath(std::string);

private:
	std::vector<CTree*> population;
	std::vector<CTree*> parentsPopulation;

	std::string path;
};//class  CPGAlg