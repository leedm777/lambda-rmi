package lambda_rmi.lib

import Ice.AsyncResult

case class IceCallbackWrapper(fn: (Ice.AsyncResult) => Unit) extends Ice.Callback {
  def completed(r: AsyncResult) = fn(r)
}

object IceCallbackWrapper {
  implicit def fn2wrapper(fn: (Ice.AsyncResult) => Unit) = IceCallbackWrapper(fn)
}