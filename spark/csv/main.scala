import org.apache.spark.SparkContext

object Main {
  def main(args: Array[String]) {

      val sc = new SparkContext("local[*]", "App") 

      val rddFromFile = sc.textFile("random_data.txt")
      val rdd = rddFromFile.map(f=>{
      	  f.split(",")
      })

      rdd.foreach(f=>{
      println("Col1: "+ f(0) + ",Col2: "+ f(1))
      })

      sc.stop()
  }
}

