#include <stdio.h>
#include  <string.h>
#include "../cmap.h"

void test(void *key, void *val_addr, void *any)
{
  const char *s = *((char **)val_addr);

  printf("key: %s   val: %s\n", key, s);
}

int main()
{
    cmap_t map;
    char *s = NULL;


    const char *one = "one";
    const char *two = "two";
    const char *three = "three";
    const char *four = "four";
    const char *five = "five";


    printf("begin\n");
    cmap_init(&map, strcmp, sizeof(char *));
    cmap_insert(&map,"5", &five);
    cmap_insert(&map,"1", &one);
    cmap_insert(&map,"2", &two);

    cmap_erase(&map, "4", NULL, NULL);
    cmap_insert(&map, "4", &four);
    s = NULL; printf("res=%d  val= ", cmap_find(&map, "4", &s)); printf("%s\n", s);

    cmap_iter(&map, NULL, test);

    printf("end\n");
    cmap_destroy(&map, NULL, NULL);

    return (0);
}