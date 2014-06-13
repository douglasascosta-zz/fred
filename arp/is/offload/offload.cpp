#include "offload.h"

using user::offload;

offload::offload( sc_module_name module_name ) :
  sc_module( module_name ),
  target_export("oport") ,
  res(0) 
{
    /// Binds target_export to the memory
    target_export( *this );
}

offload::~offload()
{
  //Empty
}

ac_tlm_rsp_status offload::writem( const uint32_t &a, const uint32_t &d ) 
{
  // Input for offload
  int v = 0x000000FF & d;
  int val = (0x0000FF00 & d) >> 8;
  res = v - val;
  return SUCCESS;
}

ac_tlm_rsp_status offload::readm( const uint32_t &a, uint32_t &d ) 
{
  // Output for offload
  *((uint32_t *) &d) = res << 24;
  return SUCCESS;
}

