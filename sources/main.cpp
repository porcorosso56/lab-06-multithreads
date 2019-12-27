#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <picosha2.h>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

void init()
{
    logging::register_simple_formatter_factory<
        logging::trivial::severity_level,
        char
    >("Severity");
    static const std::string format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%";

    auto sinkFile = logging::add_file_log(
        keywords::file_name = "logs/log_%N.log",
        keywords::rotation_size = 128 * 1024 * 1024,
        keywords::format = format
    );
    sinkFile->set_filter(
        logging::trivial::severity >= logging::trivial::trace
    );  

    auto sinkConsole = logging::add_console_log(
        std::cout,
        keywords::format = format
    );
    sinkConsole->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );     
	
	logging::add_common_attributes();
}

bool isEndingOfHashCorrect(std::string str) {
	const std::string check = "0000";
	if ( str.size() - check.size() > 0 && str.substr(str.size() - check.size(), str.size()) == check )
		return true;
	return false;
}

void generateHash() {
	srand(time(NULL));
	while(true)
	{
		std::string randomStr = std::to_string(rand());
		std::string hash = picosha2::hash256_hex_string(randomStr);
		std::stringstream ss;
		if(isEndingOfHashCorrect(hash)) {
			ss << "Founded correct hash = " << hash << ", from (" << randomStr << ")";
			BOOST_LOG_TRIVIAL(info) << ss.str();
		} else {
			ss <<"Current hash = " << hash << ", from (" << randomStr << ")";
			BOOST_LOG_TRIVIAL(trace) << ss.str();
		}
	}
}

int main(int argc, char* argv[]) {
	init();

	int threadAmount;
	if (argc >= 2) {
		threadAmount = atoi(argv[1]);
	} else {
		threadAmount = std::thread::hardware_concurrency();
	}

	std::vector<std::thread> threads;
	threads.reserve(threadAmount);
	for (int i = 0; i < threadAmount; i++) {
		threads.emplace_back(generateHash);
	}	
	for (std::thread &thread : threads) {
		thread.join();
	}
}
