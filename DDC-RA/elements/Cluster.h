/*
 * Cluster.h
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#ifndef ELEMENTS_CLUSTER_H_
#define ELEMENTS_CLUSTER_H_

#include "Constants.h"
#include "Pool.h"
#include "CPUPool.h"
#include "LocalMemPool.h"
#include "RemoteMemPool.h"
#include "LogicalServer.h"

namespace DC {


class Cluster {
public:
	Cluster();
	~Cluster();
	unsigned int getUtilization(void);
	unsigned int getID(void) const;
	unsigned long long getTotalCPU(void);
	unsigned long long getUsedCPU(void);
	unsigned long long getAvailCPU(void);
	unsigned long long getTotalMem(void);
	unsigned long long getUsedMem(void);
	unsigned long long getAvailMem(void);
	unsigned long long getTotalLocalMem(void);
	unsigned long long getUsedLocalMem(void);
	unsigned long long getAvailLocalMem(void);
	unsigned long long getLocalMemCost(void);
	unsigned long long getTotalLocalRemoteMem(void);
	unsigned long long getUsedLocalRemoteMem(void);
	unsigned long long getAvailLocalRemoteMem(void);
	unsigned long long getLocalRemoteMemCost(void);
	unsigned long long getTotalRemoteRemoteMem(void);
	unsigned long long getUsedRemoteRemoteMem(void);
	unsigned long long getAvailRemoteRemoteMem(void);
	unsigned long long getRemoteRemoteMemCost(void);
	unsigned long long getTotalRemoteMem(void);
	unsigned long long getUsedRemoteMem(void);
	unsigned long long getAvailRemoteMem(void);


	void addPool(Pool* p);
	void removePool(Pool* p);
	std::list<Pool*> getPools(void);
	std::list<CPUPool*> getCPUPools(void);
	std::list<LocalMemPool*> getLocalMemPools(void);
	std::list<RemoteMemPool*> getRemoteMemPools(void);
	static unsigned long long cluster_count;
	friend std::ostream& operator << (std::ostream &out, Cluster& c);
	friend std::ofstream& operator << (std::ofstream &file, Cluster& c);


	void addLogicalServer(LogicalServer* ls,Solution s);
	void removeLogicalServer(LogicalServer* ls);



private:
	const unsigned int cluster_id;
	Status cluster_status;
	std::list<Pool*> pool_list;
	std::list<std::tuple<LogicalServer*, Solution>> logical_server_list;
	unsigned int cluster_utilization;
	unsigned long long total_cpu;
	unsigned long long used_cpu;
	unsigned long long total_local_memory;				/* Memory in the cpu pool */
	unsigned long long used_local_memory;
	unsigned long long local_memory_cost;
	unsigned long long total_local_remote_memory;		/* Memory in the same rack */
	unsigned long long used_local_remote_memory;
	unsigned long long local_remote_memory_cost;
	unsigned long long total_remote_remote_memory;		/* Memory in the remote rack */
	unsigned long long used_remote_remote_memory;
	unsigned long long remote_remote_memory_cost;

	void setClusterStatus(Status s);
	void updateClusterStatus(void);
	void addCPUPool(CPUPool* c);
	void removeCPUPool(CPUPool* c);
	void addLocalMemPool(LocalMemPool* m);
	void removeLocalMemPool(LocalMemPool* m);
	void addRemoteMemPool(RemoteMemPool* m);
	void removeRemoteMemPool(RemoteMemPool* m);
	unsigned int getCPUUtilization(void);
	unsigned int getMemUtilization(void);

	 void print(std::ostream& out);
	 void printCSV(std::ofstream& file);
};

inline std::ostream& operator << (std::ostream &out, Cluster& c) {
	c.print(out);
	return out;
}

inline std::ofstream& operator << (std::ofstream &file, Cluster& c) {
	c.printCSV(file);
	return file;
}

} /* namespace DC */

#endif /* ELEMENTS_CLUSTER_H_ */
