#include "Config.hpp"

#include "MainApplication.hpp"

#include "Logger.hpp"
#include "Trace.hpp"
#include "FileTraceFunction.hpp"

#include "ConfigFileReader.hpp"

//TODO REMOVE AFTER TESTING
#include "CompassSensor.hpp"
#include "LidarDistanceSensor.hpp"
#include "OdometerSensor.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

int main(int argc,
		 char *argv[])
{

	// Base::Trace::setTraceFunction( std::make_unique<Base::FileTraceFunction>("trace", "log", true));
	ConfigFileReader fileReader;

	double odomSdev, compassSdev, lidarSdev;

	if (fileReader.read(compassSdev, odomSdev, lidarSdev))
	{
		std::cout << "succesfully read config file and applied values" << std::endl;
	}
	else
	{
		std::cout << "failed to read config file" << std::endl;
	}

	try
	{
		// Call the wxWidgets main variant
		// This will actually call Application
		int result = runGUI(argc, argv);
		return result;
	}
	catch (std::exception &e)
	{
		Application::Logger::log(__PRETTY_FUNCTION__ + std::string(": ") + e.what());
		std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
	}
	catch (...)
	{
		Application::Logger::log(__PRETTY_FUNCTION__ + std::string(": unknown exception"));
		std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
	}
	return 0;
}
