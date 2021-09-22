/*
 * CPUPool.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#include "CPUPool.h"

namespace DC {

CPUPool::CPUPool()
: Pool() {
}

CPUPool::CPUPool(unsigned long long tc,
		unsigned long long ms, unsigned long long mc)
: Pool(tc, ms, mc, CPU) {
}

CPUPool::~CPUPool() {
}

void CPUPool::print(std::ostream &out) {
	out << "======" << std::endl;
	out << "CPU Pool: " << getID() << std::endl;
	out << getTotalCores() << " cores" << std::endl;
	out << getTotalMemSize() << " MB" << std::endl;
	out << "Memory Access Cost: " << getMemCost() << std::endl;
}

void CPUPool::printCSV(std::ofstream &file) {

	/* Header of data file is 
	* "ID, Type, Total_Memory, Used_Memory, Memory_Cost, Total_CPU, Used_CPU
	* header can be changed in DataCenter::printPoolsCSV
	*/
	file << getID() << "," 
	//<< getType() << "," 
	<< "CPUPool" << "," 
	<< getTotalMemSize() << "," 
	<< getUsedMemSize() << "," 
	<< getMemCost() << "," 
	<< getTotalCores() << "," 
	<< getUsedCores() << "\n";
}

} /* namespace DC */
