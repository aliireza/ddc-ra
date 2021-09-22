/*
 * RemoteMemPool.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#ifndef ELEMENTS_REMOTEMEMPOOL_H_
#define ELEMENTS_REMOTEMEMPOOL_H_

#include "Pool.h"

namespace DC {

class RemoteMemPool: public Pool {
public:
	RemoteMemPool();
	RemoteMemPool(unsigned long long ms, unsigned long long mc);
	~RemoteMemPool();
protected:
	void print(std::ostream& out) override;
	void printCSV(std::ofstream& file) override;

};

} /* namespace DC */

#endif /* ELEMENTS_REMOTEMEMPOOL_H_ */
