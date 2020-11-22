import akka.actor._
import scala.collection.mutable.ListBuffer

case class MapOutput(val entries:List[(String, Int)])
case class ReduceInput(val key:String, val entries:List[(String, Int)])
case class ReduceOutput(val key:String, val value:Int)
case class Begin()

case class FirstMessage(prop1: List[String])

case class MapCompleted()
case class ReduceCompleted()

class MainActor extends Actor with ActorLogging {
  import context._

  val mapOutputs = new ListBuffer[MapOutput]
  val reduceInputs = new ListBuffer[ReduceInput]
  val reduceOutputs = new ListBuffer[ReduceOutput]

  val MapRef = system.actorOf(Props[MapActor], name = "MapActor")

  def receive = {
    case FirstMessage(prop1) => map(0, prop1)
    case _ => log.warning("unknown")
  }

  def map(index: Int, data:List[String]) {
    data match {
      case Nil =>
      case x::xs => println("Main calls map():" + x)
		    MapRef ! x
                    map(index+1, xs)
    }
  }

  def appendMapOutput(o: MapOutput) = {
    println("MasterActor: received the response: map(" + o + ")")
    mapOutputs += o
  }

  def checkMapProgress =
    if (mapOutputs.length > 10) sender ! MapCompleted
}

/**
 * Main
 */
object MapSample extends App {
  val system = ActorSystem()

  val data = List("Hello World",
                  "Hello Scala World",
                  "Hello Java World",
                  "Hello Java Hello",
                  "Hello Scala Scala",
                  "Scala World World World",
                  "Scala World Scala World",
                  "Hello Groovy")

  // アクターの生成
  val ref = system.actorOf(Props[MainActor], name = "mainActor")
  ref ! FirstMessage(data)

  Thread.sleep(3000)
  // すべてのアクターを終了
  system.terminate()
}