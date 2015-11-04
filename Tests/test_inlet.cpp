#include "lsl_cpp.h"
#include <iostream>

int main(int argc, char* argv[]) {
    
    // resolve the stream of interest & make an inlet to get data from the first result
    std::vector<lsl::stream_info> results = lsl::resolve_stream("type","Events");
    lsl::stream_inlet inlet(results[0]);
    lsl::stream_info info = inlet.info();
    std::cout << "Found stream name " << info.name() << " of type " << info.type() << std::endl;
    
    // receive data & time stamps forever (not displaying them here)
    float sample[1];
    double ts;
    while (true)
    {
        ts = inlet.pull_sample(&sample[0], 1);
        std::cout << ts << "\t" << sample[0] << std::endl;
    }
    
    
    
    return 0;
}