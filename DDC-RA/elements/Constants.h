/*
 * Constants.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */


#ifndef ELEMENTS_CONSTANTS_H_
#define ELEMENTS_CONSTANTS_H_

#include <list>
#include <iostream>
#include <stdlib.h>
#include <tuple>
#include <fstream>

namespace DC {

#define SUCCESS 1
#define FAILURE -1

#define LOCAL_MEM_PRICE 100
#define LOCAL_REMOTE_MEM_PRICE 50
#define REMOTE_REMOTE_MEM_PRICE 25



enum Status {OFF=0, ON=1};

enum Type {NONE=0, CPU=1, LOCALMEM=2, REMOTEMEM=3};

struct Solution
{
	unsigned int cores;
	unsigned long long local_memory;
	unsigned long long local_remote_memory;
	unsigned long long remote_remote_memory;
	unsigned long long price;
	unsigned long long cost;
	bool solved;
	Solution()
	: cores(0),
	  local_memory(0),
	  local_remote_memory(0),
	  remote_remote_memory(0),
	  price(0),
	  cost(0),
	  solved(false) {}
	Solution(unsigned int c, unsigned long long lm, unsigned long long lrm, unsigned long long rrm, unsigned long long p, unsigned long long cst, bool s)
	: cores(c),
	  local_memory(lm),
	  local_remote_memory(lrm),
	  remote_remote_memory(rrm),
	  price(p),
	  cost(cst),
	  solved(s) {}
	inline Solution operator=(Solution s) {
	        cores=s.cores;
	        local_memory=s.local_memory;
	        local_remote_memory=s.local_remote_memory;
	        remote_remote_memory=s.remote_remote_memory;
	        price=s.price;
	        cost=s.cost;
	        solved=s.solved;
	        return s;
	}
};

} /* namespace DC */


#endif /* ELEMENTS_CONSTANTS_H_ */
