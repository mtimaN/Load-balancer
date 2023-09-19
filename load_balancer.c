/* Copyright 2023 < Matei Mantu > */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "load_balancer.h"
#include "lb_helpers.h"
#include "server.h"
#include "lists.h"
#include "utils.h"

#define MAX_SERVERS 100000

unsigned int hash_function_servers(void *a) {
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

load_balancer *init_load_balancer() {
	load_balancer *balancer = malloc(sizeof(load_balancer));
	DIE(!balancer, M_ERR);
	balancer->servers = NULL;
	balancer->size = 0;
	return balancer;
}

void loader_add_server(load_balancer *balancer, int server_id) {
	if (balancer->size == 0)
		balancer->servers = malloc(sizeof(server_memory) * 3);
	else
		balancer->servers =
		realloc(balancer->servers, (balancer->size + 3) * sizeof(server_memory));
	DIE(!balancer->servers, M_ERR);
	/* The server will have 3 replicas on the hash ring */
	for (int i = 0; i < 3; ++i) {
		int collision;
		unsigned int new_hash = hash_function_servers(&server_id);
		int pos = binary_search_server(balancer, new_hash, &collision);
		/**
		 * In case two servers have the same hash, the one with the smaller
		 * hash will be the first in the array
		 */
		if (collision)
			if (server_id > balancer->servers[pos]->id)
				pos++;
		insert_server(balancer, server_id, pos);

		/* Shifting server_id to the next replica */
		server_id += MAX_SERVERS;
		/* Distributing load to the new server */
		server_memory *next_server = balancer->servers[(pos + 1) % balancer->size];
		server_memory *new = balancer->servers[pos];

		if (!next_server || balancer->size == 1)
			continue;

		for (unsigned int j = 0; j < next_server->hmax; ++j) {
			ll_node_t *first = next_server->buckets[j]->head;
			while (first) {
				server_item *tmp_item = (server_item*)first->data;
				first = first->next;
				unsigned int key_hash =
				hash_function_key(tmp_item->key);
				/* Adding the server on the last position */
				if (pos + 1 == balancer->size &&
				key_hash > hash_function_servers(&balancer->servers[0]->id) &&
				key_hash < hash_function_servers(&balancer->servers[pos]->id)) {
					server_store(new, tmp_item->key, tmp_item->value);
					server_remove(next_server, tmp_item->key);
				/* Common case */
				} else if (key_hash <
				hash_function_servers(&balancer->servers[pos]->id) &&
				pos + 1 != balancer->size) {
					server_store(new, tmp_item->key, tmp_item->value);
					server_remove(next_server, tmp_item->key);
				/* Adding the server on the first position */
				} else if (pos == 0 && key_hash >
						   hash_function_servers(&balancer->servers[1]->id)) {
					server_store(new, tmp_item->key, tmp_item->value);
					server_remove(next_server, tmp_item->key);
				}
			}
		}
	}
}

void loader_remove_server(load_balancer *balancer, int server_id) {
	for (int i = 0; i < 3; ++i) {
		int found;
		unsigned int new_hash = hash_function_servers(&server_id);
		int pos = binary_search_server(balancer, new_hash, &found) % balancer->size;
		if (!found) {
			printf("Server %d not found!\n", server_id);
			return;
		}
		/* shifting server_id to the next replica */
		server_id += MAX_SERVERS;
		/* Distributing load to other servers */
		server_memory *next_server;
		server_memory *old_server = balancer->servers[pos];
		next_server = balancer->servers[(pos + 1) % balancer->size];
		for (unsigned int j = 0; j < old_server->hmax; ++j) {
			ll_node_t *first = old_server->buckets[j]->head;
			while (first) {
				server_item *tmp_item = (server_item*)first->data;
				first = first->next;
				server_store(next_server, tmp_item->key, tmp_item->value);
				server_remove(old_server, tmp_item->key);
			}
		}
		delete_server(balancer, pos);
	}
	balancer->servers = realloc(balancer->servers,
							   (balancer->size) * sizeof(server_memory));
	DIE(!balancer->servers, M_ERR);
}

void loader_store(load_balancer *balancer, char *key, char *value,
				  int *server_id) {
	int found;
	unsigned int key_hash = hash_function_key(key);
	int pos = binary_search_server(balancer, key_hash, &found) % balancer->size;
	if (pos < 0)
		pos += balancer->size;
	*server_id = balancer->servers[pos]->id % MAX_SERVERS;
	server_store(balancer->servers[pos], key, value);
}

char *loader_retrieve(load_balancer *balancer, char *key, int *server_id) {
	int found;
	unsigned int key_hash = hash_function_key(key);
	int pos = binary_search_server(balancer, key_hash, &found) % balancer->size;
	*server_id = balancer->servers[pos]->id % MAX_SERVERS;
	return server_retrieve(balancer->servers[pos], key);
}

void free_load_balancer(load_balancer *balancer) {
	while (balancer->size > 0) {
		balancer->servers[0]->size = 0;
		delete_server(balancer, 0);
	}
	free(balancer->servers);
	free(balancer);
}
