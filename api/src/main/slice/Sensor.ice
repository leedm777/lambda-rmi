module LambdaRmi
{
    /** Individual reading from the sensor */
    class Reading
    {
        double latitudeDegrees;
        double longitudeDegrees;
        double temperatureCelcius;
    };

    /** Individual sensor */
    interface Sensor
    {
        ["amd"] Reading getReading();
    };

    sequence<Sensor*> SensorSeq;

    /** API for a collection of Sensors */
    interface AllSensors
    {
        SensorSeq list();
    };
};
