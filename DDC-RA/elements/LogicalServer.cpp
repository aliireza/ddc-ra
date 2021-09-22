/*
 * LogicalServer.cpp
 *
 *  Created on: Aug 12, 2019
 *      Author: alireza
 */

#include "LogicalServer.h"

namespace DC {

unsigned long long LogicalServer::logical_server_count=1;

LogicalServer::LogicalServer()
: logical_server_id(logical_server_count), n_cpus(0), s_mem(0), required_local_SLA(0), required_remote_SLA(0) {
	local_memory=0;
	local_remote_memory=0;
	remote_remote_memory=0;
	cost=0;
	price=0;
	logical_server_count++;
}

LogicalServer::LogicalServer(unsigned long long nc,
		unsigned long long sm, unsigned long long rls, unsigned long long rrs)
: logical_server_id(logical_server_count), n_cpus(nc), s_mem(sm), required_local_SLA(rls), required_remote_SLA(rrs){
	local_memory=0;
	local_remote_memory=0;
	remote_remote_memory=0;
	cost=0;
	price=0;
	logical_server_count++;
	if(rls+rrs<sm) {
		std::cout << "Wrong SLA for logical server " << logical_server_count - 1 << "!" << std::endl;
		// std::cout << "Total Memory: " << sm << " Local SLA: " << rls << " Remote SLA: " << rrs << std::endl;
		// std::cout << rls+rrs << std::endl;
		exit(EXIT_FAILURE);
	}
}

LogicalServer::~LogicalServer() {

}

unsigned int LogicalServer::getID(void) {
	return logical_server_id;
}

unsigned long long LogicalServer::getCPUNumber(void) {
	return n_cpus;
}

unsigned long long LogicalServer::getMemSize(void) {
	return s_mem;
}

unsigned long long LogicalServer::getRequiredLocalSLA(void) {
	return required_local_SLA;
}

unsigned long long LogicalServer::getRequiredRemoteSLA(void) {
	return required_remote_SLA;
}


unsigned long long LogicalServer::getLocalMemSize(void) {
	return local_memory;
}

unsigned long long LogicalServer::getLocalRemoteMemSize(void){
	return local_remote_memory;
}

unsigned long long LogicalServer::getRemoteRemoteMemSize(void){
	return remote_remote_memory;
}

unsigned long long LogicalServer::getCost(void){
	return cost;
}

unsigned long long LogicalServer::getPrice(void){
	return price;
}

int LogicalServer::setMemory(unsigned long long lm, unsigned long long lrm, unsigned long long rrm) {
	if(lm+lrm+rrm==s_mem) {
		local_memory=lm;
		local_remote_memory=lrm;
		remote_remote_memory=rrm;
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

/* This uses the values defined in constant.h*/
int LogicalServer::setPriceAndCost(unsigned long long p, unsigned long long c){
	if(p >= 0 && c>=0) {
		price = p;
		cost = c;
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

int LogicalServer::allocate(Solution s) {
	if(setMemory(s.local_memory,s.local_remote_memory,s.remote_remote_memory)) {
		if(setPriceAndCost(s.price,s.cost)) {
			return SUCCESS;
		}
		else {
			return FAILURE;
		}
	} else {
		return FAILURE;
	}
}


void LogicalServer::print(std::ostream &out) {
	out << "======" << std::endl;
	out << "Logical Server: " << getID() << std::endl;
	out << getCPUNumber() << " cores" << std::endl;
	out << getMemSize() << " MB" << std::endl;
	out << "Minimum Local Memory (Local SLA): " << getRequiredLocalSLA() << " MB " << std::endl;
	out << "Maximum Weighted Remote Memory (Remote SLA): " << getRequiredRemoteSLA() << " MB " << std::endl;
	out << "Cost" << "" << std::endl; // Add function to calculate the Cost of Logical Server
}

void LogicalServer::printCSV(std::ofstream &file) {

/* The header of CSV file. If you you make a change 
	be sure to apply the chanages to DataCenter::printLogicalServersCSV
	id,
	n_cpus,
	total_mem,
	required_local_SLA
	required_remote_SLA
	local_memory,
	local_remote_memory,
	remote_remote_memory,
	cost
*/
	file << getID() << "," 
	<< getCPUNumber() << ","
	<< getMemSize() << ","
	<< getRequiredLocalSLA() << ","  /* Minimum required local memory */
	<< getRequiredRemoteSLA() << "," 
	<< getLocalMemSize() << "," 
	<< getLocalRemoteMemSize() << "," 
	<< getRemoteRemoteMemSize() << "," 
	<< getPrice() << ","
	<< getCost() << ","
	<< ((getLocalMemSize() >= getRequiredLocalSLA()) ? 0 : 1) << ","
	<< ((getCost() <= getRequiredRemoteSLA()) ? 0 : 1) << "\n"; 
}


} /* namespace DC */
