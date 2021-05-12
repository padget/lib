#include <lib/ios.hpp>
#include <lib/test.hpp>
#include <lib/clon.hpp>

void should_parse_clon_string()
{

  using namespace lib::literals;

  lib::clon c =  R"(
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
