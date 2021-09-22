/*
 * CPUPool.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#ifndef ELEMENTS_CPUPOOL_H_
#define ELEMENTS_CPUPOOL_H_

#include "Pool.h"

namespace DC {

class CPUPool: public Pool {
public:
	CPUPool();
	CPUPool(unsigned long long tc, unsigned long long ms, unsigned long long mc);
	~CPUPool();
protected:
	void print(std::ostream& out) override;
	void printCSV(std::ofstream& file) override;

};

} /* namespace DC */

#endif /* ELEMENTS_CPUPOOL_H_ */
