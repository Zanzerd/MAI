#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

int main(void)
{
  int act = 0;
  ElemType key = 0;
  BST tree = NULL;
  printf("Компиляция \n\n");
  printf("Выберите операцию:\n");
  printf("1) Добавить элемент\n");
    printf("2) Удалить элемент\n");
    printf("3) Поиск элемента\n");
    printf("4) Печать дерева\n");
    printf("0) Выход\n");
    while (scanf("%d", &act) && act) {
        switch(act) {
        case 1:
            printf("Введите элемент ");
            scanf("%d", &key);
            TreeInsert(&tree, key);
            break;
        case 2:
            printf("Введите элемент: ");
            scanf("%d", &key);
            if (TreeFind(tree, key)) {
                tree = TreeRemove(tree, key);
            } else {
                printf("Такого элемента не существует\n");
            }
            break;
        case 3:
            printf("Введите элемент: ");
            scanf("%d", &key);
            if (TreeFind(tree, key)) {
                printf("Элемент найден!\n");
            } else {
                printf("Элемент не найден\n");
            }
            break;
        case 4:
            if (tree) {
                printf("\n");
                TreePrint(tree);
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
    TreeDestroy(tree);
    return SUCCESS;
}
