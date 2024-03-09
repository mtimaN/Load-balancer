**Nume: Mantu Matei-Cristian**
**Grupă: 312CA**

## Load Balancer - Tema 2

### Description:

This program implements a load balancer in C, using consistent hashing.
Consistent hashing implies using a hash ring - an imaginary ring where the hash of every server and item is placed. By traversing the ring clock-wise, every item is assigned to the server closest to them. In actuality, the servers are stored in an array, a choice which facilitates the search of the items by performing binary search for finding the server where every item is/should be stored. With every server added, items from the server next to it will be checked whether they should be moved over to the new one. When a server is deleted, every item is moved to the next server on the hashring.

For implementing the functions handling individual server management, I have minimally modified the functions from the hashtable lab. That's why I won't explain them here.
### Resources used:

I checked the links from the OCW page:
https://www.toptal.com/big-data/consistent-hashing
https://www.youtube.com/watch?v=zaRkONvyGr8&ab_channel=GauravSe

But the thing that helped me solve bugs and understand everything better was [the vizualization of test 1](https://docs.google.com/presentation/d/1gdy9Al3KtNzUL2a42d9lbDxnlUfn4P0FUX62FR9OPcs/).
