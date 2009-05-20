#include "shm_allocator.h"

#define kk std::cout << "..." << std::endl;

using namespace shm;

int main()
{
	int ia[]={0,1,2,3,4,5};
	map<int, vector<int> > mm;
	int i=0;
	for(int i=0; i<100;i++)
	{
		vector<int> sd;
		sd.push_back(100);
		sd.push_back(200);
		sd.push_back(300);
		mm[i]= sd;
	}
}

