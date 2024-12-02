#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 40
char* s_gets(char* st, int n);

typedef struct film
{
    char         Name[NAME_SIZE];
    int          rating;
    struct film* next;
} film_list;

static film_list* head;
static film_list *prev, *current;

int main()
{
    char buf[NAME_SIZE];

    puts("Enter your film name:");
    while (s_gets(buf, NAME_SIZE) != NULL && buf[0] != 'q') {
        current = (film_list*)malloc(sizeof(film_list));
        if (head == NULL)
            head = current;
        else {
            prev->next = current;
        }

        current->next = NULL;
        strncpy(current->Name, buf, strlen(buf) + 1);
        puts("Enter your film rating(1 ~ 10):");
        scanf("%d", &current->rating);
        while (getchar() != '\n') continue;
        puts("Enter your film next name (q to stop):");
        prev = current;
    }

    if (head == NULL)
        printf("film list is empty\n");
    else
        printf("film list is : \n");

    current = head;
    printf("NAME    RATING\n");
    while (current != NULL) {
        printf("%s     %d\n", current->Name, current->rating);
        current = current->next;
    }
    current = head;
    while (current != NULL) {
        current = head;
        head    = current->next;
        free(current);
    }
    printf("end");
    return 0;
}

char* s_gets(char* st, int n)
{
    char* ret_val;
    char* find;
    ret_val = fgets(st, n, stdin);
    if (ret_val) {
        find = strchr(st, '\n');
        if (find)
            *find = '\0';
        else
            while (getchar() != '\n') continue;
    }
    return ret_val;
}