#include "instance.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <climits>
#include <cmath>

#include "helpers.h"

using namespace std;

Instance::Instance() { }

Instance::Instance(const Instance& i)
    : pathes(i.pathes),
      nClients(i.nClients),
      nFacilities(i.nFacilities),
      nPathes(i.nPathes),
      facilityCost(i.facilityCost),
      costMatrix(i.costMatrix),
      pathesWithFacility(i.pathesWithFacility)
{
}

Instance::Instance(int nClients, int nFacilities, int nPathes)
	: nClients(nClients), nFacilities(nFacilities), nPathes(nPathes)
{

	pathes.assign(nPathes + 1, vector<int>());
	facilityCost.assign(nFacilities + 1, -1);
	costMatrix.assign(nPathes + 1, vector<int>(nClients + 1, -1));
}

void Instance::setAllFacilityCosts(int c)
{
	this->facilityCost.assign(nFacilities+1, c);
}

void Instance::setRangeFacilityCost(int from, int to, int cost)
{
	if (from > to) return;
	for (int i = from; i <= min(to+1, (int)facilityCost.size()-1); ++i)
	{
		facilityCost[i] = cost;
	}
}

void Instance::addFacilityToPath(int path, int facility)
{
	if (path <= 0 || path > nPathes)
		{ cout << "Invalid path index. (" << path << ")" << endl; return; }
	pathes[path].push_back(facility);
	pathesWithFacility[facility].insert(path);
}

void Instance::setCostBetween(int path, int client, int cost)
{
	if (path <= 0 || path > nPathes)
		{ cout << "Invalid path index. (" << path << ")" << endl; return; }
	if (client <= 0 || client > nClients)
		{ cout << "Invalid client index. (" << client << ")" << endl; return; }
	costMatrix[path][client] = cost;
    INF += cost;
}




Instance processInput()
{
	string line;
	// Ignore first line.
	getline(cin, line);
	getline(cin, line);
	
	int nc, nf, np;
	cin >> nf >> np >> nc;
	Instance inst(nc, nf, np);
	int cost;
	cin.ignore();
	cin >> cost;
	if (cin.peek() == '-')
	{
		cin.ignore();
		int from, to;
		from = cost;
		cin >> to;
		cin.ignore();
		int cost;
		cin >> cost;
		inst.setRangeFacilityCost(from, to, cost);
		cout << from << " " << to << " " << cost << endl;
		while (cin.peek() == ',')
		{
			cin.ignore();
			int from, to;
			cin >> from;
			cin.ignore();
			cin >> to;
			cin.ignore();
			int cost;
			cin >> cost;
			inst.setRangeFacilityCost(from, to, cost);
			cout << from << " " << to << " " << cost << endl;
		}
		
	}
	else
		inst.setAllFacilityCosts(cost);

	getline(cin, line);
	getline(cin, line);
	for (int i = 0; i < np; ++i)
	{

		getline(cin, line);
		cin.ignore();

		stringstream s(line);
		int facility;
		while (s >> facility)
		{
			inst.addFacilityToPath(i+1, facility);
			
		}
        cout << "Path " << i+1 << " : ";
        for (int s : inst.pathes[i+1])
        {
            cout << s << " ";
        }
        cout << endl;
	}


	getline(cin, line); getline(cin, line);
	int path, client;

	while (cin >> path >> client >> cost)
	{
		inst.setCostBetween(path, client, cost);
	}
	return inst;
}

void Instance::writeGLPKFile(const char *filename)
{
    if (this->INF <= 1000000) this->INF *= 10;
    ofstream out;
    if (filename == NULL) return;
    out.open(filename);
    
    out << "set I;" << endl << "set J;" << endl << "set L;" << endl;
    
    out << "param F{i in I};" << endl;
    out << "param C{l in L, j in J};" << endl;
    
    out << "var x{l in L, j in J} binary;" << endl;
    out << "var y{i in I} binary;" << endl << endl;
    
    out << "minimize obj: sum{i in I}(F[i]*y[i]) + sum{j in J, l in L}(C[l,j]*x[l,j]);" << endl
        << endl;
    
    out << "s.t. R1{j in J}: sum{l in L}(x[l,j]) = 1;" << endl;
    
    for (int i = 1; i <= nFacilities; ++i)
    {
        for (int j = 1; j <= nClients; ++j)
        {
            out << "s.t. R" << i*(nClients+1) + j << ":";
            out << "y[" << i << "] >= ";
            auto l = pathesWithFacility[i].begin();
            for ( ; l != pathesWithFacility[i].end(); ++l)
            {
                l++;
                if (l == pathesWithFacility[i].end()) break;
                l--;
                out << "x[" << *l << ", " << j << "] +";
            }
            l--;
            out << "x[" << *l << ", " << j << "];" << endl;
        }
    }
    
    out << "data;" << endl;
    out << "set I := ";
    for (int i = 1; i < nFacilities; ++i)
        out << i << " ";
    out << nFacilities << ";" << endl;
    out << "set J := ";
    for (int j = 1; j < nClients; ++j)
    {
        out << j << " ";
    }
    out << nClients << ";" << endl;
    out << "set L := ";
    for (int l = 1; l < nPathes; ++l)
        out << l << " ";
    out << nPathes << ";" << endl;
    
    out << "param F :=" << endl;
    for (int i = 1; i <= nFacilities; ++i)
    {
        out << "\t" << i << " " << facilityCost[i] << endl;
    }
    out << "\t;" << endl;
    out << "param C : " << endl;
    out << "\t";
    for (int j = 1; j <= nClients; ++j)
    {
        out << j << "\t";
    }
    out << ":= \n";
    for (int l = 1; l <= nPathes; ++l)
    {
        out << "\t" << l << " ";
        for (int j = 1; j <= nClients; ++j)
        {
            if (costMatrix[l][j] == 0)
            {
                cout << "No cost for path " << l << " to client " << j << endl;
            }
            out << "\t" << (costMatrix[l][j] == -1 ? INF : costMatrix[l][j]);
        }
        out << endl;
    }
    out << ";" << endl;
    out.close();
}

