#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "bst.h"

int main(void)
{
    void (*TreeInsert)(BST *root, ElemType newKey);
    BST (*TreeFind)(BST root, ElemType key);
    BST (*TreeRemove)(BST root, ElemType key);
    void (*TreePrint)(BST root);
    void (*TreeDestroy)(BST root);
    char *err;

    void *libHandle;
    libHandle = dlopen("libbst.so", RTLD_LAZY);
    if (!libHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(FAILURE);
    }

    TreeInsert = dlsym(libHandle, "TreeInsert");
    TreeRemove = dlsym(libHandle, "TreeRemove");
    TreeFind = dlsym(libHandle, "TreeFind");
    TreePrint = dlsym(libHandle, "TreePrint");
    TreeDestroy = dlsym(libHandle, "TreeDestroy");

    if(err = dlerror()) {
      fprintf(stderr, "%s\n", err);
      exit(FAILURE);
    }

    int act = 0;
    ElemType key = 0;
    BST tree = NULL;
    printf("Рантайм \n\n");
    printf("Выберите операцию:\n");
    printf("1) Добавить элемент\n");
    printf("2) Удалить элемент\n");
    printf("3) Поиск элемента\n");
    printf("4) Печать дерева\n");
    printf("0) Выход\n");
    while (scanf("%d", &act) && act) {
        switch(act) {
        case 1:
            printf("Введите элемент: ");
            scanf("%d", &key);
            (*TreeInsert)(&tree, key);
            break;
        case 2:
            printf("Введите элемент: ");
            scanf("%d", &key);
            if ((*TreeFind)(tree, key)) {
                tree = (*TreeRemove)(tree, key);
            } else {
                printf("Такого элемента не существует\n");
            }
            break;
        case 3:
            printf("Введите элемент ");
            scanf("%d", &key);
            if ((*TreeFind)(tree, key)) {
                printf("Ключ найден\n");
            } else {
                printf("Ключ не найден\n");
            }
            break;
        case 4:
            if (tree) {
                printf("\n");
                (*TreePrint)(tree);
                printf("\n");
            } else {
                printf("Дерево пусто\n");
            }
            break;
        default:
	  printf("Такого пункта меню не существует\n");
	  break;
        }
	printf("Выберите операцию:\n");
	printf("1) Добавить элемент\n");
	printf("2) Удалить элемент\n");
	printf("3) Поиск элемента\n");
	printf("4) Печать дерева\n");
	printf("0) Выход\n");
    }
    (*TreeDestroy)(tree);
    dlclose(libHandle);
    return SUCCESS;
}
