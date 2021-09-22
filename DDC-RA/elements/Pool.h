/*
 * Pool.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#ifndef ELEMENTS_POOL_H_
#define ELEMENTS_POOL_H_

#include "Constants.h"

namespace DC {

class Pool {
public:
	Pool();
	Pool(unsigned long long tc, unsigned long long ms, unsigned long long mc, Type t);
	virtual ~Pool();

	unsigned long long getTotalMemSize(void) const;
	unsigned long long getUsedMemSize(void);
	unsigned long long getAvailMemSize(void);
	unsigned long long getMemCost(void) const;
	unsigned long long getTotalCores(void) const;
	unsigned long long getUsedCores(void);
	unsigned long long getAvailCores(void);
	unsigned int getID(void) const;
	Type getType(void) const;
	// int assignMemory(unsigned long long m);
	// int freeMemory(unsigned long long m);
	// int assignCores(unsigned long long c);
	// int freeCores(unsigned long long c);
	friend std::ostream& operator << (std::ostream &out, Pool& c);
	/* For CSV printing */
	friend std::ofstream& operator << (std::ofstream &file, Pool& c);

	unsigned long long getPoolCount(void);


private:
	//TODO: define price
	const unsigned int pool_id;
	const Type pool_type;
	const unsigned long long total_memory_size;
	unsigned long long used_memory_size;
	const unsigned long long memory_cost;
	const unsigned long long total_cores;
	unsigned long long used_cores;
	static unsigned long long pool_count;
protected:
	virtual void print(std::ostream& out) = 0;
	virtual void printCSV(std::ofstream& file) = 0;

};

inline std::ostream& operator << (std::ostream &out, Pool& c) {
	c.print(out);
	return out;
}
inline std::ofstream& operator << (std::ofstream& file, Pool& c) {
	c.printCSV(file);
	return file;
}

} /* namespace DC */

#endif /* ELEMENTS_POOL_H_ */
