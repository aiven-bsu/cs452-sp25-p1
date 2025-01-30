#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "lab.h"

/**
 * Create a new list with callbacks to deal with the data that the
 * list is storing. 
 *
 * @param destroy_data Function that will free the memory for user supplied data
 * @param compare_to Function that will compare two user data elements
 * @return struct list* pointer to the newly allocated list.
 */
list_t *list_init(void (*destroy_data)(void *),int (*compare_to)(const void *, const void *)) {
    list_t *list = (list_t*)malloc(sizeof(list_t)); // Allocate memory for the list
    // Check if the memory allocation was successful
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
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
        fprintf(stderr, "Error: Node memory allocation failed\n");
        exit(1);
    }

    // Initialize the head/sentinel node
    list->head->data = NULL; // Sentinel node stores no data
    list->head->next = list->head; 
    list->head->prev = list->head; 

    return list;
}

/**
 * Destroy (free) the list and all associated data. This function will call
 * destroy_data on each nodes data element.
 *
 * @param list a pointer to the list that needs to be destroyed
 */
void list_destroy(list_t **list) {
    // Check if the list is NULL or if the pointer to the list is NULL
    if (list == NULL || *list == NULL) return;

    // Free the memory allocated for the data in each node
    node_t *curr = (*list)->head->next;
    while (curr != (*list)->head) {
        (*list)->destroy_data(curr->data);  // Call the destroy_data function pointer to free the memory allocated for the data
        node_t *next = curr->next;          // Store the memory address of the next node in curr
        free(curr);                         // Free the memory allocated for the current node
        curr = next;                        // Move to the next node in the list
    }

    // Free the allocated memory for the list and node then set the list pointer to NULL
    free((*list)->head); 
    free(*list); 
    *list = NULL; 
}

/**
 * Adds data to the front of the list
 *
 * @param list a pointer to an existing list.
 * @param data the data to add
 * @return A pointer to the list
 */
list_t *list_add(list_t *list, void *data) {
    // Check if the list is NULL or if the data is NULL
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return list;
    }

    if (data == NULL) {
        fprintf(stderr, "Error: Data is NULL\n");
        return list;
    }

    // Create a new node to store the data
    node_t *new_node = malloc(sizeof(node_t));
    // Check if the memory allocation was successful
    if (new_node == NULL) {
        fprintf(stderr, "Error: New node memory allocation failed\n");
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

/**
 * Removes the data at the specified index.
 *
 * @param list The list to remove the element from
 * @param index The index
 * @return void* The data that was removed or NULL if nothing was removed
 */
void *list_remove_index(list_t *list, size_t index) {
    // Check if the list is NULL or if the index is out of bounds
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return NULL;
    }

    if (index >= list->size) {
        fprintf(stderr, "Error: Index out of bounds\n");
        return NULL;
    }

    // Find the node at the specified index
    node_t *curr = list->head->next;
    for (size_t i = 0; i < index; i++) {
        curr = curr->next;
    }

    // Store the data to be removed
    void *data = curr->data;

    // Update the pointers of adjacent nodes
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;

    // Free the memory allocated for the node
    free(curr);

    // Decrement the size of the list
    list->size--;

    return data;
}

/**
 * Search for any occurrence of data from the list.
 *
 * @param list the list to search for data
 * @param data the data to look for
 * @return The index of the item if found or -1 if not
 */
int list_indexof(list_t *list, void *data) {
    // Check if the list is NULL or if the data is NULL
    if (list == NULL) {
        fprintf(stderr, "Error: List is NULL\n");
        return -1;
    }

    if (data == NULL) {
        fprintf(stderr, "Error: Data is NULL\n");
        return -1;
    }

    // Find the index of the data in the list
    node_t *curr = list->head->next;
    size_t index = 0;
    while (curr != list->head) {
        // Compare the data in the current node with the specified data
        if (list->compare_to && list->compare_to(curr->data, data) == 0) {
            return index;
        }
        curr = curr->next;
        index++;
    }

    // Data not found in the list
    fprintf(stderr, "Error: Data not found in the list\n");
    return -1;
}