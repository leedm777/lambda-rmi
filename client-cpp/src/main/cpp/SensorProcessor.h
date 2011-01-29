#include <Ice/Ice.h>
#include <Sensor.h>

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

class SensorProcessor : public Ice::Object
{
public:
    SensorProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        allSensors(allSensors)
    {}
    virtual ~SensorProcessor() {}

    virtual double getAverageTemperatureCelsius() const = 0;

protected:
    const LambdaRmi::AllSensorsPrx& getAllSensors() const { return allSensors; }

private:
    LambdaRmi::AllSensorsPrx allSensors;
};

typedef IceUtil::Handle<SensorProcessor> SensorProcessorPtr;

SensorProcessorPtr newSyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors);
SensorProcessorPtr newAsyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors);
SensorProcessorPtr newLambdaProcessor(const LambdaRmi::AllSensorsPrx& allSensors);
