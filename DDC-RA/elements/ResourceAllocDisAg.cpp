/*
 * ResourceAllocDisAg.cpp
 *
 *  Created on: Aug 14, 2019
 *      Author: alireza
 */

#include "ResourceAllocDisAg.h"

ResourceAllocDisAg::ResourceAllocDisAg(DC::Cluster *c, DC::LogicalServer *s)
: number_cpu(c->getAvailCPU()),
  local_meory_size(c->getAvailLocalMem()),
  remote_1_memory_size(c->getAvailLocalRemoteMem()),
  remote_2_memory_size(c->getAvailRemoteRemoteMem()),
  remote_1_memory_penalty(c->getLocalRemoteMemCost()),
  remote_2_memory_penalty(c->getRemoteRemoteMemCost()),
  local_memory_price(LOCAL_MEM_PRICE), //TODO: This can be extended to be per-pool
  remote_1_memory_price(LOCAL_REMOTE_MEM_PRICE),
  remote_2_memory_price(REMOTE_REMOTE_MEM_PRICE),
  logical_server_cores(s->getCPUNumber()),
  logical_server_memory(s->getMemSize()),
  logical_server_local_SLA(s->getRequiredLocalSLA()),
  logical_server_remote_SLA(s->getRequiredRemoteSLA()),
  allocated_local_memory(*this,0,s->getMemSize()),
  allocated_remote_1_memory(*this,0,s->getMemSize()),
  allocated_remote_2_memory(*this,0,s->getMemSize()),
  price(*this,0,100*1000*1000) {

	/* Constraints */

	/* Allocate enough resources for the logical server */

	//rel(*this, number_cpu >= logical_server_cores);

	rel(*this, allocated_local_memory + allocated_remote_1_memory + allocated_remote_2_memory == logical_server_memory);

	rel(*this, remote_1_memory_penalty*allocated_remote_1_memory + remote_2_memory_penalty*allocated_remote_2_memory <= logical_server_remote_SLA);

	rel(*this, allocated_local_memory >= logical_server_local_SLA);

	/* Do not exceed the available capacity of resources */
	rel(*this, allocated_local_memory <= local_meory_size);
	rel(*this, allocated_remote_1_memory <=  remote_1_memory_size);
	rel(*this, allocated_remote_2_memory <=  remote_2_memory_size);

	/* Price Constraints */
	rel(*this, price == allocated_local_memory*local_memory_price
			+ allocated_remote_1_memory * remote_1_memory_price
			+ allocated_remote_2_memory * remote_2_memory_price);

	/* Branching */

	/* Start branching from the furthest memory */
	branch(*this, allocated_remote_2_memory, INT_VAL_MAX());
	branch(*this, allocated_remote_1_memory, INT_VAL_MAX());
	branch(*this, allocated_local_memory, INT_VAL_MIN());


}

ResourceAllocDisAg::ResourceAllocDisAg(ResourceAllocDisAg &s)
: Space(s),
  number_cpu(s.number_cpu),
  local_meory_size(s.local_meory_size),
  remote_1_memory_size(s.remote_1_memory_size),
  remote_2_memory_size(s.remote_2_memory_size),
  remote_1_memory_penalty(s.remote_1_memory_penalty),
  remote_2_memory_penalty(s.remote_2_memory_penalty),
  local_memory_price(s.local_memory_price),
  remote_1_memory_price(s.remote_1_memory_price),
  remote_2_memory_price(s.remote_2_memory_price),
  logical_server_cores(s.logical_server_cores),
  logical_server_memory(s.logical_server_memory),
  logical_server_local_SLA(s.logical_server_local_SLA),
  logical_server_remote_SLA(s.logical_server_remote_SLA) {

	allocated_local_memory.update(*this,s.allocated_local_memory);
	allocated_remote_1_memory.update(*this,s.allocated_remote_1_memory);
	allocated_remote_2_memory.update(*this,s.allocated_remote_2_memory);
	price.update(*this,s.price);
}

void ResourceAllocDisAg::constrain(const Space &_s) {
	const ResourceAllocDisAg& s = static_cast<const ResourceAllocDisAg&>(_s);
	int current_price = s.allocated_local_memory.val()*local_memory_price
			+ s.allocated_remote_1_memory.val() * remote_1_memory_price
			+ s.allocated_remote_2_memory.val() * remote_2_memory_price;
	rel(*this, price < current_price);
}

Space* ResourceAllocDisAg::copy(void) {
	return new ResourceAllocDisAg(*this);
}

void ResourceAllocDisAg::print(std::ostream &os) const {
	os << "Local Allocated Memory: " << allocated_local_memory.val() << " MB" <<std::endl;
	os << "Remote 1 Allocated Memory: "<< allocated_remote_1_memory.val() << " MB" << std::endl;
	os << "Remote 2 Allocated Memory: "<< allocated_remote_2_memory.val() << " MB" << std::endl;
	os << "Price: " << price.val() << " USD" << std::endl;
}

int ResourceAllocDisAg::getAllocatedLocalMem(void) {
	return allocated_local_memory.val();
}

int ResourceAllocDisAg::getAllocatedRemote1Mem(void) {
	return allocated_remote_1_memory.val();
}

int ResourceAllocDisAg::getAllocatedRemote2Mem(void) {
	return allocated_remote_2_memory.val();
}

int ResourceAllocDisAg::getPrice(void) {
	return price.val();
}
