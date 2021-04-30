#include <lib/ios.hpp>
#include <lib/test.hpp>
#include <lib/clon.hpp>

void print_tree(const lib::tree<lib::clon_value<char>>& t)
{
  for (std::size_t i(0); i<t.size(); ++i)
    lib::printfln("# : c # - lc # - n # - v (# \"#\")", 
    t[i].id, t[i].child, t[i].last_child, t[i].next, t[i].value.name, t[i].value.val);
}

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
  print_tree(c.nodes);
 
}

int main(int argc, char **argv)
{
  should_parse_clon_string();
  lib::println("clon.hpp compile !");
  return 0;
}
