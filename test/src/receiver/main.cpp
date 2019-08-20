
/**
 * \brief  This main file of receiver implements the
 * operation of receiver model which can be used for testing.
 *
 * receiver receive the data and send back an acknowledgment extracted
 * from the received data after a time period.
 * The Application generator takes file path as input and stores the
 * output data. It also generates the logs using Cadmium and
 * Desttimes which are third party libraries.
 * The time limit set for this to run is 04:00:00:000, i.e it runs until the
 * mentioned time
 */

/**
 * Defining the file path for input
 */

#define RECEIVER_INPUT  "test/data/receiver/receiver_input_test.txt"

/**
 * Defining the sender output file path
 */
#define RECEIVER_OUTPUT  "test/data/receiver/receiver_test_output.txt"

/**
 * Defining path for new modified output
 */

#define FILTER_OUTPUT  "test/data/receiver/output.txt"

/**
 * Defining path for improved output file
 */

#define LIMIT_OUTPUT "test/data/receiver/limit_output.txt"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include <cadmium/modeling/coupling.hpp>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/concept/coupled_model_assert.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/tuple_to_ostream.hpp>
#include <cadmium/logger/common_loggers.hpp>

#include "../../../include/message.hpp"
#include "../../../lib/DESTimes/include/NDTime.hpp"
#include "../../../lib/vendor/include/iestream.hpp"
#include "../../../include/receiver_cadmium.hpp"

#include "../../../src/text_filter.cpp"
#include "../../../src/limit_output.cpp"

#include "../../../include/filter.hpp"
#include "../../../include/limit.hpp"

using namespace std;
using hclock=chrono::high_resolution_clock;
using TIME = NDTime;

/**
 *  Sets input ports for message
 */

struct input : public cadmium::in_port<message_t>{};

/**
 *  Sets Output ports for message
 */

struct output : public cadmium::out_port<message_t>{};

/**
 * \brief class ApplicationGen for Application Generator.
 *
 *  The below class application generator(ApplicationGen) takes the file path
 *  and transmits as message.
 */

template<typename T>
class ApplicationGen : public iestream_input<message_t,T>{
    public:

	/**
     * Default constructor for the class
	 */

    ApplicationGen() = default;

    /**
     * The parameterized constructor of ApplicationGen class takes the input
     * file path for the Application generator
     */

    ApplicationGen(const char* file_path) : iestream_input<message_t,
        T>(file_path) {}
};


int main(){

	/**
	 * initializing input parameters to pass to the function
	 */

	const char *input_file = RECEIVER_OUTPUT;
	const char *output_file = FILTER_OUTPUT;
	const char *limit_file = LIMIT_OUTPUT;


	/**
	 *  This variable will have the start time of simulation
	 */

    auto start = hclock::now();

    /**
     * In the below path mentioned, the messages and logs which are passed in
     * the execution time is stored.
     */

    static std::ofstream out_data(RECEIVER_OUTPUT);

    /**
     * The below structure calls the output stream and returns the data
     * stored in the output data files.
     */

    struct oss_sink_provider{
        static std::ostream& sink(){          
            return out_data;
        }
    };

    /**
     * Cadmium library functions are used to call the source logger
     * to generate the log files and store them.
     */

    using info=cadmium::logger::logger<cadmium::logger::logger_info,
    		   cadmium::dynamic::logger::formatter<TIME>,
			   oss_sink_provider>;
    using debug=cadmium::logger::logger<cadmium::logger::logger_debug,
    		    cadmium::dynamic::logger::formatter<TIME>,
				oss_sink_provider>;
    using state=cadmium::logger::logger<cadmium::logger::logger_state,
    		    cadmium::dynamic::logger::formatter<TIME>,
				oss_sink_provider>;
    using log_messages=cadmium::logger::logger<cadmium::logger::logger_messages,
    		           cadmium::dynamic::logger::formatter<TIME>,
					   oss_sink_provider>;
    using routing=cadmium::logger::logger<cadmium::logger::logger_message_routing,
    		      cadmium::dynamic::logger::formatter<TIME>,
				  oss_sink_provider>;
    using global_time=cadmium::logger::logger<cadmium::logger::logger_global_time,
    		          cadmium::dynamic::logger::formatter<TIME>,
					  oss_sink_provider>;
    using local_time=cadmium::logger::logger<cadmium::logger::logger_local_time,
    		         cadmium::dynamic::logger::formatter<TIME>,
					 oss_sink_provider>;
    using log_all=cadmium::logger::multilogger<info, debug, state,
    		      log_messages,routing, global_time, local_time>;

    using logger_top=cadmium::logger::multilogger<log_messages, global_time>;

    /**
     * Takes the input control file from the following path
     */

    string input_data_control = RECEIVER_INPUT;

    /**
     * pointer that points to a file
     */
    const char * p_input_data_control = input_data_control.c_str();

    /**
     * The generator is initialized here which considers the time and input file
     * and generates the output file
     */

    std::shared_ptr<cadmium::dynamic::modeling::model> generator =
        cadmium::dynamic::translate::make_dynamic_atomic_model<ApplicationGen,
		TIME, const char* >("generator" , std::move(p_input_data_control));

    /**
     * Gets the output from receiver1
     */

    std::shared_ptr<cadmium::dynamic::modeling::model> receiver1 =
    	cadmium::dynamic::translate::make_dynamic_atomic_model<Receiver,
		TIME>("receiver1");

    /**
     * Stores data obtained in top model operations over a time frame
     * which will be stored in output file
     */

    cadmium::dynamic::modeling::Ports iports_TOP = {};
    cadmium::dynamic::modeling::Ports oports_TOP = {typeid(output)};
    cadmium::dynamic::modeling::Models submodels_TOP = {generator, receiver1};
    cadmium::dynamic::modeling::EICs eics_TOP = {};
    cadmium::dynamic::modeling::EOCs eocs_TOP = {
        cadmium::dynamic::translate::make_EOC<receiver_defs::output,
		output>("receiver1")
    };

    cadmium::dynamic::modeling::ICs ics_TOP = {
        cadmium::dynamic::translate::make_IC<iestream_input_defs<message_t>::out,
		receiver_defs::input>("generator","receiver1")
    };
    std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP =
    std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>(
        "TOP",
        submodels_TOP,
        iports_TOP,
        oports_TOP,
        eics_TOP,
        eocs_TOP,
        ics_TOP
    );

    /**
     * Creates a model and measures the time taken for creating this model.
     */

    auto time_elapsed = std::chrono::duration_cast<std::chrono::duration<double,
    		        std::ratio<1>>>(hclock::now() - start).count();
    cout << "Model Created. Elapsed time: " << time_elapsed << "sec" << endl;

    /**
     *  This creates a runner and measures the time taken for creating this runner.
     */

    cadmium::dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    time_elapsed = std::chrono::duration_cast<std::chrono::duration<double,
    		   std::ratio<1>>>(hclock::now() - start).count();
    cout << "Runner Created. Elapsed time: " << time_elapsed << "sec" << endl;

    /**
     * Starts the simulation and runs until 04:00:00:000
     */

    cout << "Simulation starts" << endl;
    r.run_until(NDTime("04:00:00:000"));
    auto simulation_time = std::chrono::duration_cast<std::chrono::duration<double,
    		       std::ratio<1>>>(hclock::now() - start).count();
    cout << "Simulation took:" << simulation_time << "sec" << endl;

    /**
     * calling the function to modify the existing output file
     */

    output_filter(input_file,output_file);

    struct compare *c1;

    limit_output(output_file,limit_file,c1);

    return 0;
}
