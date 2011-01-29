#include "SensorProcessor.h"

#include <iostream>
#include <string>

#include <Ice/Ice.h>
#include <Sensor.h>

int main(int argc, const char* argv[])
{
    std::string allSensorsPrxStr = "AllSensors -t:tcp -h localhost -p 1099";

    switch(argc)
    {
    case 2:
        allSensorsPrxStr = argv[1];
        // fall through
    case 1:
        // no-op
        break;
    }

    Ice::CommunicatorPtr ic;
    try
    {
        ic = Ice::initialize();
        LambdaRmi::AllSensorsPrx allSensors = LambdaRmi::AllSensorsPrx::checkedCast(
            ic->stringToProxy(allSensorsPrxStr));
        if (!allSensors)
        {
            std::clog << "Invalid proxy" << allSensorsPrxStr << "\n";
            return EXIT_FAILURE;
        }


        SensorProcessorPtr processor = newSyncProcessor(allSensors);
        double avg = processor->getAverageTemperatureCelsius();
        std::cout << "Average temp: " << avg << '\n';
    }
    catch (const std::exception& e)
    {
        std::clog << "Exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::clog << "Exception: " << "unknown" << '\n';
    }

    if (ic)
    {
        ic->destroy();
    }
}
