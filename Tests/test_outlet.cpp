#include "lsl_cpp.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define NCHANNELS 1

int main(int argc, char* argv[]) {
	std::string name = "TestOutlet";
	std::string type = "ControlSignal";
    
    int flip_flop = 1;
	
	try {

		// make a new stream_info (100 Hz)
		lsl::stream_info info(name, type, NCHANNELS, 100, lsl::cf_float32, std::string(name) += type);

		// make a new outlet
		lsl::stream_outlet outlet(info);

		// send data forever
		std::cout << "Now sending data... " << std::endl;
		double starttime = ((double)clock())/CLOCKS_PER_SEC;
		for(unsigned t=0;;t++) {
			
			// wait a bit and create random data
			while (((double)clock())/CLOCKS_PER_SEC < starttime + t*0.01);
			float sample[NCHANNELS];
			for (int c = 0; c < NCHANNELS; c++)
			{
                flip_flop = fmod(((double)clock())/CLOCKS_PER_SEC, 4) >= 2.0;
                
				// Random value between -0.5 and +0.5 or between +0.5 and +1.5
				sample[c] = ((float)rand() / (float)RAND_MAX) - 0.5 + flip_flop;
			}
			
			// send the sample
			outlet.push_sample(sample);
			std::cout << "Pushed sample (ch 1 = " << sample[0] << ")" << std::endl;
		}

	} catch(std::exception &e) {
		std::cerr << "Got an exception: " << e.what() << std::endl;
	}
	std::cout << "Press any key to exit. " << std::endl; std::cin.get();
	return 0;
}
