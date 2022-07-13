#include "ngx_mem_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ngx_log_error_core(ngx_uint_t level, const char *fmt, ...)
{
}

typedef struct Data stData;
struct Data
{
    char *ptr;
    FILE *pfile;
};

void func1(void *p1) // void(*)(void*)
{
    //进行一个类型强转
    char *p = (char *)p1;
    printf("free ptr mem!");
    free(p);
}
void func2(void *pf1)
{
     //进行一个类型强转
     FILE *pf = (FILE*)pf1;
    printf("close file!");
    fclose(pf);
}
int main()
{
    // （可用字节）512 - sizeof(ngx_pool_t) 《-》 4095   =>   max
    // ngx_create_pool的代码逻辑可以直接实现在mempool的构造函数当中
    ngx_mem_pool mempool;
    auto pool = mempool.ngx_create_pool(512); //创建内存池
    if (pool == nullptr)
    {
        printf("ngx_create_pool fail...");
        return 0;
    }

    void *p1 = mempool.ngx_palloc(128); // 从小块内存池分配的
    if (p1 == NULL)
    {
        printf("ngx_palloc 128 bytes fail...");
        return 0;
    }

    stData *p2 = (stData *)mempool.ngx_palloc(512); // 从大块内存池分配的
    if (p2 == NULL)
    {
        printf("ngx_palloc 512 bytes fail...");
        return 0;
    }
    p2->ptr = (char *)malloc(12);
    strcpy(p2->ptr, "hello world");
    p2->pfile = fopen("data.txt", "w");

    ngx_pool_cleanup_s *c1 = mempool.ngx_pool_cleanup_add(sizeof(char *));
    c1->handler = func1;
    c1->data = p2->ptr;

    ngx_pool_cleanup_s *c2 = mempool.ngx_pool_cleanup_add(sizeof(FILE *));
    c2->handler = func2;
    c2->data = p2->pfile;

    // ngx_destroy_pool的代码逻辑可以直接实现在mempool的析构函数之中
    mempool.ngx_destroy_pool(); // 1.调用所有的预置的清理函数 2.释放大块内存 3.释放小块内存池所有内存

    return 0;
}
