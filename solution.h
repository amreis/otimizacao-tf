#ifndef SOLUTION_H
#define SOLUTION_H

#include "instance.h"

#include <vector>
#include <iostream>
#include "helpers.h"
using namespace std;

class Solution
{
public:
	vector<bool> openFacility;
	vector< vector <bool> > served;
	int cost;
	Instance &inst;
	
	void getNeighbour ();
	Solution(vector < pair <int, int> > initialSolution, Instance &inst);
};
#endif
