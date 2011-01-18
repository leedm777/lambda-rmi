package lambda_rmi

import Ice.Current
import LambdaRmi.{SensorPrx, _SensorsDisp}

class FakeSensors(sensors: Array[SensorPrx]) extends _SensorsDisp {

  def getSensors(c: Current) = sensors

}