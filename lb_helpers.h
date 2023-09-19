/* Copyright 2023 < Matei Mantu > */
#ifndef LB_HELPERS_H_
#define LB_HELPERS_H_

#include "load_balancer.h"

/**
 * @binary_search_server - looks for where the key_hash should be in the
 *                         sorted array of servers. It works with both
 *                         key hashes and server hashes.
 * 
 * @balancer - pointer to the load balancer
 * @key_hash - the hash of the key that we're searching
 * @found - 1 if the hash is in the balancer, 0 if not
 * 
 * Return: the position where key_hash should be in the array
 */
int binary_search_server(load_balancer *balancer, unsigned int key_hash,
                         int *found);

/**
 * @insert_server - inserts a new server into the load balancer
 * 
 * @balancer - pointer to the load balancer
 * @server_id - id of the inserted server
 * @pos - the position where to insert
 */
void insert_server(load_balancer *balancer, unsigned int server_id, int pos);

/**
 * @delete_server - delets a server from the load balancer
 * 
 * @balancer - pointer to the load balancer
 * @pos - position of the deleted server
 */
void delete_server(load_balancer *balancer, int pos);

#endif /* LB_HELPERS_H_ */
