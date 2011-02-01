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

#include <memory>

/**
 * SensorProcessor which synchronously walks through the given sensors, one at a
 * time.
 */
class SyncProcessor : public SensorProcessor
{
public:
    SyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        SensorProcessor(allSensors) {}
    void getAverageTemperatureCelsius(
        std::function<void (double)> callback) const
    {
        double sum = 0;
        std::size_t count = 0;
        LambdaRmi::SensorSeq sensorSeq = getAllSensors()->list();
        for (LambdaRmi::SensorSeq::iterator i = sensorSeq.begin();
             i != sensorSeq.end();
             ++i)
        {
            std::cout << (*i)->getReading() << '\n';
            sum += (*i)->getReading()->temperatureCelcius;
            ++count;
        }

        if (count != 0)
        {
            callback(sum / count);
        }
        else
        {
            callback(0);
        }
    }
};

std::shared_ptr<SensorProcessor> newSyncProcessor(
    const LambdaRmi::AllSensorsPrx& allSensors)
{
    return std::shared_ptr<SensorProcessor>(new SyncProcessor(allSensors));
}
