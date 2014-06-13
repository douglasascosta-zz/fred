//////////////////////////////////////////////////////////////////////////////

#ifndef OFFLOAD_H_
#define OFFLOAD_H_

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

  class offload :
    public sc_module,
    public ac_tlm_transport_if // Using ArchC TLM protocol
  {
    public:
      /// Exposed port with ArchC interface
      sc_export< ac_tlm_transport_if > target_export;
      ac_tlm_rsp_status writem( const uint32_t &, const uint32_t &);
      ac_tlm_rsp_status readm( const uint32_t &, uint32_t &);

      /// request for 
      ac_tlm_rsp transport( const ac_tlm_req &request ) {

        ac_tlm_rsp response;

        switch( request.type ) {
        case READ :
          response.status = readm( request.addr, response.data );
          break;
        case WRITE :
          response.status = writem( request.addr, request.data );
          break;
        default :
          response.status = ERROR;
          break;
        }

        return response;
      }

      /// Default constructor
      offload( sc_module_name module_name );

      virtual ~offload();

    private:
      int res;    
  };
};
#endif // OFFLOAD_H_
