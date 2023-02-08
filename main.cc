#include <boost/program_options.hpp>
#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>

// #include "alsa.h"

struct options {
    int period_size_frames;
    int num_periods;
    int sampling_rate_hz;
    std::string pcm_device_name;
    std::string sample_format;
    int input_channels;
    int output_channels;
    int priority;
    int buffer_size_frames;
    int sizeof_sample;
    int sample_size;
    int verbose;
    int show_header;
    int sleep_percent;
    int busy_sleep_us;
    int prefault_heap_size_mb;
    int processing_buffer_frames;
};

int main(int argc, char *argv[]) {
    options o;

    namespace po = boost::program_options;

    po::options_description options_desc("Options");
    options_desc.add_options()
        ("help,h", "produce this help message")
        ("verbose,v", po::value<int>(&o.verbose)->default_value(0), "whether to be a little more verbose")
        ("period-size,p", po::value<int>(&o.period_size_frames)->default_value(1024), "period size (audio frames)")
        ("number-of-periods,n", po::value<int>(&o.num_periods)->default_value(2), "number of periods")
        ("rate,r", po::value<int>(&o.sampling_rate_hz)->default_value(48000), "sampling rate (hz)")
        ("pcm-device-name,d", po::value<std::string>(&o.pcm_device_name)->default_value("default"), "the ALSA pcm device name string")
        ("input-channels,i", po::value<int>(&o.input_channels)->default_value(2), "the number of input channels")
        ("output-channels,o", po::value<int>(&o.output_channels)->default_value(2), "the number of output channels")
        ("priority,P", po::value<int>(&o.priority)->default_value(70), "SCHED_FIFO priority")
        ("sample-size,s", po::value<int>(&o.sample_size)->default_value(1000), "the number of samples to collect for stats (might be less due how to alsa works)")
        ("sample-format,f", po::value<std::string>(&o.sample_format)->default_value("S32LE"), "the sample format. Available formats: S16LE, S32LE")
        ("show-header,e", po::value<int>(&o.show_header)->default_value(1), "whether to show a header in the output table")
        ("busy,b", po::value<int>(&o.busy_sleep_us)->default_value(1), "the number of microseconds to sleep everytime when nothing was done")
        ("prefault-heap-size,a", po::value<int>(&o.prefault_heap_size_mb)->default_value(100), "the number of megabytes of heap space to prefault")
        ("processing-buffer-size,c", po::value<int>(&o.processing_buffer_frames)->default_value(-1), "the processing buffer size (audio frames)")
        ("load,l", po::value<int>(&o.sleep_percent)->default_value(0), "the percentage of a period to sleep after reading a period")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options_desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << options_desc << "\n";
        exit(EXIT_SUCCESS);
    }

    while(true) {
        char * line = readline("> ");
        if (line == 0) {
            break;
        }

        std::string line_str = line;

        if (line_str == "") {
            continue;
        }

        if (line_str == "exit") {
            break;
        }
        add_history(line);
    }
}
