import sbt._

class LambdaRmiProject(info: ProjectInfo) extends DefaultProject(info) with AkkaProject {
  val mavenLocal = "Local Maven Repository" at
    "file://" + Path.userHome + "/.m2/repository"

  val akka = "Akka Maven Repository" at "http://akka.io/repository/"

  val akkaStm = akkaModule("stm")
  val akkaTypedActor = akkaModule("typed-actor")


  override def mainClass = Some("lambda_rmi.App")

  override def libraryDependencies = Set(
    "com.github.leedm777.lambda-rmi" % "api" % "0.0.1-SNAPSHOT" % "compile->default",
    "org.scalatest" % "scalatest" % "1.2" % "test->default"
  ) ++ super.libraryDependencies
}
