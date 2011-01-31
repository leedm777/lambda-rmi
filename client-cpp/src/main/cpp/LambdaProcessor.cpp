/*
 * lambda-rmi - a demonstration of using lambda funcitons for RMI
 *
 * Copyright (C) 2011, David M. Lee, II <leedm777@yahoo.com>
 *
 * This program is free software, distributed under the terms of
 * the BSD License. See the LICENSE.txt file at the top of the
 * source tree.
 */
#include "FunctionalIceCallback.h"
#include "SensorProcessor.h"

/**
 * Mutable shared state between collection lambdas.  Not that immutable state
 * is automagically captured by the lambda function itself.
 */
struct CollectionData
{
    CollectionData() : sum(0), count(0) {}

    double sum;
    std::size_t count;
};

/**
 * SensorProcessor which asynchronously queries all sensors at once, using
 * function literals to make the code awesome.
 */
class LambdaProcessor : public SensorProcessor
{
public:
    LambdaProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        SensorProcessor(allSensors) {}

    void getAverageTemperatureCelsius(
        std::function<void (double)> callback) const
    {
        // collection data must live past the end of this function, since it's
        // accessed asynchronously by the Ice callbacks.  So, allocate on the
        // heap, using a shared_ptr to automagically clean up.
        std::shared_ptr<CollectionData> data(new CollectionData);
        const LambdaRmi::SensorSeq& sensorSeq = getAllSensors()->list();
        assert(sensorSeq.size() > 0);
        // capture the size of the sensor list, so that it's accessible from
        // the lambda without copying the whole SensorSeq
        std::size_t expected = sensorSeq.size();

        for (LambdaRmi::SensorSeq::const_iterator i = sensorSeq.begin();
             i != sensorSeq.end();
             ++i)
        {
            // capture the sensor proxy for use in the lambda
            LambdaRmi::SensorPrx sensor = *i;
            // [=] indicates a lambda function which captures local variables by
            // copy.
            sensor->begin_getReading(toIce([=](const Ice::AsyncResultPtr&r)
            {
                // this is executed as the callback on the Ice thread

                // complete the execution to get the return value
                try
                {
                    LambdaRmi::ReadingPtr reading = sensor->end_getReading(r);
                    std::cout << reading << '\n';
                    data->sum += reading->temperatureCelcius;
                } catch (...) { /* ignore */ }
                ++(data->count);

                // when we've received all our expected results, pass the average
                // back to the callback
                if (data->count == expected)
                {
                    callback(data->sum / data->count);
                }
            }));
        }
    }
};

SensorProcessorPtr newLambdaProcessor(const LambdaRmi::AllSensorsPrx& allSensors)
{
    return new LambdaProcessor(allSensors);
}
