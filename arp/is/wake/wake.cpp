#include "wake.h"

using user::wake;

wake::wake( sc_module_name module_name, int num_of_procs, mips1 **procs) :
  sc_module( module_name ),
  target_export("wport") ,
  num_of_processors(num_of_procs),
  processors(procs)
{
    /// Binds target_export to the memory
    target_export( *this );

}

wake::~wake()
{
  //Empty
}

ac_tlm_rsp_status wake::writem( const uint32_t &a, const uint32_t &d ) 
{
  // Used as a barrier for all processes join at execution point
  static int end_procs = 0;
  end_procs++;
  int proc_id = 0x000000FF & d;
  processors[proc_id]->ac_wait_sig = 0;

  if (end_procs == num_of_processors) {
    end_procs = 0;
    for (int i = 0; i < num_of_processors; i++)
      processors[i]->ac_wait_sig = 1;
  }
  return SUCCESS;
}

ac_tlm_rsp_status wake::readm( const uint32_t &a, uint32_t &d ) 
{
  static int core = 0;
  *((uint32_t *) &d) = core++ << 24;

  static bool isBootstrap = true;
  if (isBootstrap) {
    isBootstrap = false;
    // Init all processors
    for (int i = 0; i < num_of_processors; i++)
      processors[i]->ac_wait_sig = 1;
  }

  return SUCCESS;
}

