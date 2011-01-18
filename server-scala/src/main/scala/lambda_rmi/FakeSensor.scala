package lambda_rmi

import LambdaRmi.{AMD_Sensor_getReading, _SensorDisp}
import Ice.Current


case class FakeSensor(zip: String, state: String, city: String, lat: Double, long: Double, maxResponseTimeMillis: Int) extends _SensorDisp {
  def getReading_async(cb: AMD_Sensor_getReading, current: Current) = {
  }
}