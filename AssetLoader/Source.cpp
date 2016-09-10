#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;




unsigned long factorial(unsigned long f)
{
	if (f == 0)
		return 1;
	return(f * factorial(f - 1));
}
unsigned long power(int base, int exp)
{
	unsigned long rval = base;
	for (int i = 0;i<exp;++i)
		rval *= base;
	return rval;
}
int main() {
	unsigned long m = power(10, 9) + 7;
	unsigned long int cunt = 40;
	int anotheranswer = 0;
	unsigned long n = 0;
	unsigned long in = 0;
	vector<long> a;
	cin >> n;
	for (long i = 0; i<n;i++)
	{
		cin >> in;
		a.push_back(in);
	}
	unsigned long int cock;
	for (int i;i<n;++i)
	{
		if (a[i] == -1)
			cock++;
	}

	
	
	if (cock) 
	{
		for (int i;i < cock;cock++)
		{
			cunt = cock * (cock + i);
		}
	}
	cout << cunt%m;
	return 0;
}