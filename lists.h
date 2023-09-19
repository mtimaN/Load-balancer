/* Copyright 2023 < Matei Mantu > */
#ifndef LISTS_H_
#define LISTS_H_

#define MAX_STRING_SIZE	256
/**
 * struct ll_node_t - simply linked list node
 * 
 * @data - pointer to the data inside node
 * @next - pointer to the next node in the list
 */
typedef struct ll_node_t
{
	void* data;
	struct ll_node_t* next;
} ll_node_t;

/**
 * linked_list_t - simply linked list
 * 
 * @head - the list's head
 * @data_size - the data size of a node inside the list
 * @size - the number of nodes
 */
typedef struct linked_list_t
{
	ll_node_t* head;
	unsigned int data_size;
	unsigned int size;
} linked_list_t;

/**
 * ll_create - Creates a new linked list
 * 
 * @data_size - the data size of a node inside the list
 * 
 * Return: linked_list_t* pointer to the newly created list
 */
linked_list_t *ll_create(unsigned int data_size);

/**
 * ll_add_nth_node - adds a new node to a list
 * 
 * @list - pointer to the list
 * @n - position where the new node will be added
 * @new_data - the data inside the new node
 */
void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data);

/**
 * @ll_remove_nth_node - removes a node from the list
 * 
 * @list - pointer to the list
 * @n - position of the removed node
 * 
 * Return: ll_node_t* pointer to the removed node
 */
ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n);

/**
 * @ll_get_size - Returns the size of a list 
 * 
 * @list - pointer to the list
 * 
 * Return: list's size 
 */
unsigned int ll_get_size(linked_list_t* list);

/**
 * @ll_free - deletes the list and frees memory
 * 
 * @pp_list - address of the list's pointer
 */
void ll_free(linked_list_t** pp_list);

#endif /* LISTS_H_ */
