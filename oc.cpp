#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>       /* time */
#include <climits>
#include <unistd.h> /* sleep */
#include <chrono>
#include <random>
#include "solution.h"
#include "instance.h"

using namespace std;



Solution generateGreedySolution(Instance inst)
{	
    
	vector <pair <int, int> > initialSolution;
    
	initialSolution.assign(inst.nClients+1, make_pair(INT_MAX,-1));	
    /*
	vector<bool> alreadyPaidFor(inst.nFacilities + 1, false);
	for (int i = 1; i <= inst.nPathes; i++)
	{
		for(int j = 1; j <= inst.nClients; j++)
		{
            if (inst.costMatrix[i][j] == -1) continue;
            int cost = inst.costMatrix[i][j];
            for (size_t k = 0; k < inst.pathes[i].size(); ++k)
            {
                int f = inst.pathes[i][k];
                if (alreadyPaidFor[f]) continue;
                cost += inst.facilityCost[f];
                alreadyPaidFor[f] = true;
            }
			if (cost < initialSolution[j].first)
			{
                cout << "Replacing cost for client " << j << ": " << cost << endl;
				initialSolution[j].first = cost;
                
				initialSolution[j].second = i;
			}
		}
	}	
	*/
    
    // Another approach:
    // open 1/10 of the pathes
    // for each client, use the path with minimum cost to serve him.
    srand(time(NULL));
    set<int> pathesToOpen;
    while (pathesToOpen.size() < (unsigned int)inst.nPathes/10)
    {
        int p = (rand() % inst.nPathes) + 1;
        pathesToOpen.insert(p);
    }
    
    for (int j = 1; j <= inst.nClients; ++j)
    {
        for (auto it = pathesToOpen.begin(); it != pathesToOpen.end(); ++it)
        {
            if (inst.costMatrix[*it][j] == -1) continue;
            if (inst.costMatrix[*it][j] < initialSolution[j].first)
            {
                initialSolution[j].first = inst.costMatrix[*it][j];
                initialSolution[j].second = *it;
            }
        }
        if (initialSolution[j].second == -1)
        {
            for (int i = 1; i <= inst.nPathes; ++i)
            {
                if (inst.costMatrix[i][j] != -1)
                {
                    initialSolution[j] = pair<int,int>(inst.costMatrix[i][j], i);
                    pathesToOpen.insert(i);
                    break;
                }
            }
            
        }
    }

	return Solution(initialSolution, inst);
}

Solution sim_annealing(Solution s, double k, double t, double r, int stop1, int stop2)
{
    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed1);
    std::uniform_real_distribution<double> dist(0,1);
    for (int i = 0; i < stop2; ++i)
    {
        //start = high_resolution_clock::now();
        for (int j = 0; j < stop1; ++j)
        {
            double p = dist(generator);
            Solution n(s);
            if (p < 0.9)
                n = s.getNeighbour();
            else if (p < 0.95)
                n = s.getNeighbour2();
            else
                n = s.getNeighbour3();
            if (n.cost < s.cost)
                s = n;
            else
            {
                double x = dist(generator);
                if (x < exp( -((n.cost-s.cost)/(k*t)) ) )
                {
                    s = n;
                }
            }
        }
        t *= r;
        
        duration<double> span = duration_cast<duration<double>>(
            high_resolution_clock::now() - start);
        cout << "Outer loop iter " << i << ": sol cost => " << s.cost 
            << " after " << span.count() << " seconds (kT = " << k*t << ")" << endl;
        if (k*t <= 0.0001) 
        {
            cout << "Too cold!" << endl;
            break;
        }
    }
    return s;
}

int main(int argc, char** argv)
{
	
	double k, t;
    int stop1, stop2;
	double r;
	cout << argc << endl;
	if (argc != 11)
	{
		cout << "Usage: " << "./heur -k <float> -t <float> -r <float> -s1 <int> -s2 <int>" << endl;
		return 0;
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			char *arg = argv[i];
			switch(arg[1])
			{
				case 'k':
					i++;
					k = atof(argv[i]);
				break;
				case 't':
					i++;
					t = atof(argv[i]);
				break;
				case 'r':
					i++;
					r = atof(argv[i]);
				break;
				case 's':
					i++;
					if (arg[2] == '1')
					{
						stop1 = atoi(argv[i]);
					}
					else if (arg[2] == '2')
					{
						stop2 = atoi(argv[i]);
					}
				break;
			}
			
		}
	}
	
	cout << k << " " << t << " " << r  << " " << stop1<< " " << stop2 << endl;
	
	Instance inst = processInput();
    cout << "Cost Matrix is " << inst.costMatrix.size() << " x " << inst.costMatrix[0].size() << endl;
	cout << "Generated Instance" << endl;
	Solution s = generateGreedySolution(inst);
    Solution final = sim_annealing(s, k, t, r, stop1, stop2);
    cout << "Best solution's cost: " << final.cost << endl;
}
