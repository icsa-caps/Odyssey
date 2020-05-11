//
// Created by vasilis on 11/05/20.
//

#ifndef KITE_CLIENT_IF_UTIL_H
#define KITE_CLIENT_IF_UTIL_H

/*-------------------------------- CLIENT REQUEST ARRAY ----------------------------------------*/
// signal completion of a request to the client
static inline void signal_completion_to_client(uint32_t sess_id,
                                               uint32_t req_array_i, uint16_t t_id)
{
  if (ENABLE_CLIENTS) {
    struct client_op *req_array = &interface[t_id].req_array[sess_id][req_array_i];
    check_session_id_and_req_array_index((uint16_t) sess_id, (uint16_t) req_array_i, t_id);

//    my_printf(yellow, "Wrkr %u/%u completing poll ptr %u for req %u at state %u \n", t_id,
//           sess_id, req_array_i, req_array->opcode, req_array->state);

    if (ENABLE_ASSERTIONS) {
      if (req_array->state != IN_PROGRESS_REQ)
        printf("op %u, state %u slot %u/%u \n", req_array->opcode, req_array->state, sess_id, req_array_i);
    }
    check_state_with_allowed_flags(2, req_array->state, IN_PROGRESS_REQ);

    atomic_store_explicit(&req_array->state, (uint8_t) COMPLETED_REQ, memory_order_release);
    if (CLIENT_DEBUG)
      my_printf(green, "Releasing sess %u, ptr_to_req %u ptr %p \n", sess_id,
                req_array_i, &req_array->state);
  }
}

// signal that the request is being processed to tne client
static inline void signal_in_progress_to_client(uint32_t sess_id,
                                                uint32_t req_array_i, uint16_t t_id)
{
  if (ENABLE_CLIENTS) {

    struct client_op *req_array = &interface[t_id].req_array[sess_id][req_array_i];
    //my_printf(cyan, "Wrkr %u/%u signals in progress for  poll ptr %u for req %u at state %u \n", t_id,
    //       sess_id, req_array_i,req_array->opcode, req_array->state);
    check_session_id_and_req_array_index((uint16_t) sess_id, (uint16_t) req_array_i, t_id);
    if (ENABLE_ASSERTIONS) memset(&req_array->key, 0, TRUE_KEY_SIZE);
    check_state_with_allowed_flags(2, req_array->state, ACTIVE_REQ);
    atomic_store_explicit(&req_array->state, (uint8_t) IN_PROGRESS_REQ, memory_order_release);
  }
}

// Returns whether a certain request is active, i.e. if the client has issued a request in a slot
static inline bool is_client_req_active(uint32_t sess_id,
                                        uint32_t req_array_i, uint16_t t_id)
{
  struct client_op * req_array = &interface[t_id].req_array[sess_id][req_array_i];
  check_session_id_and_req_array_index((uint16_t) sess_id, (uint16_t) req_array_i, t_id);
  return req_array->state == ACTIVE_REQ;
}

// is any request of the client request array active
static inline bool any_request_active(uint16_t sess_id, uint32_t req_array_i, uint16_t t_id)
{
  for (uint32_t i = 0; i < PER_SESSION_REQ_NUM; i++) {
    if (is_client_req_active(sess_id, i, t_id)) {
      my_printf(red, "session %u slot %u, state %u pull ptr %u\n",
                sess_id, i, interface[t_id].req_array[sess_id][req_array_i].state, req_array_i);
      if (i == req_array_i) return false;
      return true;
    }
  }
  return false;
}

//
static inline void fill_req_array_when_after_rmw(struct rmw_local_entry *loc_entry, uint16_t t_id)
{
  if (ENABLE_CLIENTS) {
    struct client_op *cl_op = &interface[t_id].req_array[loc_entry->sess_id][loc_entry->index_to_req_array];
    if (ENABLE_ASSERTIONS) assert(loc_entry->rmw_val_len == cl_op->val_len);
    switch (loc_entry->opcode) {
      case RMW_PLAIN_WRITE:
        // This is really a write so no need to read anything
        //cl_op->rmw_is_successful = true;
        break;
      case FETCH_AND_ADD:
        memcpy(cl_op->value_to_read, loc_entry->value_to_read, cl_op->val_len);
        //*cl_op->rmw_is_successful = true; // that will segfault, no bool pointer is passed in the FAA
        //printf("%u %lu \n", loc_entry->log_no, *(uint64_t *)loc_entry->value_to_write);
        break;
      case COMPARE_AND_SWAP_WEAK:
      case COMPARE_AND_SWAP_STRONG:
        *(cl_op->rmw_is_successful) = loc_entry->rmw_is_successful;
        if (!loc_entry->rmw_is_successful)
          memcpy(cl_op->value_to_read, loc_entry->value_to_read, cl_op->val_len);
        break;
      default:
        if (ENABLE_ASSERTIONS) assert(false);
    }
  }
}

static inline void fill_req_array_on_rmw_early_fail(uint32_t sess_id, uint8_t* value_to_read,
                                                    uint32_t req_array_i, uint16_t t_id)
{
  if (ENABLE_CLIENTS) {
    if (ENABLE_ASSERTIONS) {
      assert(value_to_read != NULL);
      check_session_id_and_req_array_index((uint16_t) sess_id, (uint16_t) req_array_i, t_id);
    }
    struct client_op *cl_op = &interface[t_id].req_array[sess_id][req_array_i];
    *(cl_op->rmw_is_successful) = false;
    memcpy(cl_op->value_to_read, value_to_read, cl_op->val_len);
  }
}





#endif //KITE_CLIENT_IF_UTIL_H