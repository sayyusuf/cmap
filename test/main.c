#include <stdio.h>
#include  <string.h>
#include "cmap.h"

int main()
{
    cmap_t map;
    char *s = NULL;

    printf("basladik\n");
    cmap_new(&map, strcmp);
    cmap_insert(&map,"5", "beş");
    cmap_insert(&map,"1", "bir");
    cmap_insert(&map,"2", "iki");

    cmap_erase(&map, "5", NULL, NULL);
    
    s = NULL; printf("res=%d  val= ", cmap_find(&map, "1", &s)); printf("%s\n", s);

    cmap_destroy(&map, NULL, NULL);
    //argm_t *map = argm_new(strcmp);
  //  argm_insert(map,"5", "beş");
   // argm_insert(map, "3", "uc");
    //argm_insert(map,"4","dort");
    //argm_insert(map,"2", "iki");
    //argm_insert(map,"7","yedi");
    //argm_insert(map,"6","altı");
    //argm_insert(map,"8","sekiz");

//printf("%d\n", argm_insert(map,"8","sekiz"));
//argm_del(map, "5", NULL, NULL);
//    printf("%s\n", argm_find(map,"8"));
//    printf("first-> %s\n", map->first->value);
  //  argm_destroy(map, NULL, NULL);
    return (0);
}