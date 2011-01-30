package lambda_rmi

import io.Source
import LambdaRmi.SensorPrxHelper

class InvalidConfigurationException(cfg: Any) extends Exception(cfg.toString)

object App {

  val Rand = new util.Random

  val SensorCount = System.getProperty("lambda-rmi.sensor-count", "10").toInt
  val MaxResponseTimeMillis = 3000


  def main(args: Array[String]) {
    val zipCodesStream = getClass.getClassLoader.getResourceAsStream("zips.txt")
    assert(zipCodesStream != null)

    val zipCodes = Rand.shuffle(
      Source.fromInputStream(zipCodesStream).getLines.toSeq
        .map(_.split(",").toList.map(_.replaceAll("\"", ""))))

    val sensors = zipCodes
      .take(SensorCount)
      .map {
      case _ :: zip :: state :: city :: lat :: long :: _ :: _ :: Nil =>
        FakeSensor(zip, state, city, lat.toDouble, long.toDouble, MaxResponseTimeMillis)
      case wtf =>
        throw new InvalidConfigurationException(wtf)
    }

    val communicator = Ice.Util.initialize(args);
    val adapter = communicator.createObjectAdapterWithEndpoints(
      "LambdaAdapter", "default -h localhost -p 1099")

    val sensorProxies = sensors.map((v: Ice.Object) =>
      SensorPrxHelper.uncheckedCast(adapter.addWithUUID(v)))

    val fakeSensors = new FakeAllSensors(sensorProxies.toArray)
    val proxy = adapter.add(fakeSensors, communicator.stringToIdentity("AllSensors"))
    println(proxy)
    adapter.activate()

    /*
    {
      import LambdaRmi.AllSensorsPrxHelper;
      AllSensorsPrxHelper.checkedCast(communicator.stringToProxy(proxy.toString)).
        getSensors.map(_.ice_collocationOptimized(false)).
        map(SensorPrxHelper.uncheckedCast(_)).
        foreach {
        p =>
          val r = p.getReading
          printf("[%f, %f] = %f C\n", r.latitudeDegrees, r.longitudeDegrees, r.temperatureCelcius)
      }
    }
    */



    communicator.waitForShutdown
  }

}