/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include <interrupts.hpp>

int main(int argc, char **argv)
{

    // vectors is a C++ std::vector of strings that contain the address of the ISR
    // delays  is a C++ std::vector of ints that contain the delays of each device
    // the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;     //!< string to store single line of trace file
    std::string execution; //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    // Track current time
    int current_time = 0;
    const int CONTEXT_SWITCH_TIME = 10;
    const int IRET_RETURN_TIME = 30;
    const int ISR_EXECUTE_TIME = 40;
    const int DATA_TRANSFER_TIME = 40;
    char buffer[64]; // Buffer to store strings
    int delay;

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        // Use activity to determine if I/O or not
        if (activity == "CPU")
        {
            sprintf(buffer, "%d, %d, CPU Burst\n", current_time, duration_intr);
            execution += std::string(buffer);
            current_time += duration_intr;
        }
        else
        {
            // SYSCALL or END_IO: Device number=duration_intr, use as index into vectors to find address of ISR and into delays to find delay time
            auto address = vectors.at(duration_intr);
            auto [execution_string, new_time] = intr_boilerplate(current_time, duration_intr, CONTEXT_SWITCH_TIME, vectors);
            execution += execution_string;
            current_time = new_time;
            delay = delays.at(duration_intr);

            if (activity == "SYSCALL")
            {
                execution += std::to_string(current_time) + ", " + std::to_string(ISR_EXECUTE_TIME) + ", SYSCALL: run the ISR " + std::to_string(duration_intr) + "\n";
                current_time += ISR_EXECUTE_TIME;
                execution += std::to_string(current_time) + ", " + std::to_string(DATA_TRANSFER_TIME) + ", transfer data from device to memory\n";
                current_time += DATA_TRANSFER_TIME;
                delay -= DATA_TRANSFER_TIME; // Remove the DATA_TRANSFER_TIME from like the example does
                execution += std::to_string(current_time) + ", " + std::to_string(delay - ISR_EXECUTE_TIME) + ", check for errors\n";
            }
            else if (activity == "END_IO")
            {
                execution += std::to_string(current_time) + ", " + std::to_string(ISR_EXECUTE_TIME) + ", " + "END_IO: run the ISR " + std::to_string(duration_intr) + "\n";
                execution += std::to_string(current_time) + ", " + std::to_string(delay - ISR_EXECUTE_TIME) + ", check device status\n";
            }
            current_time += delay - ISR_EXECUTE_TIME; // Add overhead for ISR execution time for testing

            // Execute IRET
            sprintf(buffer, "%d, %d, IRET\n", current_time, IRET_RETURN_TIME);
            execution += std::string(buffer);
            current_time += IRET_RETURN_TIME;

            // Restore context
            sprintf(buffer, "%d, %d, Restore context\n", current_time, CONTEXT_SWITCH_TIME);
            execution += std::string(buffer);
            current_time += CONTEXT_SWITCH_TIME;

            // Switch back to user
            sprintf(buffer, "%d, %d, Switch to user mode\n", current_time, 1);
            execution += std::string(buffer);
            current_time += 1;
        }

        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
