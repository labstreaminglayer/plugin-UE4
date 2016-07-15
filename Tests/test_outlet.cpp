#include "lsl_cpp.h"
#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <thread>
#include <math.h>

#define NCHANNELS 1
const char *channels[] = { "CONTROL1" };
#define pi 3.14159

double g_sine_freq = 0.5;
double g_push_freq = 16;

int main(int argc, char* argv[]) {
	std::string name = "SimpleOutlet";
	std::string type = "Control";
	auto start_time = std::chrono::high_resolution_clock::now();
		
	try {
		lsl::stream_info outlet_info(name, type, NCHANNELS, g_push_freq, lsl::cf_double64, std::string(name) += type);
		// add some description fields
		lsl::xml_element info_xml = outlet_info.desc();
		lsl::xml_element channels_xml = outlet_info.desc().append_child("channels");
		for (int k = 0; k < NCHANNELS; k++)
		{
			lsl::xml_element chn = channels_xml.append_child("channel");
			chn.append_child_value("label", channels[k])
				.append_child_value("unit", "SDU")
				.append_child_value("type", "Generated");
		}
		// make a new outlet
		lsl::stream_outlet outlet(outlet_info);

		// send data forever
		std::cout << "Now sending data... " << std::endl;
		double push_counter = 0.0;
		while(true) {
			std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start_time;
			double elapsed_seconds = elapsed.count();
			if (elapsed_seconds > (push_counter / g_push_freq))
			{
				double sample[NCHANNELS];
				for (int c = 0; c < NCHANNELS; c++)
				{
					sample[c] = sin(2 * pi * g_sine_freq * elapsed_seconds);
				}
				// send the sample
				outlet.push_sample(sample);
				push_counter += 1.0;
				std::cout << "Pushed sample (ch 1 = " << sample[0] << ")" << std::endl;
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
			
		}

	} catch(std::exception &e) {
		std::cerr << "Got an exception: " << e.what() << std::endl;
	}
	std::cout << "Press any key to exit. " << std::endl; std::cin.get();
	return 0;
}
