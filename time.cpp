#include <iostream>
#include "time.hpp"

void Time::add(const Time & time2)
{
  this->hours += time2.hours;
  this->minutes += time2.minutes;
}

std::istream & operator>>(std::istream & in, Time & time)
{
  char delimiter = '0';
  in >> time.hours >> delimiter >> time.minutes;
  if (time.hours > 23 || time.minutes > 59)
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

Time time_differ(const Time & time1, const Time & time2)
{
  Time time = {0, 0};
  time.hours = time1.hours - time2.hours;
  if (time1.minutes < time2.minutes)
  {
    time.minutes = 60 - time2.minutes;
    time.minutes += time1.minutes;
    time.hours -= 1;
  }
  else
  {
    time.minutes = time1.minutes - time2.minutes;
  }
  return time;
}

int hours_differ(const Time & time1, const Time & time2)
{
  int minutes_diff = time1.minutes - time2.minutes;
  int hours_diff = time1.hours - time2.hours + 1;
  if (minutes_diff < 0)
  {
    hours_diff -= 1;
  }
  return hours_diff;
}