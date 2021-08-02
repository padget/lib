#include "base64.hpp"
#include "test.hpp"


void encode_should_encode_input()
{
  test_equals(base64::encode<lib::string>("Benjamin"), "QmVuamFtaW4=");
}

void decode_should_decode_input()
{
  test_equals(base64::decode<lib::string>("QmVuamFtaW4="), "Benjamin");
}


int main()
{
  run_test(encode_should_encode_input);
  run_test(decode_should_decode_input);
}
