
#include <lib/test.hpp>
#include <lib/clon.hpp>
#include <lib/ios.hpp>

struct person
{
  size_t age;
  lib::string name;
  lib::string firstname;
};


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

  lib::printfln("clon : #", c);
  lib::printfln("name '#'", c.get_first(lib::pth["address"]["city"]).value());
}

int main(int argc, char **argv)
{
  should_parse_clon_string();
  lib::println("clon.hpp compile !");
  return 0;
}
