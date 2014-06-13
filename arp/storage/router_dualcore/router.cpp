#include "router.h"

using user::router;

router::router( sc_module_name module_name) :
  sc_module( module_name ),
  target_export1("rport1") ,
  target_export2("rport2") ,
  DM_port("DM_port", 5242880U) ,
  wake_port("wake_port", 5242896U),
  lock_port("lock_port", 5242908U),
  offload_port("offload_port", 5242916U)
{
    /// Binds target_export to the memory
    target_export1( *this );
    target_export2( *this );
}

