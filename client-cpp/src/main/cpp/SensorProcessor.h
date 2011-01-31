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
#include <boost/function.hpp>

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

class SensorProcessor : public IceUtil::Shared
{
public:
    SensorProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        allSensors(allSensors)
    {}
    virtual ~SensorProcessor() {}

    virtual void getAverageTemperatureCelsius(
        boost::function<void (double)> callback) const = 0;

protected:
    const LambdaRmi::AllSensorsPrx& getAllSensors() const { return allSensors; }

private:
    LambdaRmi::AllSensorsPrx allSensors;
};

typedef IceUtil::Handle<SensorProcessor> SensorProcessorPtr;

SensorProcessorPtr newSyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors);
SensorProcessorPtr newAsyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors);
SensorProcessorPtr newLambdaProcessor(const LambdaRmi::AllSensorsPrx& allSensors);
