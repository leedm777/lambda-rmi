package lambda_rmi.comet

import net.liftweb.http.{SHtml, CometActor}
import net.liftweb.http.js.JsCmds._
import net.liftweb.http.js.JE._
import lambda_rmi.lib._

case class Pin(lat: Double, lng: Double, tempCelcius: Double)

class MapUpdater extends CometActor {


  override def highPriority = {
    case Pin(lat, lng, tempCelcius) =>
      implicit def double2jsexp(d: Double) = Str(d.toString)
      partialUpdate(Call("dropPin", lat, lng, tempCelcius))
  }


  override def render = {
    <div>
      {SHtml.ajaxButton("Read synchronously", () => {
      SensorReader.actor ! ReadSync(this); Noop
    })}<br/>
      {SHtml.ajaxButton("Read asynchronously", () => {
      SensorReader.actor ! ReadAsync(this); Noop
    })}
    </div>
  }
}