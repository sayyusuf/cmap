#include <stdio.h>
#include "argm_lib.h"
#include  <string.h>

int main()
{
    argm_t *map = argm_new(strcmp);
    argm_insert(map,"5", "beş");
   // argm_insert(map, "3", "uc");
    //argm_insert(map,"4","dort");
    //argm_insert(map,"2", "iki");
    //argm_insert(map,"7","yedi");
    //argm_insert(map,"6","altı");
    //argm_insert(map,"8","sekiz");

//printf("%d\n", argm_insert(map,"8","sekiz"));
argm_del(map, "5", NULL, NULL);
//    printf("%s\n", argm_find(map,"8"));
//    printf("first-> %s\n", map->first->value);
    argm_destroy(map, NULL, NULL);
    return (0);
}