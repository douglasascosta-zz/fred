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
      sc_export< ac_tlm_transport_if > target_export;

      /// Virtual function, must be implemented
      ac_tlm_rsp transport( const ac_tlm_req &request ) {

        ac_tlm_rsp response;
        response = DM_port->transport(request);

        return response;
      }

      /// Default constructor
      router( sc_module_name module_name );

  };
};
#endif // ROUTER_H_
