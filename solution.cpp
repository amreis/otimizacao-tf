#include "solution.h"
#include <algorithm>

#include <random>
#include <chrono>


Solution::Solution(const Solution& s)
    : openFacility(s.openFacility), 
      served(s.served),
      cost(s.cost),
      inst(s.getInst())
{

}

Solution Solution::getNeighbour ()
{
    
  unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    Solution n(*this);
    /*
    #ifdef DEBUG
    for (int client = 1; client <= inst.nClients; ++client)
    {
        int c = count(n.served[client].begin(), n.served[client].end(), true);
        if (c != 1)
        {
            cout << "Client " << client << " is served by " << c << " pathes!" << endl;
        }
    }
    #endif
    * */
	srand (time(NULL));
    std::default_random_engine generator(seed1);
    std::uniform_int_distribution<unsigned int> dist1(1, inst.nClients);
    
	int	randomCustomer = dist1(generator);
	
	vector < int > pathsThatReachCustomer;
	for (int i=1; i <= inst.nPathes; i++)
	{
		if (inst.costMatrix[i][randomCustomer] >= 0)
			pathsThatReachCustomer.push_back(i);		
	}
    std::uniform_int_distribution<unsigned int> dist2(0, pathsThatReachCustomer.size() - 1);
    int newPath;
    do 
    {
        newPath = dist2(generator);
    } while (served[randomCustomer][pathsThatReachCustomer[newPath]]);
    newPath = pathsThatReachCustomer[newPath];
    
    for (int facility : n.inst.pathes[newPath])
    {
        if (!n.openFacility[facility])
        {
            n.openFacility[facility] = true;
            n.cost += n.inst.facilityCost[facility];
        }
    }
    
    
    int weClosed;
    
    for (int i = 1; i <= inst.nPathes; ++i)
    {
        if (n.served[randomCustomer][i])
        {
            n.cost -= inst.costMatrix[i][randomCustomer];
            n.served[randomCustomer][i] = false;
            weClosed = i;
            break;
        }
    }
    
    n.cost += inst.costMatrix[newPath][randomCustomer];
    n.served[randomCustomer][newPath] = true;

    // If no one else uses the path we just closed, we need to see if we can
    // close any facilities
    bool used = false;
	for (int j = 1; j <= inst.nClients; ++j)
    {
        if (n.served[j][weClosed])
        {
            used = true;
            break;
        }
    }
    if (used) return n;
    else
    {
        for (int facility : inst.pathes[weClosed])
        {
            bool facilityIsUsed = false;
            for (int path : inst.pathesWithFacility[facility])
            {
                bool pathIsUsed = false;
                for (int j = 1; j <= inst.nClients; ++j)
                {
                    if (n.served[j][path])
                    {
                        pathIsUsed = true;
                         break;
                    }
                }
                if (pathIsUsed)
                {
                    facilityIsUsed = true;
                    break;
                }
            }
            if (!facilityIsUsed)
            {
                n.cost -= inst.facilityCost[facility];
                n.openFacility[facility] = false;
            }
        }
        return n;
    }
}

Solution::Solution(vector < pair <int, int> > initialSolution, Instance &inst)
	: inst(inst)
{
	this->cost = 0;
	served.assign(inst.nClients + 1, vector<bool>(inst.nPathes + 1, false));
    openFacility.assign(inst.nFacilities + 1, false);
	for (size_t customer = 1; customer < initialSolution.size(); customer++)
	{
		int pathIndex = initialSolution[customer].second;
		served[customer][pathIndex] = true;
		this->cost += inst.costMatrix[pathIndex][customer];
        for (int f : inst.pathes[pathIndex])
        {
            this->openFacility[f] = true;
        }
		//for (int i = 0; i < inst.pathes[pathIndex].size(); i++)
			
		
	}
    
    for (bool f : openFacility)
    {
        if (f)
        {
            this->cost += inst.facilityCost[f];
        }
    }
    cout << openFacility << endl;
    for (int client = 1; client <= inst.nClients; ++client)
    {
        int c = count(served[client].begin(), served[client].end(), true);
        if (c != 1)
        {
            cout << "Client " << client << " is served by " << c << " pathes!" << endl;
        }
    }
	/*for (int c = 1; c <= inst.nClients; ++c)
	{
		cout << served[c] << endl;
	}
    */
    cout << "Greedy solution cost: " << this->cost << endl;
}

