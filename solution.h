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
	Instance inst;
	
    
    Solution(const Solution& s);
	Solution getNeighbour ();
    Solution getNeighbour2();
    Solution getNeighbour3();
	Solution(vector < pair <int, int> > initialSolution, Instance &inst);
    
    Instance getInst() const
    {
        return Instance(this->inst);
    }
};
#endif
