
#include <iostream>
#include <limits>

int main()
{
  std::numeric_limits< bool >               x01;
  std::numeric_limits< char >               x02;
  std::numeric_limits< signed char >        x03;
  std::numeric_limits< unsigned char >      x04;
  std::numeric_limits< wchar_t >            x05;
  std::numeric_limits< short >              x06;
  std::numeric_limits< int >                x07;
  std::numeric_limits< long >               x08;
  std::numeric_limits< long long >          x09;
  std::numeric_limits< unsigned short >     x10;
  std::numeric_limits< unsigned int >       x11;
  std::numeric_limits< unsigned long >      x12;
  std::numeric_limits< unsigned long long > x13;
  std::numeric_limits< float >              x14;
  std::numeric_limits< double >             x15;
  std::numeric_limits< long double >        x16;

  if( x01.is_specialized ) {
    std::cout << "Specialized for bool\n";
  }
  if( x02.is_specialized ) {
    std::cout << "Specialized for char\n";
  }
  if( x03.is_specialized ) {
    std::cout << "Specialized for signed char\n";
  }
  if( x04.is_specialized ) {
    std::cout << "Specialized for unsigned char\n";
  }
  if( x05.is_specialized ) {
    std::cout << "Specialized for wchar_t\n";
  }
  if( x06.is_specialized ) {
    std::cout << "Specialized for short\n";
  }
  if( x07.is_specialized ) {
    std::cout << "Specialized for int\n";
  }
  if( x08.is_specialized ) {
    std::cout << "Specialized for long\n";
  }
  if( x09.is_specialized ) {
    std::cout << "Specialized for long long\n";
  }
  if( x10.is_specialized ) {
    std::cout << "Specialized for unsigned short\n";
  }
  if( x11.is_specialized ) {
    std::cout << "Specialized for unsigned int\n";
  }
  if( x12.is_specialized ) {
    std::cout << "Specialized for unsigned long\n";
  }
  if( x13.is_specialized ) {
    std::cout << "Specialized for unsigned long long\n";
  }
  if( x14.is_specialized ) {
    std::cout << "Specialized for float\n";
  }
  if( x15.is_specialized ) {
    std::cout << "Specialized for double\n";
  }
  if( x16.is_specialized ) {
    std::cout << "Specialized for long double\n";
  }

  return 0;
}
