/**Update this file with the starter code**/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lab.h"

list_t *list_init(void (*destroy_data)(void *),int (*compare_to)(const void *, const void *)) {
    list_t *list = (list_t*)malloc(sizeof(list_t)); // Allocate memory for the list
    // Check if the memory allocation was successful
    if (list == NULL) {
        printf("List memory allocation failed\n");
        exit(1);
    }

    // Initialize the list
    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;
    list->head = (node_t*)malloc(sizeof(node_t)); // Allocate memory for the head/sentinel node
    
    // Check if the memory allocation was successful
    if (list->head == NULL) {
        destroy_data(list);
        printf("Node memory allocation failed\n");
        exit(1);
    }

    // Initialize the head/sentinel node
    list->head->data = NULL; // Sentinel node stores no data
    list->head->next = list->head; 
    list->head->prev = list->head; 

    return list;
}

void list_destroy(list_t **list) {
    // Check if the list is NULL or if the pointer to the list is NULL
    if (list == NULL || *list == NULL) {
        return;
    }

    // Free all nodes in the list
    node_t *curr = (*list)->head->next;
    while (curr != (*list)->head) {
        // Call the destroy_data function pointer to free the memory allocated for the data
        (*list)->destroy_data(curr->data);
        // Store the memory address of the next node in curr
        node_t *next = curr->next;
        // Free the memory allocated for the current node
        free(curr);
        // Move to the next node in the list
        curr = next;
    }

    // Free all associated data and the head node
    free((*list)->head); // Free the memory allocated for head/sentinel node of the list
    free(*list); // Free the memory allocated for the pointer to the list
    *list = NULL; 
}

list_t *list_add(list_t *list, void *data) {
    // Check if the list is NULL or if the data is NULL
    if (list == NULL || data == NULL) {
        return list;
    }

    // Create a new node to store the data
    node_t *new_node = malloc(sizeof(node_t));

    // Check if the memory allocation was successful
    if (new_node == NULL) {
        return list;
    }

    // Initialize the new node
    new_node->data = data;
    new_node->next = list->head->next;
    new_node->prev = list->head;

    // Update the pointers of adjacent nodes
    list->head->next->prev = new_node;
    list->head->next = new_node;

    // Increment the size of the list
    list->size++;

    return list;
}

void *list_remove_index(list_t *list, size_t index) {
    // Check if the list is NULL or if the index is out of bounds
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    // Find the node at the specified index
    node_t *curr = list->head->next;
    for (size_t i = 0; i < index; i++) {
        curr = curr->next;
    }

    // Update the pointers of adjacent nodes
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;

    // Store the data to be removed
    void *data = curr->data;

    // Free the memory allocated for the node
    free(curr);

    // Decrement the size of the list
    list->size--;

    return data;
}

int list_indexof(list_t *list, void *data) {
    // Check if the list is NULL or if the data is NULL
    if (list == NULL || data == NULL) {
        return -1;
    }

    // Find the index of the data in the list
    node_t *curr = list->head->next;
    int index = 0;
    while (curr != list->head) {
        if (list->compare_to(curr->data, data) == 0) {
            return index;
        }
        curr = curr->next;
        index++;
    }

    return -1;
}