//////////////////////////////////////////////////////////////////////////////

#ifndef LOCK_H_ 
#define LOCK_H_

//////////////////////////////////////////////////////////////////////////////

// Standard includes
// SystemC includes
#include <systemc>
// ArchC includes
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"
#include "../../processors/mips1/mips1.H"

//////////////////////////////////////////////////////////////////////////////

// using statements
using tlm::tlm_transport_if;

//////////////////////////////////////////////////////////////////////////////

//#define DEBUG

/// Namespace to isolate memory from ArchC
namespace user
{
  const int NUMBER_OF_LOCKS = 4;

  /// A TLM memory
  class lock :
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
      lock ( sc_module_name module_name, int num_of_procs, mips1 **procs );

      virtual ~lock();

    private:
      int num_of_processors;
      mips1 **processors;
      int locks[NUMBER_OF_LOCKS];
      int *waiting_queue;

  };
};
#endif // LOCK_H_
