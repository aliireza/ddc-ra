/*
 * Cluster.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#include "Cluster.h"

namespace DC {

unsigned long long Cluster::cluster_count=1;

Cluster::Cluster()
: cluster_id(cluster_count){
	cluster_status=OFF;
	cluster_utilization=0;
	total_cpu=0;
	used_cpu=0;
	total_local_memory=0;
	used_local_memory=0;
	local_memory_cost=0;
	total_local_remote_memory=0;
	used_local_remote_memory=0;
	local_remote_memory_cost=0;
	total_remote_remote_memory=0;
	used_remote_remote_memory=0;
	remote_remote_memory_cost=0;
	cluster_count++;
}

Cluster::~Cluster() {
}

unsigned int Cluster::getUtilization(void) {
	cluster_utilization = getCPUUtilization()/2 + getMemUtilization()/2;
	return cluster_utilization;
}

unsigned int Cluster::getID(void) const {
	return cluster_id;
}

unsigned long long Cluster::getTotalCPU(void) {
	return total_cpu;
}

unsigned long long Cluster::getUsedCPU(void) {
	return used_cpu;
}

unsigned long long Cluster::getAvailCPU(void) {
	return (getTotalCPU()-getUsedCPU());
}

unsigned long long Cluster::getTotalMem(void) {
	return ( getTotalLocalMem() + getTotalRemoteMem());
}

unsigned long long Cluster::getUsedMem(void) {
	return ( getUsedLocalMem() + getUsedRemoteMem());
}

unsigned long long Cluster::getAvailMem(void) {
	return (getTotalMem()-getUsedMem());
}

unsigned long long Cluster::getTotalLocalMem(void) {
	return total_local_memory;
}

unsigned long long Cluster::getUsedLocalMem(void) {
	return used_local_memory;
}

unsigned long long Cluster::getAvailLocalMem(void) {
	return (getTotalLocalMem()-getUsedLocalMem());
}

unsigned long long Cluster::getTotalLocalRemoteMem(void) {
	return total_local_remote_memory;
}

unsigned long long Cluster::getUsedLocalRemoteMem(void) {
	return used_local_remote_memory;
}

unsigned long long Cluster::getAvailLocalRemoteMem(void) {
	return (getTotalLocalRemoteMem()-getUsedLocalRemoteMem());
}

unsigned long long Cluster::getTotalRemoteRemoteMem(void) {
	return total_remote_remote_memory;
}

unsigned long long Cluster::getUsedRemoteRemoteMem(void) {
	return used_remote_remote_memory;
}

unsigned long long Cluster::getAvailRemoteRemoteMem(void) {
	return (getTotalRemoteRemoteMem()-getUsedRemoteRemoteMem());
}

unsigned long long Cluster::getTotalRemoteMem(void) {
	return ( getTotalRemoteRemoteMem() + getTotalLocalRemoteMem());
}

unsigned long long Cluster::getUsedRemoteMem(void) {
	return ( getUsedRemoteRemoteMem() + getUsedLocalRemoteMem());
}

unsigned long long Cluster::getAvailRemoteMem(void) {
	return (getTotalRemoteMem()-getUsedRemoteMem());
}

unsigned long long Cluster::getLocalMemCost(void) {
	return local_memory_cost;
}

unsigned long long Cluster::getLocalRemoteMemCost(void) {
	return local_remote_memory_cost;
}

unsigned long long Cluster::getRemoteRemoteMemCost(void) {
	return remote_remote_memory_cost;
}

void Cluster::addPool(Pool *p) {
	if(p->getType() == CPU) {
		CPUPool* c = dynamic_cast<CPUPool*>(p);
		addCPUPool(c);
	} else if (p->getType() == LOCALMEM) {
		LocalMemPool* m = dynamic_cast<LocalMemPool*>(p);
		addLocalMemPool(m);
	} else if (p->getType() == REMOTEMEM) {
		RemoteMemPool* m = dynamic_cast<RemoteMemPool*>(p);
		addRemoteMemPool(m);
	}
}


void Cluster::removePool(Pool *p) {
	if(p->getType() == CPU) {
		CPUPool* c = dynamic_cast<CPUPool*>(p);
		removeCPUPool(c);
	} else if (p->getType() == LOCALMEM) {
		LocalMemPool* m = dynamic_cast<LocalMemPool*>(p);
		removeLocalMemPool(m);
	} else if (p->getType() == REMOTEMEM) {
		RemoteMemPool* m = dynamic_cast<RemoteMemPool*>(p);
		removeRemoteMemPool(m);
	}
}

std::list<Pool*> Cluster::getPools(void) {
	return pool_list;
}

std::list<CPUPool*> Cluster::getCPUPools(void) {
	std::list<CPUPool*> current_CPUPools;
	for (auto itr = pool_list.begin();
	         itr != pool_list.end(); itr++) {
		Pool *current_pool = (*itr);
		if(current_pool->getType()==CPU){
			CPUPool* c = dynamic_cast<CPUPool*>(current_pool);
			if(c!=nullptr)
				current_CPUPools.push_back(c);
		}
	}
	return current_CPUPools;
}

std::list<LocalMemPool*> Cluster::getLocalMemPools(void) {
	std::list<LocalMemPool*> current_LocalMemPools;
	for (auto itr = pool_list.begin();
	         itr != pool_list.end(); itr++) {
		Pool *current_pool = (*itr);
		if(current_pool->getType()==LOCALMEM){
			LocalMemPool* m = dynamic_cast<LocalMemPool*>(current_pool);
			if(m!=nullptr)
				current_LocalMemPools.push_back(m);
		}
	}
	return current_LocalMemPools;
}

std::list<RemoteMemPool*> Cluster::getRemoteMemPools(void) {
	std::list<RemoteMemPool*> current_RemoteMemPools;
	for (auto itr = pool_list.begin();
	         itr != pool_list.end(); itr++) {
		Pool* current_pool = (*itr);
		if(current_pool->getType()==REMOTEMEM){
			RemoteMemPool* m = dynamic_cast<RemoteMemPool*>(current_pool);
			if(m!=nullptr)
				current_RemoteMemPools.push_back(m);
		}
	}
	return current_RemoteMemPools;
}

void Cluster::setClusterStatus(Status s) {
	cluster_status=s;
}

void Cluster::updateClusterStatus(void) {
	if(getUsedCPU() >0 || getUsedMem()>0) {
		setClusterStatus(ON);
	} else {
		setClusterStatus(OFF);
	}
}

void Cluster::addCPUPool(CPUPool* c) {
	if (c!=nullptr) {
		pool_list.push_back(c);
		total_cpu  += c->getTotalCores();
		used_cpu += c->getUsedCores();
		total_local_memory += c->getTotalMemSize();
		used_local_memory += c->getUsedMemSize();
		local_memory_cost = c->getMemCost(); /* Assumed that we have one CPUPool */
		updateClusterStatus();
	} else {
		std::cout<<"Cannot cast to CPUPool!" << std::endl;
		exit(EXIT_FAILURE);
	}
}
void Cluster::removeCPUPool(CPUPool* c) {
	if (c!=nullptr) {
		pool_list.remove(c);
		total_cpu  -= c->getTotalCores();
		used_cpu -= c->getUsedCores();
		total_local_memory -= c->getTotalMemSize();
		used_local_memory -= c->getUsedMemSize();
		updateClusterStatus();
	} else {
		std::cout<<"Cannot cast to CPUPool!" << std::endl;
		exit(EXIT_FAILURE);
	}
}
void Cluster::addLocalMemPool(LocalMemPool* m) {
	if (m!=nullptr) {
		pool_list.push_back(m);
		total_local_remote_memory += m->getTotalMemSize();
		used_local_remote_memory += m->getUsedMemSize();
		local_remote_memory_cost = m->getMemCost(); /* Assumed that we have one LocalMemPool */
		updateClusterStatus();
	} else {
		std::cout<<"Cannot cast to LocalMemPool!" << std::endl;
		exit(EXIT_FAILURE);
	}
}
void Cluster::removeLocalMemPool(LocalMemPool* m) {
	if (m!=nullptr) {
		pool_list.remove(m);
		total_local_remote_memory -= m->getTotalMemSize();
		used_local_remote_memory -= m->getUsedMemSize();
		updateClusterStatus();
	} else {
		std::cout<<"Cannot cast to LocalMemPool!" << std::endl;
		exit(EXIT_FAILURE);
	}
}
void Cluster::addRemoteMemPool(RemoteMemPool* m) {
	if (m!=nullptr) {
		pool_list.push_back(m);
		total_remote_remote_memory += m->getTotalMemSize();
		used_remote_remote_memory += m->getUsedMemSize();
		remote_remote_memory_cost = m->getMemCost(); /* Assumed that we have one RemoteMemPool */
		updateClusterStatus();
	} else {
		std::cout<<"Cannot cast to RemoteMemPool!" << std::endl;
		exit(EXIT_FAILURE);
	}
}
void Cluster::removeRemoteMemPool(RemoteMemPool* m) {
	if (m!=nullptr) {
		pool_list.remove(m);
		total_local_remote_memory -= m->getTotalMemSize();
		used_local_remote_memory -= m->getUsedMemSize();
		updateClusterStatus();
	} else {
		std::cout<<"Cannot cast to RemoteMemPool!" << std::endl;
		exit(EXIT_FAILURE);
	}
}


unsigned int Cluster::getCPUUtilization(void) {
	//TODO: exception handling
	return (unsigned int) (getUsedCPU()*100)/getTotalCPU();
}



unsigned int Cluster::getMemUtilization(void) {
	//TODO: exception handling
	return (unsigned int) (getUsedMem()*100)/getTotalMem();
}

void Cluster::print(std::ostream &out) {
	out << "======" << std::endl;
	out << "Cluster: " << getID() << std::endl;
	out << getUsedCPU() << "\\" << getTotalCPU() << " cores" << std::endl;
	out << getUsedMem() << "\\" << getTotalMem() << " MB" << std::endl;
	out << "......" << std::endl;
	out << "Owned pools: ";
	for (auto itr = pool_list.begin();
		         itr != pool_list.end(); itr++) {
		out << (*itr)->getID() << " ";
	}
	out << std::endl;
}

/* Print in CSV format */
void Cluster::printCSV(std::ofstream& file) {

	/* The header of csv file - to update look in DataCenter::printClustersCSV
	cluster_id
	cluster_utilization
	total_cpu
	used_cpu
	cpu_utilization
	total_memory
	total_used_memory
	memory_utilization
	total_local_memory
	used_local_memory
	local_memory_cost 
	total_local_remote_memory 
	used_local_remote_memory
	local_remote_memory_cost 
	total_remote_remote_memory 
	used_remote_remote_memory 
	remote_remote_memory_cost*/

	file << getID() << ","
	<< getUtilization() << ","
	<< getTotalCPU() << ","
	<< getUsedCPU() << ","
	<< getCPUUtilization() << ","
	<< getTotalMem() << ","
	<< getUsedMem() << ","
	<< getMemUtilization() << ","
	<< getTotalLocalMem() << ","
	<< getUsedLocalMem() << ","
	<< getLocalMemCost() << ","
	<< getTotalLocalRemoteMem() << ","
	<< getUsedLocalRemoteMem() << ","
	<< getLocalRemoteMemCost() << ","
	<< getTotalRemoteRemoteMem() << ","
	<< getUsedRemoteRemoteMem() << ","
	<< getRemoteRemoteMemCost() << 
	"\n";

}

void Cluster::addLogicalServer(LogicalServer *ls, Solution s) {
	if (ls!=nullptr && s.solved) {
		logical_server_list.push_back(std::tuple<LogicalServer*,Solution>(ls,s));
		used_cpu += s.cores;
		used_local_memory += s.local_memory;
		used_local_remote_memory += s.local_remote_memory;
		used_remote_remote_memory += s.remote_remote_memory;
		updateClusterStatus();
		/*TODO update the Pools */
		if(used_local_memory>total_local_memory) {
			std::cout<<"wrong local mem "<< getID() << " "<<ls->getID() <<std::endl;
			std::cout<< getUsedLocalMem()<< " " <<getTotalLocalMem() << " "<<ls->getID() <<std::endl;
			exit(1);
		}
	}
	else {
		std::cout<<"Logical server/Solution cannot be NULL!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Cluster::removeLogicalServer(LogicalServer *ls) {
	if (ls!=nullptr) {
		for (auto itr = logical_server_list.begin();
		         itr != logical_server_list.end(); itr++) {
			if(std::get<0>((*itr)) == ls) {
				Solution s = std::get<1>((*itr));
				logical_server_list.erase(itr);
				used_cpu -= s.cores;
				used_local_memory -= s.local_memory;
				used_local_remote_memory -= s.local_remote_memory;
				used_remote_remote_memory -= s.remote_remote_memory;
				updateClusterStatus();
			}
		}

	}
	else {
		std::cout<<"Logical server cannot be NULL!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

} /* namespace DC */
