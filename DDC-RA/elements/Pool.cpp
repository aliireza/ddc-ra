/*
 * Pool.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#include "Pool.h"
#include "Cluster.h"

namespace DC {

unsigned long long Pool::pool_count=1;

Pool::Pool()
: pool_id(pool_count), pool_type(NONE), total_memory_size(0), memory_cost(0), total_cores(0) {
	used_memory_size=0;
	used_cores=0;
	Pool::pool_count++;
}

Pool::~Pool() {
	Pool::pool_count--;
}

Pool::Pool(unsigned long long tc, unsigned long long ms, unsigned long long mc, Type t)
: pool_id(pool_count), pool_type(t), total_memory_size(ms), memory_cost(mc), total_cores(tc) {
	used_memory_size=0;
	used_cores=0;
	Pool::pool_count++;
}

unsigned long long Pool::getTotalMemSize(void) const {
	return total_memory_size;
}

unsigned long long Pool::getUsedMemSize(void) {
	return used_memory_size;
}

unsigned long long Pool::getAvailMemSize(void) {
	return (total_memory_size-used_memory_size);
}

unsigned long long Pool::getMemCost(void) const {
	return memory_cost;
}

unsigned long long Pool::getTotalCores(void) const {
	return total_cores;
}

unsigned long long Pool::getUsedCores(void) {
	return used_cores;
}

unsigned long long Pool::getAvailCores(void) {
	return (total_cores-used_cores);
}


unsigned int Pool::getID(void) const {
	return pool_id;
}

Type DC::Pool::getType(void) const {
	return pool_type;
}

// int Pool::assignMemory(unsigned long long m) {
// 	unsigned long long tmp = used_memory_size + m;
// 	if(tmp<=total_memory_size) {
// 		used_memory_size=tmp;
// 		return SUCCESS;
// 	} else {
// 		return FAILURE;
// 	}
// }
// int Pool::freeMemory(unsigned long long m) {
// 	unsigned long long tmp = used_memory_size - m;
// 	if(tmp>=0) {
// 		used_memory_size=tmp;
// 		return SUCCESS;
// 	} else {
// 		return FAILURE;
// 	}
// }


// int Pool::assignCores(unsigned long long c){
// 	unsigned long long tmp = used_cores + c;
// 	if(tmp<=total_cores) {
// 		used_cores=tmp;
// 		return SUCCESS;
// 	} else {
// 		return FAILURE;
// 	}
// }

// int Pool::freeCores(unsigned long long c){
// 	unsigned long long tmp = used_cores -c;
// 	if(tmp>=0) {
// 		used_cores=tmp;
// 		return SUCCESS;
// 	} else {
// 		return FAILURE;
// 	}
// }

unsigned long long Pool::getPoolCount(void) {
	return pool_count;
}

} /* namespace DC */
