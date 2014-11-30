#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>       /* time */
#include <climits>

#include "solution.h"
#include "instance.h"

using namespace std;



Solution generateGreedySolution(Instance inst)
{	
	vector <pair <int, int> > initialSolution;
	initialSolution.assign(inst.nClients+1, make_pair(INT_MAX,-1));	
	
	for (int i = 1; i <= inst.nPathes; i++)
	{
		for(int j = 1; j <= inst.nClients; j++)
		{		
			if (inst.costMatrix[i][j] != -1 && inst.costMatrix[i][j] < initialSolution[j].first)
			{
				initialSolution[j].first = inst.costMatrix[i][j];
				initialSolution[j].second = i;
			}
		}
	}	
	
	return Solution(initialSolution, inst);
}



// ./prog -k <int> -t <int> -r <int> -s1 <int> -s2 <int>
int main(int argc, char** argv)
{
	/*
	int k, t, stop1, stop2;
	double r;
	cout << argc << endl;
	if (argc != 11)
	{
		cout << "Usage: " << "./prog -k <int> -t <int> -r <float> -s1 <int> -s2 <int>" << endl;
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
					k = atoi(argv[i]);
				break;
				case 't':
					i++;
					t = atoi(argv[i]);
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
	*/
	Instance inst = processInput();
	cout << "Generated Instance" << endl;
	Solution s = generateGreedySolution(inst);
}
