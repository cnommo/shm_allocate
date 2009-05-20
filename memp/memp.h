#include <stdio.h>
#include <stdlib.h>

// data structors
typedef struct _block
{
	void *var;
}block, *pblock;

typedef struct _node
{
	size_t num;
	size_t use;
	size_t size;
	struct _block *pb;
	struct _node  *pre;
	struct _node  *next;
}node, *pnode;

typedef struct _attr
{
	pnode  pd;
	size_t size;
	size_t next;
	size_t curnum;
	size_t maxnum;
	size_t empty;
	size_t maxem;
}attr, *pattr;

typedef struct _mp
{
	size_t num;
	pattr  addr;
}MP, *pMP;

//functions
MP   *memp_init(size_t n=1024);
bool memp_destory(MP *mp);
void *memp_getblock(MP *mp, size_t size);
void memp_retblock(MP *mp, void * p);
void memp_clean(MP *);
void memp_statistic(MP *mp, char *p=NULL);
bool addblock(pnode *pd, size_t size, size_t num);

