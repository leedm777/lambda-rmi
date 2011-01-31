/*
 * lambda-rmi - a demonstration of using lambda funcitons for RMI
 *
 * Copyright (C) 2011, David M. Lee, II <leedm777@yahoo.com>
 *
 * This program is free software, distributed under the terms of
 * the BSD License. See the LICENSE.txt file at the top of the
 * source tree.
 */
#include <Ice/Ice.h>
#include <Sensor.h>

#include <functional>

/**
 * Interface for the different methods for processing the sensors.
 */
class SensorProcessor
{
public:
    SensorProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        allSensors(allSensors)
    {}
    virtual ~SensorProcessor() {}

    /**
     * Gets the average temperature form the sensors.  Average is returned
     * via callback function to allow for async implementations.
     */
    virtual void getAverageTemperatureCelsius(
        std::function<void (double)> callback) const = 0;

protected:
    const LambdaRmi::AllSensorsPrx& getAllSensors() const { return allSensors; }

private:
    LambdaRmi::AllSensorsPrx allSensors;
};

std::shared_ptr<SensorProcessor> newSyncProcessor(
    const LambdaRmi::AllSensorsPrx& allSensors);
std::shared_ptr<SensorProcessor> newAsyncProcessor(
    const LambdaRmi::AllSensorsPrx& allSensors);
std::shared_ptr<SensorProcessor> newLambdaProcessor(
    const LambdaRmi::AllSensorsPrx& allSensors);

/** Easy way to print out readings. */
inline std::ostream& operator<<(std::ostream& o, LambdaRmi::ReadingPtr reading)
{
    if (reading)
    {
        return o << "(" << reading->latitudeDegrees << ","
                 << reading->longitudeDegrees << ") - "
                 << reading->temperatureCelcius;
    }
    else
    {
        return o << "(null)";
    }
}
