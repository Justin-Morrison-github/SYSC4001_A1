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

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        // Use activity to determine if I/O or not
        // If I/O:
        // Use duration_intr as index into vectors to find address of ISR
        // Use duration_intr as index into delays to find context save time
        // Pass the current time, duration_intr as int_num, context save time, and vectors to intr_boilerplate()

        // If not IO
        // increment current_time by duration_intr

        // pass execution string return from intr_boilerplate() to write_output()
        // update current time

        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
