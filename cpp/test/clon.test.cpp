#include <lib/ios.hpp>
#include <lib/test.hpp>
#include <lib/clon.hpp>

void should_parse_clon_string()
{
  constexpr lib::string_view cstr = R"(
    (person 
      (name "coucou")
      (firstname "cocuou")
      (address 
        (city "san fransisco")
        (street 
          (number "doudou")
          (libelle "goo")
        )
      )(name "coucou")
      (firstname "cocuou")
      (address 
        (city "san fransisco")
        (street 
          (number "doudou")
          (libelle "goo")
        )
      )(name "coucou")
      (firstname "cocuou")
      (address 
        (city "san fransisco")
        (street 
          (number "doudou")
          (libelle "goo")
        )
      )(name "coucou")
      (firstname "cocuou")
      (address 
        (city "san fransisco")
        (street 
          (number "doudou")
          (libelle "goo")
        )
      )(name "coucou")
      (firstname "cocuou")
      (address 
        (city "san fransisco")
        (street 
          (number "doudou")
          (libelle "goo")
        )
      )))";
  lib::clon c(cstr);


  lib::printfln("clon : #", c);
  // lib::printfln("c name #", c.root_name());
}

int main(int argc, char **argv)
{
  should_parse_clon_string();
  lib::println("clon.hpp compile !");
  return 0;
}
