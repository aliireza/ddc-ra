/*
 * LogicalServer.h
 *
 *  Created on: Aug 12, 2019
 *      Author: alireza
 */

#ifndef ELEMENTS_LOGICALSERVER_H_
#define ELEMENTS_LOGICALSERVER_H_

#include "Constants.h"

namespace DC {

class LogicalServer {
public:
	LogicalServer();
	LogicalServer(unsigned long long nc, unsigned long long sm, unsigned long long rls, unsigned long long rrs);
	~LogicalServer();
	unsigned int getID(void);
	unsigned long long getCPUNumber(void);
	unsigned long long getMemSize(void); /* Total memory Logical Server */
	unsigned long long getRequiredLocalSLA(void); /* Minimum required local memory */
	unsigned long long getRequiredRemoteSLA(void); /* Maximum tolarable weighted remote memory */
	unsigned long long getLocalMemSize(void);
	unsigned long long getLocalRemoteMemSize(void);
	unsigned long long getRemoteRemoteMemSize(void);
	unsigned long long getCost(void);
	unsigned long long getPrice(void);
	int allocate(Solution s);

	
	friend std::ostream& operator << (std::ostream &out, LogicalServer& c);
	friend std::ofstream& operator << (std::ofstream &file, LogicalServer& c);


private:
	const unsigned int logical_server_id;
	const unsigned long long n_cpus;
	const unsigned long long s_mem; /* Total memory Logical Server */
	const unsigned long long required_local_SLA;
	const unsigned long long required_remote_SLA;
	unsigned long long local_memory;
	unsigned long long local_remote_memory;
	unsigned long long remote_remote_memory;
	static unsigned long long logical_server_count;
	unsigned long long cost; /* This is the cost /penalty of logical server based on current configuration */
	unsigned long long price;
	int setPriceAndCost(unsigned long long p, unsigned long long c);
	int setMemory(unsigned long long lm, unsigned long long lrm, unsigned long long rrm);





	void print(std::ostream& out);
	void printCSV(std::ofstream& file);

};

inline std::ostream& operator << (std::ostream &out, LogicalServer& s) {
	s.print(out);
	return out;
}

inline std::ofstream& operator << (std::ofstream &file, LogicalServer& s) {
	s.printCSV(file);
	return file;
}

} /* namespace DC */

#endif /* ELEMENTS_LOGICALSERVER_H_ */
