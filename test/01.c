#include <stdio.h>
#include <string.h>
#include <cmap.h>

void test(cmap_key_t key, void *val_addr, void *any)
{
	(void)any;
	const char *s = *((char **)val_addr);
	printf("key: %s   val: %s\n", (char *)key, s);
}

void test2(cmap_key_t key, void *val_addr, void *any)
{
	(void)any;
 	const char *s = *((char **)val_addr);
	printf("key: %ld   val: %s\n", key, s);
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


    printf("----begin ptr ----\n");
    cmap_init(&map, (void *)strcmp, sizeof(char *));
    cmap_insert(&map, VAL_TO_KEY("5"), &five);
    cmap_insert(&map, VAL_TO_KEY("1"), &one);
    cmap_insert(&map, VAL_TO_KEY("2"), &two);

    cmap_erase(&map, VAL_TO_KEY("4"), NULL, NULL);
    cmap_insert(&map, VAL_TO_KEY("4"), &four);
    cmap_insert(&map, VAL_TO_KEY("three"), &three);
    s = NULL; printf("res=%d  val= ", cmap_find(&map, VAL_TO_KEY("4"), &s)); printf("%s\n", s);

    cmap_iter(&map, NULL, test);

    printf("---end ptr ---\n");
    cmap_destroy(&map, NULL, NULL);



    printf("---begin num ----\n");
    cmap_init(&map, NULL, sizeof(char *));
    cmap_insert(&map, 5, &five);
    cmap_insert(&map, 1, &one);
    cmap_insert(&map, 2, &two);

    cmap_erase(&map, 4, NULL, NULL);
    cmap_insert(&map, 4, &four);
    s = NULL; printf("res=%d  val= ", cmap_find(&map, 4, &s)); printf("%s\n", s);

    cmap_iter(&map, NULL, test2);

    printf("---end um ---\n");
    cmap_destroy(&map, NULL, NULL);



    return (0);
}
