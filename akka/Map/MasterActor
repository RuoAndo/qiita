// package chap09.program06
// import scala.actors._
// import scala.actors.Actor._

import scala.collection.mutable.ListBuffer
import akka.actor.Actor
import akka.actor.{ActorSystem,Props}

case class MapCompleted()
case class ReduceCompleted()
class MasterActor(mapActors: Array[Actor],
                  reduceActors:Array[Actor],
                  data: List[String]) extends Actor{
  val mapOutputs = new ListBuffer[MapOutput]
  val reduceInputs = new ListBuffer[ReduceInput]
  val reduceOutputs = new ListBuffer[ReduceOutput]

  def act() {
    println("MasterActor: Starting...")
    loop {
      react {
        case Begin => map(0, data)
        case mo: MapOutput => appendMapOutput(mo); checkMapProgress
        case MapCompleted => reduce(0, split(sort(mapOutputs.toList)))
        case ro: ReduceOutput => appendReduceOutput(ro); checkReduceProgress
        case ReduceCompleted => printResult; System.exit(0)
        case _ => println("MasterActor: ???")
      }
    }
  }

  def map(index: Int, data:List[String]) {
    data match {
      case Nil =>
      case x::xs => mapActors(index % mapActors.length) ! x
                    map(index+1, xs)
    }
  }

  def reduce(index: Int, data:List[ReduceInput]) {
    data match {
      case Nil =>
      case x::xs => reduceActors(index % reduceActors.length) ! x
                    reduce(index+1, xs)
    }
  }

  def sort(mapOutputs:List[MapOutput]) = {
    val words = new ListBuffer[(String, Int)]
    mapOutputs.foreach(words ++= _.entries)
    words.toList.sortWith(_._1 < _._1)
  }

  def split(data:List[(String, Int)]): List[ReduceInput] = {
    data match {
      case Nil => reduceInputs.toList
      case x::xs => {
        var spannedXs = xs.span(_._1 == x._1)
        reduceInputs += createReduceInput(x, spannedXs._1)
        split(spannedXs._2)
      }
    }
  }

  def createReduceInput(head: (String, Int),
                        remainder: List[(String, Int)]) = {
    val entries = new ListBuffer[(String, Int)] += head
    entries ++= remainder
    val reduceInput = ReduceInput(head._1, entries.toList)
    println("MasterActor: created: " + reduceInput)
    reduceInput
  }

  def printResult {
    println("******************** Result ********************")
    println("<word> : <count>")
    for (o <- reduceOutputs) printf("%-10s:%8d\n", o.key, o.value)
    println("************************************************")
  }

  def appendMapOutput(o: MapOutput) = {
    println("MasterActor: received the response: map(" + o + ")")
    mapOutputs += o
  }

  def appendReduceOutput(o: ReduceOutput) = {
    println("MasterActor: received the response: reduce(" + o + ")")
    reduceOutputs += o
  }

def checkMapProgress =
  if (mapOutputs.length == data.length) this ! MapCompleted

def checkReduceProgress =
  if (reduceInputs.length == reduceOutputs.length) this ! ReduceCompleted
}