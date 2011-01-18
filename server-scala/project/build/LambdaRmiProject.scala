import sbt._

class LambdaRmiProject(info: ProjectInfo) extends DefaultProject(info) {
  val mavenLocal = "Local Maven Repository" at
    "file://" + Path.userHome + "/.m2/repository"

  override def libraryDependencies = Set(
    "com.github.leedm777.lambda-rmi" % "api" % "0.0.1-SNAPSHOT" % "compile->default",
    "org.scalatest" % "scalatest" % "1.2" % "test->default"
  ) ++ super.libraryDependencies
}