/* Copyright 2023 < Matei Mantu > */
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "utils.h"
#include "lists.h"

#define MAX_BUCKETS 10

unsigned int hash_function_key(void *a)
{
	unsigned char *puchar_a = (unsigned char *)a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

server_memory *init_server_memory()
{
	server_memory *server = malloc(sizeof(server_memory));
	DIE(!server, M_ERR);
	server->size = 0;
	server->hmax = MAX_BUCKETS;
	server->buckets = (linked_list_t **)
					  malloc(server->hmax * sizeof(linked_list_t));
	DIE(!server->buckets, M_ERR);
	for (unsigned int i = 0; i < server->hmax; ++i) {
		server->buckets[i] = ll_create(sizeof(server_item));
	}
	return server;
}

void server_store(server_memory *server, char *key, char *value) {
	if (!server) {
		printf("Invalid server!\n");
		return;
	}
	int index =  hash_function_key(key) % server->hmax;
	ll_node_t *first = server->buckets[index]->head;
	while (first) {
		if (strcmp(((server_item*)first->data)->key, key) == 0) {
			/* Freeing up the old memory */
			free(((server_item*)first->data)->value);
			/* Allocating memory for the new value */
			((server_item*)first->data)->value = malloc(strlen(value) + 1);
			DIE(!((server_item*)first->data)->value, M_ERR);
			strcpy(((server_item*)first->data)->value, value);
			return;
		}
		first = first->next;
	}
	if (!first) {
		server_item new_info;
		new_info.value = malloc(strlen(value) + 1);
		DIE(!new_info.value, M_ERR);
		new_info.key = malloc(strlen(key) + 1);
		DIE(!new_info.key, M_ERR);
		strcpy(new_info.value, value);
		strcpy(new_info.key, key);
		ll_add_nth_node(server->buckets[index], 0, &new_info);
		server->size++;
	}
}

char *server_retrieve(server_memory *server, char *key) {
	if (!server) {
		printf("Error finding server!");
		return NULL;
	}

	int index = hash_function_key(key) % server->hmax;
	ll_node_t *first = server->buckets[index]->head;
	while (first) {
		if (strcmp(((server_item*)first->data)->key, key) == 0) {
			return ((server_item*)first->data)->value;
		}
		first = first->next;
	}
	return NULL;
}

void server_remove(server_memory *server, char *key) {
	if (!server) {
		return;
	}

	int index = hash_function_key(key) % server->hmax;
	ll_node_t* first = server->buckets[index]->head;

	int i = 0;

	while (first) {
		if (strcmp(((server_item*)first->data)->key, key) == 0) {
			free(((server_item*)first->data)->key);
			free(((server_item*)first->data)->value);
			ll_node_t *tmp = ll_remove_nth_node(server->buckets[index], i);
			free(tmp->data);
			free(tmp);
			server->size--;
			return;
		}
		i++;
		first = first->next;
	}
}

void free_server_memory(server_memory *server) {
	if (!server)
		return;
	for (unsigned int i = 0; i < server->hmax; ++i) {
		ll_node_t *first = server->buckets[i]->head;
		while (first) {
			ll_node_t *tmp = first;
			first = first->next;
			free(((server_item*)tmp->data)->key);
			free(((server_item*)tmp->data)->value);
			free(tmp->data);
			free(tmp);
		}
		free(server->buckets[i]);
	}
	free(server->buckets);
	free(server);
}
