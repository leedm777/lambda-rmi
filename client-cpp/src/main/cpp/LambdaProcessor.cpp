#include "FunctionalIceCallback.h"
#include "SensorProcessor.h"

struct CollectionData
{
    CollectionData() : sum(0), count(0) {}

    double sum;
    std::size_t count;
};

class LambdaProcessor : public SensorProcessor
{
public:
    LambdaProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        SensorProcessor(allSensors) {}
    void getAverageTemperatureCelsius(
        boost::function<void (double)> callback) const
    {
        std::shared_ptr<CollectionData> data(new CollectionData);
        const LambdaRmi::SensorSeq& sensorSeq = getAllSensors()->list();
        assert(sensorSeq.size() > 0);
        std::size_t expected = sensorSeq.size();
        for (LambdaRmi::SensorSeq::const_iterator i = sensorSeq.begin();
             i != sensorSeq.end();
             ++i)
        {
            LambdaRmi::SensorPrx sensor = *i;
            sensor->begin_getReading(toIce([=](const Ice::AsyncResultPtr&r)
            {
                LambdaRmi::ReadingPtr reading = sensor->end_getReading(r);
                std::cout << reading << '\n';
                data->sum += reading->temperatureCelcius;
                ++(data->count);

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
