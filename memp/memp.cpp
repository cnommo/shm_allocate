#include "memp.h"

MP *memp_init(size_t n)
{
	n=((((n-1)/4+1)-1)/16+1)*16;
	if(n<64) 	n=64;
	if(n>4096) 	n=4096;

	char  *p=(char *)malloc(sizeof(MP)+sizeof(attr)*n);
	if(p==NULL) 
		return NULL;

	MP *mp=(MP *)p;
	mp->num=n;
	mp->addr=(pattr)(p+sizeof(MP));

	for(size_t i=0;i<n;i++)
	{
		pattr ptr=mp->addr+i;
		ptr->pd=NULL;
		ptr->size=i+1;
		ptr->next=0;
		ptr->curnum=100;
		ptr->maxnum=1000;
		ptr->empty=0;
		ptr->maxem=3;
	}		
	return mp;
}

bool memp_destory(MP *mp)
{
	if(mp==NULL)
		return false;
	memp_clean(mp);
	for(size_t i=0;i<mp->num;i++)
	{
		pnode pd=(mp->addr+i)->pd;
		if(pd!=NULL)
			return false;
	}
	free(mp);
	return true;
}

void *memp_getblock(MP *mp, size_t size)
{
	if(size==0)
		return NULL;

	if((size-1)/4+1>mp->num)
	{
		char *p=(char *)malloc(size+sizeof(block));
		pblock pb=(pblock)p;
		pb->var=NULL;
		return (void *)(p+sizeof(block));	
	}
	else
	{
		pattr ptr=mp->addr+(size-1)/4;
		pnode head=ptr->pd, tmp=head;
		if(head==NULL)
		{
			addblock(&head, size, ptr->curnum);
			ptr->empty++;
		}
		while(head->pb==NULL)
		{
			head=head->next;
			if(head==tmp)
			{
				if((ptr->curnum)*2<ptr->maxnum)
					(ptr->curnum)*=2;
				else 
					ptr->curnum=ptr->maxnum;
				addblock(&head, size, ptr->curnum);
				ptr->empty++;
			}
		}
		ptr->pd=head;
		if(head->use==0)
			ptr->empty--;
		pblock pb=head->pb;
		head->pb=(pblock)(pb->var);
		head->use++;
		pb->var=head;
		return (void *)((char *)pb+sizeof(block));
	}
}

void memp_retblock(MP *mp, void * p)
{
	if(mp==NULL && p==NULL)
		return;
	pblock pb=(pblock)((char *)p-sizeof(block));
	pnode  pd=(pnode)(pb->var);
	if(pd==NULL)
	{
		free(pb);
		return;
	}
	pb->var=pd->pb;
	pd->pb=pb;
	pd->use--;
	if(pd->use==0)
	{
		pattr  ptr=mp->addr+(pd->size-1)/4;
		if(ptr->empty>ptr->maxem)
		{
			if(ptr->pd==pd)  
				ptr->pd=pd->next;	
			pd->pre->next=pd->next;
			pd->next->pre=pd->pre;
			free(pd);
		}
		else
			ptr->empty++;
	}
}

void memp_clean(MP *mp)
{
	for(size_t i=0;i<mp->num;i++)
	{
		pattr ptr=mp->addr+i;
		pnode pd=ptr->pd,tmp=pd;
		if(pd==NULL)
			continue;
		while(1)
		{
			if(pd->use==0)
			{
				ptr->empty--;
				if(pd->next==pd)
				{
					ptr->pd=NULL;
					free(pd);
					break;
				}
				else
				{
					if(ptr->pd==pd)
						ptr->pd=pd->next;
					pd->pre->next=pd->next;
					pd->next->pre=pd->pre;
					free(pd);
				}
			}
			pd=pd->next;
			if(pd==tmp) break;
		}
	}
}

void memp_statistic(MP *mp, char *p)
{
	size_t sum=0,use=0;
	for(size_t i=0;i<mp->num;i++)
	{
		pattr ptr=mp->addr+i;
		pnode pd=ptr->pd,tmp=pd;
		
		if(pd==NULL)
			continue;
		
		while(1)
		{
			sum+=(pd->num)*(pd->size);
			use+=(pd->use)*(pd->size);
			pd=pd->next;
			if(pd==tmp)
				break;
		}
	}
	printf("\nsum=%d\nuse=%d\n",sum,use);
}

bool addblock(pnode *pd, size_t size, size_t num)
{
	size_t dlen=sizeof(node);
	size_t blen=size+sizeof(block);

	char *p=(char *)malloc(dlen+blen*num);
	if(p==NULL) return false;

	pnode pn=(pnode)p;
	pn->num=num;
	pn->use=0;
	pn->size=size;
	pn->pb=NULL;

	if(*pd==NULL)
	{	
		pn->pre=pn;
		pn->next=pn;
	}
	else
	{	
		pn->next=(*pd)->next;
		(*pd)->next=pn;
		pn->pre=*pd;
		pn->next->pre=pn;
	}
	*pd=pn;

	pblock pk=(pblock)(p+=dlen);
	for(size_t i=0;i<num;i++)
	{
		pk->var=(pblock)(pn->pb);
		pn->pb=pk;
		pk=(pblock)(p+=blen);
	}
	return true;
}
