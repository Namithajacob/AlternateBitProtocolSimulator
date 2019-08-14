/*
 * Brief Description : This is the header file for receiver model.
 *
 * Detailed Description : The receiver receives the data and send
 * back acknowledgment after a fixed time delay. The receiver basically
 * has two phases, i.e active and passive. Initially the receiver will
 * be in passive phase and as soon as they receive a packet it changes
 * to active state and sends the acknowledgment after a time period and
 * then changes the state to passive.
 *
 *
 */

#ifndef __RECEIVER_HPP__
#define __RECEIVER_HPP__


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <limits>
#include <math.h> 
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <random>

#include "message.hpp"

using namespace cadmium;
using namespace std;

/*
 * This structure contains the input and output messages
 */
struct receiver_defs{
    struct output : public out_port<Message_t> {
    };
    struct input : public in_port<Message_t> {
    };
};
   
/*
 *  The class Receiver receives a message and the acknowledgment
 *  for receiving this message will be passed
 */

template<typename TIME>
class Receiver{
	/*
	 * putting definitions in context
	 */
    using defs=receiver_defs;
    public:
    /*
     * This constant has the value of time delay from input to output
     */
    TIME   PREPARATION_TIME;
    /*
     * Constructor for receiver
     * It initialize time delay constant with a value
     */
    Receiver() noexcept{
    	PREPARATION_TIME  = TIME("00:00:10");
        state.ack_num    = 0;
        state.sending     = false;
    }
            
    /*
     * In the structure below it has the acknowledgment number and state
     * of the receiver
     */
    struct state_type{
        int ack_num;
        bool sending;
    };
    state_type state;
    /*
     * Defining Input and output ports
     */
    using input_ports=std::tuple<typename defs::input>;
    using output_ports=std::tuple<typename defs::output>;

    /*
     * This function sets the receiver sending state off i.e sets it
     * as passive
     */
    void internal_transition() {
        state.sending = false;
    }

    /*
     * This function receives the message and checks the number of
     * message. If the number of message is greater than 1, it says
     * only one message per time unit. Else, it will set the receiver
     * sending state as on, i.e active
     */
    void external_transition(TIME e,
        typename make_message_bags<input_ports>::type mbs) {
            if(get_messages<typename defs::input>(mbs).size()>1){
        	    assert(false && "one message per time uniti");
            }
            for(const auto &x : get_messages<typename defs::input>(mbs)){
                state.ack_num = static_cast < int > (x.value);
                state.sending = true;
            }
                           
    }

    /*
     * This function calls both internal_transition and external_transition
     * functions.
     */
    void confluence_transition(TIME e,
    	typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }

    /*
     * This function sends acknowledgment to the output port
     * The acknowledgment value is calculated by the modulo of
     * acknowledgment number with 10.
     */
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        Message_t out;
        out.value = state.ack_num % 10;
        get_messages<typename defs::output>(bags).push_back(out);
        return bags;
    }

    /*
     * This function sets the next internal transition time.
     * If the receiver is in sending state, it will set the
     * next internal transition time as Preparation time, else
     * it will set to infinity
     */
    TIME time_advance() const {
        TIME next_internal;
        if (state.sending) {
            next_internal = PREPARATION_TIME;
        }else {
            next_internal = std::numeric_limits<TIME>::infinity();
        }
        return next_internal;
    }
    /*
     * returns acknowledgment number to a string stream
     */
    friend std::ostringstream& operator<<(std::ostringstream& os,
    const typename Receiver<TIME>::state_type& i) {
        os << "ackNum: " << i.ack_num;
        return os;
    }
};
  

#endif // __RECEIVER_HPP__
