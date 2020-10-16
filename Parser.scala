object ParserSample extends App {

  val parser = new IniParser

  val result = parser.parseAll(parser.sections, """
[db1]
indexer=Elasticsearch
url=localhost:9200
user=admin
password=qiita

[db2]
driver=Splunk
url=local:8089
user=admin
password=qiita
""")

  val sections = result.get

  println(sections)
  println("")

  val map = sections.sections.map { section =>
    (section.name.string -> section.properties.map { property =>
      (property.key.string -> property.value.string)
    }.toMap)
  }.toMap

  println(map)
}

import scala.util.parsing.combinator.RegexParsers

class IniParser extends RegexParsers {

  // String
  def string :Parser[String_of_AST] = """[^\[\]=\s]*""".r^^{
    case value => String_of_AST(value)
  }

  // Property
  def property :Parser[Property_of_AST] = string~"="~string^^{
    case (key~_~value) => Property_of_AST(key, value)
  }

  // Section
  def section :Parser[Section_of_AST] = "["~>string~"]"~rep(property)^^{
    case (section~_~properties) => Section_of_AST(section, properties)
  }

  // Section(s)
  def sections :Parser[Sections_of_AST] = rep(section)^^{
    case sections => Sections_of_AST(sections)
  }
}

trait AST

// String
case class String_of_AST(string: String) extends AST
// Property
case class Property_of_AST(key: String_of_AST, value: String_of_AST) extends AST
// Section
case class Section_of_AST(name: String_of_AST, properties: List[Property_of_AST]) extends AST
// Section(s)
case class Sections_of_AST(sections: List[Section_of_AST]) extends AST
