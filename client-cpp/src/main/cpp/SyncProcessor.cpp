#include "SensorProcessor.h"

class SyncProcessor : public SensorProcessor
{
public:
    SyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        SensorProcessor(allSensors) {}
    void getAverageTemperatureCelsius(
        boost::function<void (double)> callback) const
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

SensorProcessorPtr newSyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors)
{
    return new SyncProcessor(allSensors);
}
