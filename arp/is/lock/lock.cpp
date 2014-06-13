#include "lock.h"

using user::lock;

lock::lock( sc_module_name module_name, int num_of_procs, mips1 **procs) :
  sc_module( module_name ),
  target_export("lport") ,
  num_of_processors(num_of_procs),
  processors(procs)
{
    /// Binds target_export to the memory
    target_export( *this );

    // Initialize locks

    waiting_queue = (int *) malloc(NUMBER_OF_LOCKS * num_of_procs * sizeof *waiting_queue);
    for (int i = 0; i < NUMBER_OF_LOCKS; i++) {
      locks[i] = 0;
      for (int j = 0; j < num_of_procs; j++)
        waiting_queue[i * num_of_procs + j] = -1;
    }
       
}

lock::~lock()
{
  free(waiting_queue);
}

ac_tlm_rsp_status lock::writem( const uint32_t &a, const uint32_t &d ) 
{
  int lock_id = (d & 0x0000FF00) >> 8;
  int proc_id = d & 0x000000FF;

  if ((a & 0xFFFF0000) == 0x60010000) {
    // Try to get mutex
    if (locks[lock_id] == 0) {
      locks[lock_id] = 1;
      return SUCCESS;
    } else {
      //cout << "proc_id = " << proc_id << " sleeps" << endl;
      processors[proc_id]->ac_wait_sig = 0;    
    }

    // Put processor in queue
    int i = lock_id * num_of_processors;
    while ( waiting_queue[i] != -1 )
      i++;
    waiting_queue[i] = proc_id; 
    return SUCCESS;
  } else if ((a & 0xFFFF0000) == 0x60020000) {
    // Freeing lock
    int q = lock_id * num_of_processors;
    if ( waiting_queue[q] == -1 ) {
      locks[lock_id] = 0;
      return SUCCESS;
    }

    //cout << "proc_id = " << waiting_queue[q] << " awakes" << endl;
    processors[waiting_queue[q]]->ac_wait_sig = 1;
    for (int i = 0; i < num_of_processors - 1; i++) 
      waiting_queue[q + i] = waiting_queue[q + i + 1];
    return SUCCESS;
  } else {
    cout << endl << endl << "Invalid lock addr" << endl << endl;
    return SUCCESS;
  }
  return SUCCESS;
}

ac_tlm_rsp_status lock::readm( const uint32_t &a, uint32_t &d ) 
{
  // Not used
}
