//////////////////////////////////////////////////////////////////////////////

#ifndef ROUTER_H_
#define ROUTER_H_

//////////////////////////////////////////////////////////////////////////////

// Standard includes
// SystemC includes
#include <systemc>
// ArchC includes
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"

//////////////////////////////////////////////////////////////////////////////

// using statements
using tlm::tlm_transport_if;

//////////////////////////////////////////////////////////////////////////////

//#define DEBUG

/// Namespace to isolate memory from ArchC
namespace user
{

  /// A TLM memory
  class router :
    public sc_module,
    public ac_tlm_transport_if // Using ArchC TLM protocol
  {
    public:
      /// Exposed port with ArchC interface
      ac_tlm_port DM_port;
      ac_tlm_port wake_port;
      ac_tlm_port lock_port;
      ac_tlm_port offload_port;
      sc_export< ac_tlm_transport_if > target_export1, target_export2;

      /// Virtual function, must be implemented
      ac_tlm_rsp transport( const ac_tlm_req &request ) {
// DEBUG
/*if (request.addr >= 0x60000000 ) {
cout << "Transport addr = 0x" << hex << request.addr
     << " data = 0x" << request.data 
     << (request.type == WRITE ? " WRITE" : " READ") << endl;
}*/


        ac_tlm_rsp response;
        if (request.addr < 0x60000000 ) {
          // Memory
          response = DM_port->transport(request);
        } else if (request.addr == 0x60000000) {
          // Start up
          response = wake_port->transport(request);
        } else if (request.addr == 0x60010000 || request.addr == 0x60020000) {
          // lock
          response = lock_port->transport(request);
        } else if (request.addr == 0x60030000) {
          // offload
          response = offload_port->transport(request);
        } else {
          response.status = ERROR;
        }
        return response;
      }

      /// Default constructor
      router( sc_module_name module_name );

  };
};
#endif // ROUTER_H_
