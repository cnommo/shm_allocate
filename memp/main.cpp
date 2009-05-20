#include "memp.h"

int main()
{
	MP *mp=memp_init(1020);
	char *p[1000];
	for(int i=0;i<1000;i++)
	{
		for(int j=0;j<1000;j++)
			p[j]=(char *)memp_getblock(mp,j);
	
		for(int j=0;j<1000;j++)
		{
			if(p[j]!=NULL)
				memp_retblock(mp,p[j]);
		}
		
		printf("ok...\n");
		//system("pause");
	}
	system("pause");
	//memp_statistic(mp);
	memp_destory(mp);
	return 0;
}
