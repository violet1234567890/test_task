#include <iostream>
#include "time.hpp"
std::istream & operator>>(std::istream & in, Time & time)
{
  char delimiter = '0';
  in >> time.hours >> delimiter >> time.minutes;
  if (time.hours >= 23 || time.minutes >= 59)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream & operator<<(std::ostream & out, const Time & time)
{
  out << time.hours / 10 << time.hours % 10 << ':' << time.minutes / 10 << time.minutes % 10;
}

bool operator<(Time & time1, Time & time2)
{
  return ((time1.hours < time2.hours) ? true : (time1.minutes < time2.minutes));
}

bool operator==(Time & time1, Time & time2)
{
  return ((time1.hours == time2.hours) && (time1.minutes == time2.hours));
}

bool operator>(Time & time1, Time & time2)
{
  return !(time1 < time2);
}
