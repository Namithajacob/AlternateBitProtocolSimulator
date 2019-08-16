#define SUBNET_OUTPUT "test/data/subnet/subnet_test_output.txt"
#define SUBNET_INPUT "test/data/subnet/subnet_input_test.txt"

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


#include "../../../lib/vendor/include/NDTime.hpp"
#include "../../../lib/vendor/include/iestream.hpp"

#include "../../../include/subnet_cadmium.hpp"

using namespace std;

using hclock=chrono::high_resolution_clock;
using TIME = NDTime;


/***** SETING INPUT PORTS FOR COUPLEDs *****/
struct input_in : public cadmium::in_port<message_t>{};

/***** SETING OUTPUT PORTS FOR COUPLEDs *****/
struct output_out: public cadmium::out_port<message_t>{};

/********************************************/
/****** APPLICATION GENERATOR *******************/
/********************************************/
template<typename T>
class ApplicationGen : public iestream_input<message_t,T> {
    public:
    ApplicationGen() = default;
    ApplicationGen(const char* file_path) : iestream_input<message_t,
        T>(file_path) {}
};


int main(){

    auto start = hclock::now(); //to measure simulation execution time

/*************** Loggers *******************/
    static std::ofstream output_data_file(SUBNET_OUTPUT);
    struct oss_sink_provider{
        static std::ostream& sink(){          
            return output_data_file;
        }
    };

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
    using log_all=cadmium::logger::multilogger<info, debug, state, log_messages,
    	          routing,global_time, local_time>;

    using logger_top=cadmium::logger::multilogger<log_messages, global_time>;


/*******************************************/



/********************************************/
/****** APPLICATION GENERATOR *******************/
/********************************************/
    string input_data = SUBNET_INPUT;
    const char * p_input_data = input_data.c_str();

    std::shared_ptr<cadmium::dynamic::modeling::model> generator =
        cadmium::dynamic::translate::make_dynamic_atomic_model<ApplicationGen,
		TIME, const char* >("generator" , std::move(p_input_data));


/********************************************/
/****** SUBNET *******************/
/********************************************/

    std::shared_ptr<cadmium::dynamic::modeling::model> subnet1 =
        cadmium::dynamic::translate::make_dynamic_atomic_model<Subnet,
		TIME>("subnet1");


/************************/
/*******TOP MODEL********/
/************************/
    cadmium::dynamic::modeling::Ports iports_TOP = {};
    cadmium::dynamic::modeling::Ports oports_TOP = {typeid(output_out)};
    cadmium::dynamic::modeling::Models submodels_TOP = {generator, subnet1};
    cadmium::dynamic::modeling::EICs eics_TOP = {};
    cadmium::dynamic::modeling::EOCs eocs_TOP = {
        cadmium::dynamic::translate::make_EOC<subnet_defs::output,
		output_out>("subnet1")
    };
    cadmium::dynamic::modeling::ICs ics_TOP = {
        cadmium::dynamic::translate::make_IC<iestream_input_defs<message_t>::out,
		subnet_defs::input>("generator","subnet1")
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

///****************////

    auto time_elapsed = std::chrono::duration_cast<std::chrono::duration<double,
                    std::ratio<1>>>(hclock::now() - start).count();
    cout << "Model Created. Elapsed time: " << time_elapsed << "sec" << endl;
    
    cadmium::dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    time_elapsed = std::chrono::duration_cast<std::chrono::duration<double,
    		   std::ratio<1>>>(hclock::now() - start).count();
    cout << "Runner Created. Elapsed time: " << time_elapsed << "sec" << endl;

    cout << "Simulation starts" << endl;

    r.run_until(NDTime("04:00:00:000"));
    auto simulation_time = std::chrono::duration_cast<std::chrono::duration<double,
                   std::ratio<1>>>(hclock::now() - start).count();
    cout << "Simulation took:" << simulation_time << "sec" << endl;
    return 0;
}
