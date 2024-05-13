#include <iostream>
#include "time.hpp"
int Time::hours_differ(const Time & time2)
{
  int minutes_diff = this->minutes - time2.minutes;
  int hours_diff = this->hours - time2.hours + 1;
  if (minutes_diff < 0)
  {
    hours_diff -= 1;
  }
  return hours_diff;
}

Time Time::time_differ(const Time & time2)
{
  Time time;
  time.hours = this->hours - time2.hours;
  if (this->minutes < time2.minutes)
  {
    time.minutes = 60 + this->minutes - time2.minutes;
    time.hours -= 1;
  }
  return time;
}

Time Time::add(const Time & time2)
{
  return Time{this->hours + time2.hours, this->minutes + time2.minutes};
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
