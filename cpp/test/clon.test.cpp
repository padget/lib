
#include <lib/test.hpp>
#include <lib/clon.hpp>
#include <lib/ios.hpp>

struct person
{
  size_t age;
  lib::string name;
  lib::string firstname;
};

namespace lib
{
  clon ser(const person &s)
  {
    return clon(format(
      R"((person (name "#")(firstname "#")(age #)))", 
      s.name, s.firstname, s.age));
  }
}

void should_convert_to_clon()
{
  person ben(43, "afleck", "ben");
  
  lib::printfln(format(R"((person (name "#")(firstname "#")(age #)))",
           ben.name, ben.firstname, ben.age));
  lib::clon cp = lib::ser(ben);
  lib::printfln("#", cp);
  //lib::person = lib::des(cp, lib::to_person);
}

void should_parse_clon_string()
{

  using namespace lib::literals;

  lib::clon c = R"(
    (person 
      (address 
        (city "san fransisco"))
      (name "coucou")
      (firstname "cocuou")
      (name "coco")))"_clon;

  //lib::printfln("clon : #", c);
  lib::printfln("name '#'", c.get_first(lib::pth["address"]["city"]).value());
}

int main(int argc, char **argv)
{
  should_parse_clon_string();
  should_convert_to_clon();
  lib::println("clon.hpp compile !");
  return 0;
}
