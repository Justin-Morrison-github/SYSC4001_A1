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
    const int CONTEXT_SAVE_TIME = 10;
    const int IRET_RETURN_TIME = 1;
    const int ISR_EXECUTE_TIME = 40;
    char buffer[64]; // Buffer to store strings

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        // std::cout << activity << " " << duration_intr << std::endl;

        // Use activity to determine if I/O or not
        if (activity == "CPU")
        {
            // If not IO: increment current_time by duration_intr
            std::cout << current_time << ", " << duration_intr << ", CPU Burst" << std::endl;
            sprintf(buffer, "%d, %d, CPU Burst\n", current_time, duration_intr);
            execution += std::string(buffer);
            current_time += duration_intr;
        }
        else
        {
            // If I/O: Device number=duration_intr, use as index into vectors to find address of ISR and into delays to find context save time
            auto address = vectors.at(duration_intr);
            auto [execution_string, new_time] = intr_boilerplate(current_time, duration_intr, CONTEXT_SAVE_TIME, vectors);
            execution += execution_string;
            current_time = new_time;

            // Execute ISR
            sprintf(buffer, "%d, %d, Execute ISR at address %d\n", current_time, ISR_EXECUTE_TIME, duration_intr);
            execution += std::string(buffer);
            current_time += ISR_EXECUTE_TIME;

            // Execute IRET
            sprintf(buffer, "%d, %d, IRET\n", current_time, IRET_RETURN_TIME);
            execution += std::string(buffer);
            current_time += IRET_RETURN_TIME;
        }

        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
