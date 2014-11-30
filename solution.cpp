#include "solution.h"


void Solution::getNeighbour ()
{
	srand (time(NULL));
	int	randomCustomer = rand() % inst.nClients + 1;
	
	
	vector < int > pathsThatReachCustomer;
	for (size_t i=1; i < inst.costMatrix.size(); i++)
	{
		if (inst.costMatrix[i][randomCustomer] >= 0)
			pathsThatReachCustomer.push_back(i);		
	}

	
	for (size_t i = 0; i < served[randomCustomer].size();i++)
	{	
		if(served[randomCustomer][i]==1)
			cout << inst.pathes[i];
	}
	
}

Solution::Solution(vector < pair <int, int> > initialSolution, Instance &inst)
	: inst(inst)
{
	
	served.assign(inst.nClients + 1, vector<bool>(inst.nPathes + 1, false));
	for (size_t customer = 1; customer < initialSolution.size(); customer++)
	{
		int pathIndex = initialSolution[customer].second;
		served[customer][pathIndex] = true;
		
		//for (int i = 0; i < inst.pathes[pathIndex].size(); i++)
			
		
	}
	for (int c = 1; c <= inst.nClients; ++c)
	{
		cout << served[c] << endl;
	}
}

