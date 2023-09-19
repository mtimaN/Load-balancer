/* Copyright 2023 < Matei Mantu > */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "load_balancer.h"
#include "server.h"

int binary_search_server(load_balancer *balancer, unsigned int key_hash,
						 int *found)
{
	*found = 0;
	int left = 0, right = balancer->size - 1;
	int mid = (left + right) / 2;
		while (left <= right) {
			if (hash_function_servers(&balancer->servers[mid]->id) == key_hash) {
				*found = 1;
				return mid;
			}
			if (key_hash > hash_function_servers(&balancer->servers[mid]->id))
				left = mid + 1;
			else
				right = mid - 1;
			mid = (left + right) / 2;
		}
	return (right + 1);
}

void insert_server(load_balancer *balancer, unsigned int server_id, int pos)
{
	for (int i = balancer->size - 1; i >= pos; --i) {
		balancer->servers[i + 1] = balancer->servers[i];
	}
	balancer->servers[pos] = init_server_memory();
	balancer->servers[pos]->id = server_id;
	balancer->size++;
}

void delete_server(load_balancer *balancer, int pos)
{
	if (balancer->servers[pos]->size > 0) {
		printf("Error - deleting server with size %d!\n",
			   balancer->servers[pos]->size);
	}
	free_server_memory(balancer->servers[pos]);
	for (int i = pos; i < balancer->size - 1; ++i) {
		balancer->servers[i] = balancer->servers[i + 1];
	}
	balancer->size--;
}
