/*
 * DataCenter.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */
#include "DataCenter.h"
#include "ResourceAllocDisAg.h"
#ifdef __APPLE__
#include <json/value.h>
#include <json/json.h>
#endif
#ifdef __linux__
#include <json/value.h>
#include <jsoncpp/json/json.h>
#endif
//#include <fstream>

using namespace Gecode;

namespace DC {

unsigned long long DataCenter::dc_count=1;

DataCenter::DataCenter()
: dc_id(dc_count) {
	dc_utilization=0;
	dc_count++;
}

DataCenter::~DataCenter() {
	/* Remove clusters and pools */
	for (auto itr = pool_list.begin();
		         itr != pool_list.end(); itr++) {
		delete (*itr);
	}
	pool_list.clear();

	for (auto itr = cluster_list.begin();
		         itr != cluster_list.end(); itr++) {
		delete (*itr);
	}
	cluster_list.clear();
}

void DataCenter::addCluster(Cluster *c) {
	if(c!=nullptr) {
		cluster_list.push_back(c);
	} else {
		std::cout<<"Cluster cannot be nullptr!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void DataCenter::removeCluster(Cluster *c) {
	if(c!=nullptr) {
		cluster_list.remove(c);
		delete c;
	} else {
		std::cout<<"Cluster cannot be nullptr!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void DataCenter::addLogicalServer(LogicalServer *s) {
	if(s!=nullptr) {
		logical_server_list.push_back(s);
	} else {
		std::cout<<"Logical server cannot be nullptr!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void DataCenter::removeLogicalServer(LogicalServer *s) {
	if(s!=nullptr) {
		logical_server_list.remove(s);
		delete s;
	} else {
		std::cout<<"Cluster cannot be nullptr!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void DataCenter::addPool(Pool *p) {
	if(p!=nullptr) {
		pool_list.push_back(p);
	} else {
		std::cout<<"Pool cannot be nullptr!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void DataCenter::make(void) {
	/* Parse Topology */
	std::string input = "input.json";
	parse(input);
}

void DataCenter::print(void) {

	/* Print Pools */
	printPools();

	/* Print Clusters */
	printClusters();

	/* Print Logical Servers */
	printLogicalServers();
}


void DataCenter::printCSV(void) {
	
	/* Print Pools */
  	std::ofstream poolFile ("pool-data.csv", std::ofstream::out);
  	if (poolFile.is_open()){
  		printPoolsCSV(poolFile);
  		poolFile.close();
  	}

  	/* Print Clusters */
  	std::ofstream clusterFile ("cluster-data.csv", std::ofstream::out);
  	if (clusterFile.is_open()){
  		printClustersCSV(clusterFile);
  		clusterFile.close();
  	}
	

	/* Print Logical Servers */
	std::ofstream serverFile ("logicalServer-data.csv", std::ofstream::out);
  	if (serverFile.is_open()){
  		printLogicalServersCSV(serverFile);
  		serverFile.close();
  	}

}

void DataCenter::removePool(Pool *p) {
	if(p!=nullptr) {
		pool_list.remove(p);
	} else {
		std::cout<<"Pool cannot be nullptr!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::list<Cluster*> DataCenter::getClusters(void) {
	return cluster_list;
}

std::list<Pool*> DataCenter::getPools(void) {
	return pool_list;
}

unsigned int DataCenter::getUtilization(void) {
	unsigned long long util_sum=0,clust_num=0;
	for (auto itr = cluster_list.begin();
		         itr != cluster_list.end(); itr++) {
		util_sum +=(*itr)->getUtilization();clust_num++;
	}
	dc_utilization=util_sum/clust_num;
	return dc_utilization;
}

unsigned int DataCenter::getID(void) const {
	return dc_id;
}

int DataCenter::allocate(void) {

	std:: cout << "======" << std::endl;
	std:: cout << "Allocating >>>" << std::endl;
	/* Sort logical servers based on SLA */
	logical_server_list.sort([](LogicalServer* s1, LogicalServer* s2){
		//TODO: Exception handling
		return ( (unsigned long long) (s1->getRequiredLocalSLA()/s1->getRequiredRemoteSLA()) > (unsigned long long) (s2->getRequiredLocalSLA()/s2->getRequiredRemoteSLA()));
	});

	/*Allocate based on the input requirements of logical servers */
	for (auto ls_itr = logical_server_list.begin();
				ls_itr != logical_server_list.end(); ls_itr++) {
		Solution res;
		/* Sort clusters based on utilization */
		cluster_list.sort([](Cluster* c1, Cluster* c2){
			return ( c1->getUtilization() > c2->getUtilization());
		});
		for (auto cl_itr = cluster_list.begin();
					cl_itr != cluster_list.end(); cl_itr++) {
			//std::cout << "solving " << std::endl;
			res=solveCP((*cl_itr),(*ls_itr));

			if (res.solved) {
				(*cl_itr)->addLogicalServer((*ls_itr),res);
				std::cout << " Logical Server " << (*ls_itr)->getID() << " allocated on Cluster " << (*cl_itr)->getID() << std::endl;
				std::cout << " Local Memory: " << res.local_memory << std::endl;
				std::cout << " Local Remote Memory: " << res.local_remote_memory <<std::endl;
				std::cout << " Remote Remote Memory: " << res.remote_remote_memory <<std::endl;
				if((*ls_itr)->allocate(res)) {
					break;
				}
			}

		}
		if(!res.solved) {
			std::cout<<" No solution found!" << std::endl;
			return FAILURE;
		}
	}

	/* After allocation */
	std:: cout << "======" << std::endl;
	printClusters();

	return SUCCESS;
}



/* This function allocate resources based on FIFO over the resources*/
int DataCenter::allocate_ff (void) { //  FIRST FIT aproache

	std:: cout << "======" << std::endl;
	std:: cout << "Allocating >>>" << std::endl;
	
	// /* Sort logical servers based on SLA */
	// logical_server_list.sort([](LogicalServer* s1, LogicalServer* s2){
	// 	//TODO: Exception handling
	// 	return ( (unsigned long long) (s1->getRequiredLocalSLA()/s1->getRequiredRemoteSLA()) > (unsigned long long) (s2->getRequiredLocalSLA()/s2->getRequiredRemoteSLA()));
	// });

	/*Allocate based on the input requirements of logical servers */
	for (auto ls_itr = logical_server_list.begin();
				ls_itr != logical_server_list.end(); ls_itr++) {
		
		Solution res;

		// /* Sort clusters based on utilization */
		// cluster_list.sort([](Cluster* c1, Cluster* c2){
		// 	return ( c1->getUtilization() > c2->getUtilization());
		// });

		for (auto cl_itr = cluster_list.begin();
					cl_itr != cluster_list.end(); cl_itr++) {
			
			/* find the resource on cluster */
			res=solveFF((*cl_itr),(*ls_itr));

			if (res.solved) {
				(*cl_itr)->addLogicalServer((*ls_itr),res);
				//(*ls_itr)->price = res.price; /* set the price of logical server */
				std::cout << " Logical Server " << (*ls_itr)->getID() << " allocated on Cluster " << (*cl_itr)->getID() << std::endl;
				std::cout << " Local Memory: " << res.local_memory << std::endl;
				std::cout << " Local Remote Memory: " << res.local_remote_memory <<std::endl;
				std::cout << " Remote Remote Memory: " << res.remote_remote_memory <<std::endl;
				if((*ls_itr)->allocate(res)) {
					break;
				}
			}

		}
		if(!res.solved) {
			std::cout<<" No solution found!" << std::endl;
			return FAILURE;
		}
	}

	/* After allocation */
	std:: cout << "======" << std::endl;
	printClusters();

	return SUCCESS;
}




void DataCenter::printPools(void) {
	std::cout << "Datacenter Pools: " << std::endl;
	for (auto itr = pool_list.begin();
		         itr != pool_list.end(); itr++) {
		std::cout << *(*itr);
	}
}

void DataCenter::printClusters(void) {
	std::cout << "Datacenter Clusters: " << std::endl;
	for (auto itr = cluster_list.begin();
		         itr != cluster_list.end(); itr++) {
		std::cout << *(*itr);
	}
}

void DataCenter::printLogicalServers(void) {
	std::cout << "Datacenter Logical Servers: " << std::endl;
	for (auto itr = logical_server_list.begin();
		         itr != logical_server_list.end(); itr++) {
		std::cout << *(*itr);
	}
}

/* CSV printing */
void DataCenter::printPoolsCSV(std::ofstream &ofs) {
	/* The Header Of CSV file */
	ofs << "ID, Type, Total_Memory, Used_Memory, Memory_Cost, Total_CPU, Used_CPU\n";
	for (auto itr = pool_list.begin();
		         itr != pool_list.end(); itr++) {
		ofs << *(*itr);
	}
}

void DataCenter::printClustersCSV(std::ofstream &ofs) {
	/*Header of CSV file if you change it make sure to apply the changes at Cluster::printCSV*/
	ofs << "cluster_id,"
	<< "cluster_utilization,"
	<< "total_cpu,"
	<< "used_cpu,"
	<< "cpu_utilization,"
	<< "total_memory,"
	<< "total_used_memory,"
	<< "memory_utilization,"
	<< "total_local_memory,"
	<< "used_local_memory,"
	<< "local_memory_cost,"
	<< "total_local_remote_memory," 
	<< "used_local_remote_memory,"
	<< "local_remote_memory_cost,"
	<< "total_remote_remote_memory,"
	<< "used_remote_remote_memory,"
	<< "remote_remote_memory_cost\n";

	for (auto itr = cluster_list.begin();
		         itr != cluster_list.end(); itr++) {
		ofs << *(*itr);
	}
}

void DataCenter::printLogicalServersCSV(std::ofstream &ofs) {
	/*Header of CSV file. if you make a changes make sure you apply same at LogicalServer::printCSV */
	ofs << "id,"
	<< "n_cpus,"
	<< "total_mem,"
	<< "required_local_SLA,"
	<< "required_remote_SLA,"
	<< "local_memory,"
	<< "local_remote_memory,"
	<< "remote_remote_memory,"
	<< "price,"
	<< "cost,"
	<< "LocalSLAViolation,"
	<< "RemoteSLAViolation\n";

	 for (auto itr = logical_server_list.begin();
	 	         itr != logical_server_list.end(); itr++) {
	 	ofs << *(*itr);
	 }
}

void DataCenter::parse(std::string input) {
	Json::Value root;
	std::ifstream inputfile(input);
	inputfile >> root;

	
	// Creating Pools
	int n_obj=0;
	int n_nested_obj=0;
	Json::Value leaf = root["pools"];
	for( int i=0; i< leaf.size();i++) {
		if(leaf[i]["type"].asString()=="CPU") {
			unsigned long long cores = leaf[i]["cores"].asUInt64();
			unsigned long long memory = leaf[i]["memory"].asUInt64();
			unsigned long long penalty = leaf[i]["penalty"].asUInt64();
			// std::cout << "CPU" << std::endl;
			// std::cout << cores << std::endl;
			// std::cout << memory << std::endl;
			// std::cout << penalty << std::endl;
			Pool* p = new CPUPool(cores,memory,penalty);
			addPool(p);
			n_obj++;
		} else if(leaf[i]["type"].asString()=="LOCALMEM") {
			unsigned long long cores = leaf[i]["cores"].asUInt64();
			unsigned long long memory = leaf[i]["memory"].asUInt64();
			unsigned long long penalty = leaf[i]["penalty"].asUInt64();
			// std::cout << "LOCALMEM" << std::endl;
			// std::cout << cores << std::endl;
			// std::cout << memory << std::endl;
			// std::cout << penalty << std::endl;
			Pool* p = new LocalMemPool(memory,penalty);
			addPool(p);
			n_obj++;
		} else if(leaf[i]["type"].asString()=="REMOTEMEM") {
			unsigned long long cores = leaf[i]["cores"].asUInt64();
			unsigned long long memory = leaf[i]["memory"].asUInt64();
			unsigned long long penalty = leaf[i]["penalty"].asUInt64();
			// std::cout << "LOCALMEM" << std::endl;
			// std::cout << cores << std::endl;
			// std::cout << memory << std::endl;
			// std::cout << penalty << std::endl;
			Pool* p = new RemoteMemPool(memory,penalty);
			addPool(p);
			n_obj++;
		} else {
			std::cout << "Wrong pool format!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	if(n_obj==0) {
		std::cout << "No pools!" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Create Clusters */
	n_obj=0;
	leaf = root["clusters"];
	for( int i=0; i< leaf.size();i++) {
		if(leaf[i].size()!=0) {
			n_nested_obj=0;
			Json::Value leaf2 = leaf[i]["pools"];
			Cluster* c = new Cluster();
			for( int j=0; j< leaf2.size();j++) {
				if(leaf2[j].isUInt64()) {
					unsigned long long pool_id = leaf2[j].asUInt64();
					std::cout << "Pool " << pool_id << std::endl;
					for (auto itr = pool_list.begin();
		         			itr != pool_list.end(); itr++) {
						if((*itr)->getID()==pool_id) {
							n_nested_obj++;
							c->addPool((*itr));
							break;
						}
					}
				}
			}
			if(n_nested_obj==0) {
				std::cout << "No pool for cluster" << std::endl;
				exit(EXIT_FAILURE);
			}
			addCluster(c);
			n_obj++;
		} else {
			std::cout << "Wrong cluster format!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	if(n_obj==0) {
		std::cout << "No clusters!" << std::endl;
		exit(EXIT_FAILURE);
	}


	/* Create Logical Servers */
	leaf = root["servers"];
	n_obj=0;
	for( int i=0; i< leaf.size();i++) {
		unsigned long long cores = leaf[i]["cores"].asUInt64();
		unsigned long long memory = leaf[i]["mem"].asUInt64();
		unsigned long long local_sla = leaf[i]["local_SLA"].asUInt64();
		unsigned long long remote_sla = leaf[i]["remote_SLA"].asUInt64();
		LogicalServer* s = new LogicalServer(cores, memory, local_sla, remote_sla);
		addLogicalServer(s);
		n_obj++;
	}
	if(n_obj==0) {
		std::cout << "No logical servers!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

Solution DataCenter::solveCP(Cluster* c, LogicalServer* s) {

	DC::Solution sol;
	if(c->getAvailCPU() >= s->getCPUNumber()) {
		ResourceAllocDisAg* problem = new ResourceAllocDisAg(c,s);
		BAB<ResourceAllocDisAg> e(problem);
		delete problem;
		std::vector<ResourceAllocDisAg*> solutions;
		ResourceAllocDisAg* optimal_solution;
		while(ResourceAllocDisAg* s = e.next()) {
			//solutions.push_back(s);
			optimal_solution=s;
		}

		//if(solutions.size()>0) {
		//if(ResourceAllocDisAg* optimal_solution = solutions.back()) {
			if(optimal_solution!=nullptr) {
			
			sol.cores=s->getCPUNumber();
			sol.local_memory=optimal_solution->getAllocatedLocalMem();
			sol.local_remote_memory=optimal_solution->getAllocatedRemote1Mem();
			sol.remote_remote_memory=optimal_solution->getAllocatedRemote2Mem();
			sol.price=optimal_solution->getPrice();
			sol.cost=sol.local_memory * c->getLocalMemCost() + sol.local_remote_memory * c->getLocalRemoteMemCost() + sol.remote_remote_memory * c->getRemoteRemoteMemCost() ;
			sol.solved=true;
			//std::cout << "hi" << solutions.size() << std::endl;
			/* Erase solutions */
			//for (auto itr = solutions.begin();
		    //     itr != solutions.end() && solutions.size()>0; itr++) {
		         
			//	delete (*itr);
				//solutions.erase(itr);
			//}
			//solutions.clear();

		}
	}
	//std::cout << "hi" << sol.solved;
	//}
	return sol;
}

Solution DataCenter::solveFF(Cluster* c, LogicalServer* s) {

	DC::Solution sol;

	unsigned long long needed_total_logical_server_mem= s->getMemSize();

	if(c->getAvailCPU() >= s->getCPUNumber() && c->getAvailMem() >= s->getMemSize()) { // We have enough CPU and memory in cluster 
		sol.cores=(int)s->getCPUNumber();
		/*We start from local local and try to fit the requirements*/
		if (c->getAvailLocalMem() > 0) { 
			if(needed_total_logical_server_mem>c->getAvailLocalMem()) {
				sol.local_memory= c->getAvailLocalMem();
				needed_total_logical_server_mem -= c->getAvailLocalMem();
			} else {
				sol.local_memory = needed_total_logical_server_mem;
				needed_total_logical_server_mem = 0;
			}
		}else{
				sol.local_memory=0;
			}

		if(c->getAvailLocalRemoteMem() > 0  && needed_total_logical_server_mem > 0 ) { /* If local partially meet */
			if(needed_total_logical_server_mem>c->getAvailLocalRemoteMem()) {
				sol.local_remote_memory= c->getAvailLocalRemoteMem();
				needed_total_logical_server_mem -= c->getAvailLocalRemoteMem();
			} else {
				sol.local_remote_memory = needed_total_logical_server_mem;
				needed_total_logical_server_mem = 0;
			}
		}else{
				sol.local_remote_memory =0;
			}

		if(c->getAvailRemoteRemoteMem() > 0  && needed_total_logical_server_mem > 0 ) { /* If local partially meet */
			if(needed_total_logical_server_mem>c->getAvailRemoteRemoteMem()) {
				sol.remote_remote_memory= c->getAvailRemoteRemoteMem();
				needed_total_logical_server_mem -= c->getAvailRemoteRemoteMem();
			} else {
				sol.remote_remote_memory = needed_total_logical_server_mem;
				needed_total_logical_server_mem = 0;
			}
		}else{
				sol.remote_remote_memory= 0;
			}
		sol.price=calcLogicalServerPrice(c, sol);
		sol.cost=sol.local_memory * c->getLocalMemCost() + sol.local_remote_memory * c->getLocalRemoteMemCost() + sol.remote_remote_memory * c->getRemoteRemoteMemCost() ;
		sol.solved=true;
	}

	return sol;
}




unsigned long long DataCenter::absCalc(unsigned long long a, unsigned long long b) {

	return (a > b) ? a - b : b - a;
}

/*Calculate the pric eof each logical server based on memeory */
unsigned long long DataCenter::calcLogicalServerPrice(Cluster* c, Solution sol){

	return ((sol.local_memory * LOCAL_MEM_PRICE) + (sol.local_remote_memory * LOCAL_REMOTE_MEM_PRICE) + (sol.remote_remote_memory * REMOTE_REMOTE_MEM_PRICE));
}


} /* namespace DC */
