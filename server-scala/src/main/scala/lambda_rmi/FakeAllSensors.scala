package lambda_rmi

import Ice.Current
import LambdaRmi.{SensorPrx, _AllSensorsDisp}

class FakeAllSensors(sensors: Array[SensorPrx]) extends _AllSensorsDisp {

  def list(c: Current) = sensors

}