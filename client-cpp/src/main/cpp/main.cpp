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

/**
 * Communicator for the application.  This is a quick hack, so I'll let the
 * global variable slide.  This time...
 */
namespace { Ice::CommunicatorPtr ic; }

/** Completion callback for the sensor processor */
void complete(double average)
{
    std::cout << "Average temp: " << average << '\n';

    if (ic)
    {
        ic->shutdown();
    }
}

/**
 * A simple command line application that contacts the sensor server, prints the
 * temp from all sensors, then prints the average.
 */
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
        // Ice init
        ic = Ice::initialize();

        // build the proxy for the AllSensors object
        LambdaRmi::AllSensorsPrx allSensors =
            LambdaRmi::AllSensorsPrx::checkedCast(
                ic->stringToProxy(allSensorsPrxStr));
        if (!allSensors)
        {
            std::clog << "Invalid proxy" << allSensorsPrxStr << "\n";
            return EXIT_FAILURE;
        }

        // create the processor
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
            return EXIT_FAILURE;
        }

        // process
        processor->getAverageTemperatureCelsius(complete);

        // wait for the completion callback to shut us down
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
