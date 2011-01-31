/*
 * lambda-rmi - a demonstration of using lambda funcitons for RMI
 *
 * Copyright (C) 2011, David M. Lee, II <leedm777@yahoo.com>
 *
 * This program is free software, distributed under the terms of
 * the BSD License. See the LICENSE.txt file at the top of the
 * source tree.
 */
#include "SensorProcessor.h"

#include <iostream>
#include <string>

#include <Ice/Ice.h>
#include <Sensor.h>

Ice::CommunicatorPtr ic;

void complete(double average)
{
    std::cout << "Average temp: " << average << '\n';

    if (ic)
    {
        ic->shutdown();
    }
}

int main(int argc, const char* argv[])
{
    std::string allSensorsPrxStr = "AllSensors -t:tcp -h localhost -p 1099";
    std::string processorName = "sync";

    switch(argc)
    {
    case 3:
        allSensorsPrxStr = argv[2];
        // fall through
    case 2:
        processorName = argv[1];
    case 1:
        // no-op
        break;
    }

    try
    {
        ic = Ice::initialize();
        LambdaRmi::AllSensorsPrx allSensors =
            LambdaRmi::AllSensorsPrx::checkedCast(
                ic->stringToProxy(allSensorsPrxStr));
        if (!allSensors)
        {
            std::clog << "Invalid proxy" << allSensorsPrxStr << "\n";
            return EXIT_FAILURE;
        }

        SensorProcessorPtr processor;
        if (processorName == "sync")
        {
            processor = newSyncProcessor(allSensors);
        }
        else if (processorName == "async")
        {
            processor = newAsyncProcessor(allSensors);
        }
        else if (processorName == "lambda")
        {
            processor = newLambdaProcessor(allSensors);
        }
        else
        {
            std::clog << "Unknown processor " << processorName << '\n';
            return 1;
        }

        processor->getAverageTemperatureCelsius(complete);
        ic->waitForShutdown();
    }
    catch (const std::exception& e)
    {
        std::clog << "Exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::clog << "Exception: " << "unknown" << '\n';
    }

}
