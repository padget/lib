#include <lib/ios.hpp>
#include <lib/test.hpp>
#include <lib/clon.hpp>

void should_parse_clon_string()
{
  constexpr lib::string_view cstr = R"(
    (person 
      (address 
        (city "san fransisco"))
      (name "coucou")
      (firstname "cocuou")
      (name "coco")))";
  lib::clon c(cstr);

  lib::printfln("clon : #", c);
  lib::printfln("name '#'", c.get_first(lib::pth["address"]["city"]));
}

int main(int argc, char **argv)
{
  should_parse_clon_string();
  lib::println("clon.hpp compile !");
  return 0;
}
