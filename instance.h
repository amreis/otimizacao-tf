#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <map>
#include <set>

using namespace std;

class Instance 
{
private:
    unsigned int INF = 1000000;
public:
	vector<vector<int> > pathes;
	int nClients;
	int nFacilities;
	int nPathes;
	vector<int> facilityCost;
	vector<vector<int> > costMatrix;
	map<int, set<int> > pathesWithFacility;
    
	Instance();
	Instance(int, int, int);
	
	void setAllFacilityCosts(int);
	void setRangeFacilityCost(int from, int to, int cost);
	void addFacilityToPath(int path, int facility);
	void setCostBetween(int path, int client, int cost);

    void writeGLPKFile(const char*);
};

Instance processInput();

#endif
