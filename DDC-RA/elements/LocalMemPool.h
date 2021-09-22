/*
 * LocalMemPool.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#ifndef ELEMENTS_LOCALMEMPOOL_H_
#define ELEMENTS_LOCALMEMPOOL_H_

#include "Pool.h"

namespace DC {

class LocalMemPool: public Pool {
public:
	LocalMemPool();
	LocalMemPool(unsigned long long ms, unsigned long long mc);
	~LocalMemPool();
protected:
	void print(std::ostream& out) override;
	void printCSV(std::ofstream& file) override;

};

} /* namespace DC */

#endif /* ELEMENTS_LOCALMEMPOOL_H_ */
