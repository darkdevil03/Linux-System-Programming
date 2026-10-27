#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

static void insert(struct node *head,int data) {
    struct node *travel = head;
    struct node *temp = (struct node*) malloc(sizeof(struct node));
    while (travel -> next != NULL) {
        travel = travel -> next;
    }
    temp->data = data;
    temp->next = NULL;
    travel -> next = temp;

}

int main() {
    struct node *head = NULL;

    struct node *temp = (struct node*) malloc(sizeof(struct node));
    temp->data = 10;
    temp->next = NULL;
    head = temp;

    printf("Enter the data to create a linked list : ");
    int dat;
    for (int i = 1; i < 5; i++) {
        scanf("%d",&dat);
        insert(head,dat);
    }

    struct node *travel = head;
    printf("List : ");
    for (int i = 0 ; i < 5; i++) {
        if (travel!= NULL) {
            printf("%d ",travel->data);
            travel = travel -> next;
        }
        printf(" ");
    }
    printf("\n");

    int result[5];
    struct node *travel2 = head;
    
    for (int i = 0 ; i < 5; i++) {
        if (travel2 != NULL) {
            result[i] = travel2 -> data;
            travel2 = travel2 -> next;
        }
    }

    printf("Reversed array : \n");
    for (int i = 4; i <= 0 ; i--) {
        printf("%d ", result[i]);
    }

    return 0;
}