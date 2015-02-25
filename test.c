
#include "slope-list.h"
#include <stdio.h>

int main()
{
    slope_list_t *list = NULL;
    
    slope_iterator_t *iter = slope_list_first(list);
    while (iter) {
        const char *word = (const char*) slope_iterator_data(iter);
        printf("%s\n", word);
        slope_iterator_next(&iter);
    }
    printf("list size: %ld\n", slope_list_size(list));
    slope_list_destroy(list);
    return 0;
}
