package lambda_rmi.lib

import net.liftweb.http.CometActor
import lambda_rmi.comet.Pin
import actors.Actor
import actors.Actor._
import IceCallbackWrapper._
import LambdaRmi.Reading

case class ReadSync(page: CometActor)

case class ReadAsync(page: CometActor)

object SensorReader {
  val AllSensorsPrxStr =    System.getProperty("lambda-rmi.all-sensors-proxy",
      "AllSensors -t:tcp -h localhost -p 1099");

  val ic = Ice.Util.initialize()
  val allSensors: LambdaRmi.AllSensorsPrx =
    LambdaRmi.AllSensorsPrxHelper.checkedCast(
      ic.stringToProxy(AllSensorsPrxStr));

  val actor = Actor.actor {
    def reading2pin(r: Reading) = Pin(r.latitudeDegrees, r.longitudeDegrees, r.temperatureCelcius)
    while (true) {
      receive {

        case ReadSync(page) =>
          for (sensor <- allSensors.list) {
            val reading = sensor.getReading
            page ! reading2pin(reading)
          }

        case ReadAsync(page) =>
          for (sensor <- allSensors.list) {
            sensor.begin_getReading {
              r: Ice.AsyncResult =>
                val reading = sensor.end_getReading(r)
                page ! reading2pin(reading)
            }
          }
      }
    }
  }
}