#ifndef HELPERS_H
#define HELPERS_H

template<class C>
ostream& operator<<(ostream& os, vector<C> v)
{
	for (size_t i = 0; i < v.size() - 1; ++i)
	{
		os << int(v[i]) << " ";
	}
	os << int(v[v.size()-1]);
	return os;
}

#endif
