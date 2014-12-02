#include "solution.h"
#include <algorithm>

#include <random>
#include <chrono>
#include <climits>
#include <set>

Solution::Solution(const Solution& s)
    : openFacility(s.openFacility), 
      served(s.served),
      cost(s.cost),
      inst(s.getInst())
{

}

Solution Solution::getNeighbour3()
{
    // New neighborhood: close one path.
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    Solution n(*this);
    std::default_random_engine generator(seed1);
    
    std::uniform_int_distribution<unsigned int> dist(1, inst.nPathes);
    
    int close = dist(generator);
    set<int> openPathes;
    for (int i = 1; i <= inst.nClients; ++i)
    {
        for (int l = 1; l <= inst.nPathes; ++l)
        {
            if (served[i][l])
            {
                openPathes.insert(l);
                break;
            }
        }
    }
    if (openPathes.empty() || openPathes.find(close) == openPathes.end()) return Solution(*this);
    for (int j = 1; j <= inst.nClients; ++j)
    {
        if (served[j][close])
        {
            n.cost -= inst.costMatrix[close][j];
            n.served[j][close] = false;
            int bestCost = INT_MAX, best = -1;
            for (int l = 1; l <= inst.nPathes; ++l)
            {
                if (l == close) continue;
                if (inst.costMatrix[l][j] != -1 && openPathes.find(l) != openPathes.end() && bestCost > inst.costMatrix[l][j])
                {
                    bestCost = inst.costMatrix[l][j];
                    best = l;
                }
            }
            if (best == -1)
                return Solution(*this);
            n.served[j][best] = true;
            for (int facility : inst.pathes[best])
            {
                if (!openFacility[facility])
                {
                    openFacility[facility] = true;
                    n.cost += inst.facilityCost[facility];
                }
            }
            
            n.cost += bestCost;
        }
    }
    
    for (int facility : inst.pathes[close])
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

Solution Solution::getNeighbour2()
{
    // New neighborhood: open one path that is not yet open
    // For every client, if the cost of being serviced by this path 
    // is less than that of the path that currently services it,
    // change it.
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    Solution n(*this);
    std::default_random_engine generator(seed1);
    
    vector<int> pathesNotOpen;
    
    for (int l = 1; l <= inst.nPathes; ++l)
    {
        bool isOpen = false;
        for (int j = 1; j <= inst.nClients; ++j)
        {
            if (n.served[j][l])
            {
                isOpen = true;
                break;
            }
        }
        if (!isOpen) pathesNotOpen.push_back(l);
    }
    
    std::uniform_int_distribution<unsigned int> dist1(0, pathesNotOpen.size() - 1);
    if (pathesNotOpen.empty()) return n;
    int openMePls = pathesNotOpen[dist1(generator)];
    if (openMePls == 0) cout << pathesNotOpen << endl;
    for (int j = 1; j <= inst.nClients; ++j)
    {
        int servicedBy = -1;
        for (int l = 1; l <= inst.nPathes; ++l)
        {
            if (n.served[j][l])
            {
                
                servicedBy = l;
                break;
            }
        }
        if (servicedBy <= 0 || servicedBy >= (int)inst.costMatrix.size()) { cout << "INCONSISTENT servicedBy. (" << servicedBy << ")" << endl; return n; }
        if (openMePls <= 0 || openMePls >= (int)inst.costMatrix.size()) { cout << "INCONSISTENT openMePls. (" << openMePls << ")" << endl; return n; }
        //if (servicedBy == -1) { cout << "INCONSISTENT servicedBy." << endl; return n; }
        if (inst.costMatrix[openMePls][j] != -1 && inst.costMatrix[servicedBy][j] > inst.costMatrix[openMePls][j])
        {
            //used = true;
            n.served[j][servicedBy] = false;
            n.served[j][openMePls] = true;
            n.cost -= inst.costMatrix[servicedBy][j];
            n.cost += inst.costMatrix[openMePls][j];
            bool used = false;
            for (int j = 1; j <= inst.nClients; ++j)
            {
                if (n.served[j][servicedBy])
                {
                    used = true;
                    break;
                }
            }
            if (used) continue;
            for (int facility : inst.pathes[servicedBy])
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
        }
    }
    for (int facility : inst.pathes[openMePls])
    {
        if(!n.openFacility[facility])
        {
            n.openFacility[facility] = true;
            n.cost += n.inst.facilityCost[facility];
        }
    }
    return n;
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
    
    for (int i = 1; i <= inst.nFacilities; ++i)
    {
        if (openFacility[i])
        {
            this->cost += inst.facilityCost[i];
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

