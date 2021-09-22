/*
 * ResourceAllocDisAg.h
 *
 *  Created on: Aug 14, 2019
 *      Author: alireza
 */

#ifndef ELEMENTS_RESOURCEALLOCDISAG_H_
#define ELEMENTS_RESOURCEALLOCDISAG_H_

#include "Cluster.h"
#include "LogicalServer.h"

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>

using namespace Gecode;

class ResourceAllocDisAg : public Space {

public:
	ResourceAllocDisAg(DC::Cluster* c, DC::LogicalServer* s);
	ResourceAllocDisAg(ResourceAllocDisAg& s);
	virtual void constrain(const Space& _s);
	virtual Space* copy(void);
	virtual void print(std::ostream& os) const;
	int getAllocatedLocalMem(void);
	int getAllocatedRemote1Mem(void);
	int getAllocatedRemote2Mem(void);
	int getPrice(void);

private:

  /* Datacenter Constants */
  const unsigned long long number_cpu;
  const unsigned long long local_meory_size;
  const unsigned long long remote_1_memory_size;
  const unsigned long long remote_2_memory_size;

  /* SLA Constants */
  const unsigned int remote_1_memory_penalty;
  const unsigned int remote_2_memory_penalty;

  /* Price Constraints */
  const unsigned long long local_memory_price;
  const unsigned long long remote_1_memory_price;
  const unsigned long long remote_2_memory_price;

  /* Logical Server Requirements */
  const unsigned int logical_server_cores;
  const unsigned long long logical_server_memory;
  const unsigned long long logical_server_local_SLA;
  const unsigned long long logical_server_remote_SLA;

protected:
  /* Gecode Variables */
  IntVar allocated_local_memory;
  IntVar allocated_remote_1_memory;
  IntVar allocated_remote_2_memory;
  IntVar price;
};


#endif /* ELEMENTS_RESOURCEALLOCDISAG_H_ */
