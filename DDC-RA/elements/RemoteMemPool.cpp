/*
 * RemoteMemPool.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#include "RemoteMemPool.h"

namespace DC {

RemoteMemPool::RemoteMemPool()
: Pool() {
}

RemoteMemPool::RemoteMemPool(unsigned long long ms, unsigned long long mc)
: Pool(0,ms,mc,REMOTEMEM) {
}

RemoteMemPool::~RemoteMemPool() {
}

void RemoteMemPool::print(std::ostream &out) {
	out << "======" << std::endl;
	out << "Remote Memory Pool: " << getID() << std::endl;
	out << getTotalMemSize() << " MB" << std::endl;
	out << "Memory Access Cost: " << getMemCost() << std::endl;
}

void RemoteMemPool::printCSV(std::ofstream &file) {
	/* Header of data file is 
	* "ID, Type, Total_Memory, Used_Memory, Memory_Cost, Total_CPU, Used_CPU
	* header can be changed in DataCenter::printPoolsCSV
	*/
	file << getID() << "," 
	//<< getType() << "," 
	<< "RemoteMemPool" << "," 
	<< getTotalMemSize() << "," 
	<< getUsedMemSize() << "," 
	<< getMemCost() << "," 
	<< getTotalCores() << "," 
	<< getUsedCores() << 
	"\n";
}


} /* namespace DC */
