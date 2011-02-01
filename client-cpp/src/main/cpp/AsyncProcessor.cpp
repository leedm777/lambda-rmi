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

#include <boost/function.hpp>

/**
 * Shared state between callback functions.
 */
struct CollectionData : public Ice::LocalObject
{
    CollectionData(boost::function<void (double)> callback,
        std::size_t expected) :
        callback(callback), expected(expected), sum(0), count(0)
    {
    }

    boost::function<void (double)> callback;
    std::size_t expected;
    double sum;
    int count;

    void finished(const Ice::AsyncResultPtr& r)
    {
        LambdaRmi::SensorPrx sensor =
            LambdaRmi::SensorPrx::uncheckedCast(r->getProxy());
        LambdaRmi::ReadingPtr reading = sensor->end_getReading(r);
        std::cout << reading << '\n';

	sum += reading->temperatureCelcius;
	++count;

        if (expected == count)
        {
            callback(sum / count);
        }
    }
};
typedef IceUtil::Handle<CollectionData> CollectionDataPtr;

/**
 * SensorProcessor which asynchronously queries all sensors at once.
 */
class AsyncProcessor : public SensorProcessor
{
public:
    AsyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        SensorProcessor(allSensors)
    {}
    void getAverageTemperatureCelsius(
        std::function<void (double)> callback) const
    {
        LambdaRmi::SensorSeq sensorSeq = getAllSensors()->list();
        assert(sensorSeq.size() > 0);
        CollectionDataPtr collectionData =
            new CollectionData(callback, sensorSeq.size());
        Ice::CallbackPtr iceCallback =
            Ice::newCallback(collectionData, &CollectionData::finished);

        for (LambdaRmi::SensorSeq::iterator i = sensorSeq.begin();
             i != sensorSeq.end();
             ++i)
        {
            (*i)->begin_getReading(iceCallback, collectionData);
        }
    }
};

std::shared_ptr<SensorProcessor> newAsyncProcessor(
    const LambdaRmi::AllSensorsPrx& allSensors)
{
    return std::shared_ptr<SensorProcessor>(new AsyncProcessor(allSensors));
}
