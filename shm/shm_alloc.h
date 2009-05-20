#ifndef _SHM_ALLOC_H_
#define _SHM_ALLOC_H_

typedef struct _node
{
	int x;
}node;

#ifdef _cpluscplus
extren "C" {
#endif

void shm_init(int key, size_t size, size_t slab_size);
void *shm_malloc(size_t n, void *ptr);
void *shm_ralloc(size_t n, void *ptr);
void shm_free(void *p, void *ptr);

#ifdef _cplusplus
}
#endif

#endif
