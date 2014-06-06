#include "router.h"

using user::router;

//ac_tlm_mem mem("mem");

router::router( sc_module_name module_name) :
  sc_module( module_name ),
  target_export("rport") ,
  DM_port("DM_port", 5242880U)
{
    /// Binds target_export to the memory
    target_export( *this );
}

