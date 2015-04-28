#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include "myahrs_plus.hpp"

static const int BAUDRATE = 115200;
static const char* DIVIDER = "1";  // 100 Hz

double processFuzzyInput(double);	//

void handle_error(const char* error_msg)
{
    fprintf(stderr, "ERROR: %s\n", error_msg);
    exit(1);
}

void wait_for_user_input()
{
    printf("\npress enter key to quit.\n");
    char c = getchar();
    exit(1);
}

void usage(const char* prog)
{

    const char* dev_name_example = "/dev/ttyACM0 /dev/ttyACM1 ...";

    printf("\nusage : %s \"example_number\" \"serial port list\"  \n\t ex) $ %s 1 %s\n\n", prog, prog, dev_name_example);
}

void read_serial_devices_from_command_line(std::vector<std::string>& args, std::vector<std::string>& serial_device_list)
{
    for(size_t i=1; i<args.size(); i++) {
        serial_device_list.push_back(args[i]);
    }
}

/******************************************************************************************************************************
 *
 *  EXAMPLE 1
 *
 ******************************************************************************************************************************/


void ex1_synchronous_read_ascii(const char* serial_device, int baudrate)
{

    using namespace WithRobot;

    printf("\n### %s() ###\n", __FUNCTION__);

    MyAhrsPlus sensor;
    SensorData sensor_data;
    uint32_t sample_count = 0;

    /*
     * 	start communication with the myAHRS+.
     */
    if(sensor.start(serial_device, baudrate) == false) {
        handle_error("start() returns false");
    }

    /*
     *  set ascii output format
     *   - select euler angle
     */
    if(sensor.cmd_ascii_data_format("RPY") == false) {
        handle_error("cmd_ascii_data_format() returns false");
    }

    /*
     *  set divider
     *   - output rate(Hz) = max_rate/divider
     */
    if(sensor.cmd_divider(DIVIDER) ==false) {
        handle_error("cmd_divider() returns false");
    }

    /*
     *  set transfer mode
     *   - AC : ASCII Message & Continuous mode
     */
    if(sensor.cmd_mode("AC") ==false) {
        handle_error("cmd_mode() returns false");
    }

    

    while(sample_count < 3000) {
        if(sensor.wait_data() == true) { // waiting for new data
        	// read counter
            sample_count = sensor.get_sample_count();

            // copy sensor data
            sensor.get_data(sensor_data);

            // print euler angle
            EulerAngle& e = sensor_data.euler_angle;
            printf("%04d) input(roll) = %.2f, Output = %.2f\n", sample_count, e.roll, processFuzzyInput(e.roll) );
        }
    }

    /*
     * 	stop communication
     */
    sensor.stop();

    printf("END OF TEST(%s)\n\n", __FUNCTION__);
}

/******************************************************************************************************************************
 *
 *
 *
 ******************************************************************************************************************************/

double processFuzzyInput(double input){

    using namespace fl;

Engine* engine = new Engine;
engine->setName("roll");

InputVariable* inputVariable = new InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("roll");
inputVariable->setRange(-180.000, 180.000);
inputVariable->addTerm(new ZShape("unstable_left", -92.600, -0.476));
inputVariable->addTerm(new Gaussian("stable", 0.000, 30.330));
inputVariable->addTerm(new SShape("unstable_right", 2.380, 88.095));
engine->addInputVariable(inputVariable);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("stability");
outputVariable->setRange(0.000, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(new Maximum);
outputVariable->setDefuzzifier(new Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(new ZShape("safe", 0.040, 0.993));
outputVariable->addTerm(new SShape("danger", 0.012, 0.960));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setActivation(new Minimum);
ruleBlock->addRule(fl::Rule::parse("if roll is unstable_left then stability is danger", engine));
ruleBlock->addRule(fl::Rule::parse("if roll is stable then stability is safe", engine));
ruleBlock->addRule(fl::Rule::parse("if roll is unstable_right then stability is danger", engine));
engine->addRuleBlock(ruleBlock);


    engine->configure("", "", "Minimum", "Maximum", "Centroid");

    std::string status;
    if (not engine->isReady(&status))
        throw Exception("Engine not ready. "
            "The following errors were encountered:\n" + status, FL_AT);
	
	
       	inputVariable->setInputValue(input);
       	 engine->process();
	return outputVariable->getOutputValue();
     
}



int main(int argc, char* argv[]) {

	
    std::vector<std::string> args;

    for(int i=0; i<argc; i++) {
        args.push_back(std::string(argv[i]));
    }

    if(argc < 3) {
        printf("ERROR. need more arguments\n");
        usage(args[0].c_str());
        exit(1);
    }

    int example_id = atoi(args[1].c_str());

    std::vector<std::string> serial_device_list;
    for(size_t i=2; i<args.size(); i++) {
        serial_device_list.push_back(args[i]);
    }

    for(size_t i=0; i<serial_device_list.size(); i++) {
        printf(" - serial device(%d) : %s\n", i, serial_device_list[i].c_str());
    }

    switch(example_id){

    case 1:
    	ex1_synchronous_read_ascii(serial_device_list[0].c_str(), BAUDRATE);
    	break;
    case 2:
    	 break;
    case 3:
    	break;
    case 4:
    	break;
    case 5:
    	break;
    case 6:
    	break;
    default:
    	handle_error("Invalid example id");
    	break;
    }

    wait_for_user_input();

    return 0;
}


