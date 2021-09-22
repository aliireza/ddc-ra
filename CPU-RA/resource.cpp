/*
 *  Authors:
 *    Alireza Farshin <farshin@kth.se>
 *    Amir Roozbeh    <amirrsk@kth.se>
 *
 */

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <iostream>

using namespace Gecode;

class ResourceAllocDisAg : public IntMinimizeScript {


public:

  /* Datacenter Constants */

  int nCPUPool=2;
  int nCorePerPool=8; /* TODO: different pools can have different cores */
  unsigned long long localMemPerPoolSize=5*1024;      /*MB*/
  unsigned long long remoteMem1Size=10*1024;  /*MB*/
  unsigned long long remoteMem2Size=20*1024; /*MB*/


   /* SLA Constants */

  int remoteMem1Penalty=1;
  int remoteMem2Penalty=2;

  /* Price Constraints */

  int localMemPrice=100; /*USD*/
  int remoteMem1Price=50; /*USD*/
  int remoteMem2Price=25; /*USD*/

  /* Application Constants */
  int appCore=4;
  unsigned long long appMem=4*1024; /*MB*/
  unsigned long long remoteCallPenalty=2*1024;


  /* Variables */

  IntVar appLocalMem;
  IntVar appRemote1Mem;
  IntVar appRemote2Mem;
  IntVar price;

  /* Constructor */
  ResourceAllocDisAg(const Options& opt)
    : IntMinimizeScript(opt), 
    appLocalMem(*this,0,5*1024),
    appRemote1Mem(*this,0,10*1024),
    appRemote2Mem(*this,0,20*1024),
    price(*this,0,100*4*1024)
    {

    /* Constraints */


    /* Minimum Local Memory */
    rel(*this, (appRemote1Mem >= 512) || (appRemote1Mem==0));
    rel(*this, (appRemote2Mem >= 512) || (appRemote2Mem==0));

    /* Minimum Remote Memory - To avoid fragmentation */
    rel(*this, appLocalMem >= 100);

    /* Allocate enough memory for the app */
    rel(*this, appLocalMem + appRemote1Mem + appRemote2Mem == appMem);

    /* Penalty Constraint */
    rel(*this, remoteMem1Penalty*appRemote1Mem + remoteMem2Penalty*appRemote2Mem <= remoteCallPenalty);

    /* Price Constraint */
    rel(*this, price == appLocalMem * localMemPrice
    + appRemote1Mem * remoteMem1Price
    + appRemote2Mem * remoteMem2Price);

    /* Branching */

    /* Start from the memory with higher penalty */ 
    branch(*this, appRemote2Mem, INT_VAL_MAX());
    branch(*this, appRemote1Mem, INT_VAL_MAX());
    branch(*this, appLocalMem, INT_VAL_MAX());

  }


  /* Cost Function - Minimize Price and inherently Maximize Penalty*/
  virtual IntVar cost(void) const
  {
    return price;
  }

  /* Cloning Constructor */
  ResourceAllocDisAg(ResourceAllocDisAg& S)
  : IntMinimizeScript(S) {
    appLocalMem.update(*this, S.appLocalMem);
    appRemote1Mem.update(*this, S.appRemote1Mem);
    appRemote2Mem.update(*this, S.appRemote2Mem);
    price.update(*this, S.price);
  }

  /* Copying */
  virtual Space*
  copy(void) {
    return new ResourceAllocDisAg(*this);
  }

  /* Printing */
  virtual void
  print(std::ostream& os) const {
    os << "App Required Memory: " << appMem << " MB" << std::endl;
    os << "App Required Penalty: " << remoteCallPenalty << std::endl;
    os << "Local Allocated Memory: " << appLocalMem.val() << " MB" <<std::endl;
    os << "Remote 1 Allocated Memory: "<< appRemote1Mem.val() << " MB" << std::endl;
    os << "Remote 2 Allocated Memory: "<< appRemote2Mem.val() << " MB" << std::endl;
    os << "Achieved Penalty: "<<  remoteMem1Penalty*appRemote1Mem.val() + remoteMem2Penalty*appRemote2Mem.val() << std::endl;
    os << "Price: " << price.val() << " USD" << std::endl;
    }
};

/* Main */
int main(int argc, char* argv[]) {

  Options opt("ResourceAllocDisAg");
  opt.solutions(1);     /* Searching for all solution */
  opt.parse(argc,argv);
  
  //Script::run<ResourceAllocDisAg,BAB,Options>(opt);
  
  ResourceAllocDisAg* m = new ResourceAllocDisAg(opt);
  BAB<ResourceAllocDisAg> e(m);
  delete m;
  ResourceAllocDisAg* o;
  while (ResourceAllocDisAg* s = e.next()) {
	  //std::cout<<s->appLocalMem.val()<<std::endl; 
	  if(e.next()){
	  	delete s;
	  }else {
	  	o=s;
	  }
  }
  std::cout<< "Local Memory: " << o->appLocalMem.val() << std::endl;
  std::cout<< "Remote Memory 1: " << o->appRemote1Mem.val() << std::endl;
  std::cout<< "Remote Memory 2: " << o->appRemote2Mem.val() << std::endl;

  return 0;
}
