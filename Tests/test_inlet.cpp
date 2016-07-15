#include "lsl_cpp.h"
#include <iostream>

int main(int argc, char* argv[]) {
    
    // resolve the stream of interest & make an inlet to get data from the first result
	std::cout << "Searching for stream type Markers ..." << std::endl;
    std::vector<lsl::stream_info> results = lsl::resolve_stream("type","Markers");
	std::cout << "Found:" << std::endl;
	std::cout << results[0].as_xml() << std::endl;

	lsl::stream_inlet inlet(results[0]);
	lsl::stream_info info = inlet.info();
	std::cout << "Creating inlet for stream " << info.name() << " of type " << info.type() << std::endl;
        
    // receive data & time stamps forever (not displaying them here)
    double ts;
    while (true)
    {
		std::string sample;
        ts = inlet.pull_sample(&sample, 1);
        std::cout << ts << "\t" << sample << std::endl;
    }
    
    
    
    return 0;
}