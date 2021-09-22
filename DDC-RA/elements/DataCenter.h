/*
 * DataCenter.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#ifndef ELEMENTS_DATACENTER_H_
#define ELEMENTS_DATACENTER_H_

#include "Constants.h"
#include "Cluster.h"
#include "LogicalServer.h"
#include "ResourceAllocDisAg.h"

namespace DC {

class DataCenter {
public:
	DataCenter();
	~DataCenter();
	void make(void);
	void print(void);
	void printCSV(void); /* Print the result as a CSV format */	
	int allocate(void);
	int allocate_ff(void); /*First Fit allocation */
	std::list<Cluster*> getClusters(void);
	std::list<Pool*> getPools(void);
	unsigned int getUtilization(void);
	unsigned int getID (void) const;

private:
	const unsigned int dc_id;
	std::list<Cluster*> cluster_list;
	std::list<Pool*> pool_list;
	std::list<LogicalServer*> logical_server_list;
	static unsigned long long dc_count;
	unsigned int dc_utilization;

	void addCluster(Cluster* c);
	void removeCluster(Cluster* c);
	void addLogicalServer(LogicalServer* s);
	void removeLogicalServer(LogicalServer* s);
	unsigned long long calcLogicalServerPrice(Cluster* c, Solution sol);
	void addPool(Pool* p);
	void removePool(Pool* p);
	void printPools(void);
	void printClusters(void);
	void printLogicalServers(void);
	/* CSV format of print */
	void printPoolsCSV(std::ofstream &ofs);
	void printClustersCSV(std::ofstream &ofs);
	void printLogicalServersCSV(std::ofstream &ofs);
	void parse(std::string input);
	/* Different solvers */
	Solution solveCP(Cluster* c, LogicalServer* s);
	Solution solveFF(Cluster* c, LogicalServer* s);
	unsigned long long absCalc(unsigned long long a, unsigned long long b);


};

} /* namespace DC */

#endif /* ELEMENTS_DATACENTER_H_ */
