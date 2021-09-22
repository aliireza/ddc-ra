/*
 * LocalMemPool.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#include "LocalMemPool.h"

namespace DC {

LocalMemPool::LocalMemPool()
: Pool() {
}

LocalMemPool::LocalMemPool(unsigned long long ms, unsigned long long mc)
: Pool(0,ms,mc,LOCALMEM) {
}

LocalMemPool::~LocalMemPool() {
}

void LocalMemPool::print(std::ostream &out) {
	out << "======" << std::endl;
	out << "Local Memory Pool: " << getID() << std::endl;
	out << getTotalMemSize() << " MB" << std::endl;
	out << "Memory Access Cost: " << getMemCost() << std::endl;
}

void LocalMemPool::printCSV(std::ofstream &file) {
	/* Header of data file is 
	* "ID, Type, Total_Memory, Used_Memory, Memory_Cost, Total_CPU, Used_CPU
	* header can be changed in DataCenter::printPoolsCSV
	*/
	file << getID() << "," 
	//<< getType() << "," 
	<< "LocalMemPool" << "," 
	<< getTotalMemSize() << "," 
	<< getUsedMemSize() << "," 
	<< getMemCost() << "," 
	<< getTotalCores() << "," 
	<< getUsedCores() << 
	"\n";

}

} /* namespace DC */
