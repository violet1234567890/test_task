#include <iostream>
#include <ios>
#include <fstream>
#include <string>

struct Time {
  size_t minutes;
  size_t hours;
};
std::istream & operator>>(std::istream & in, Time & time);
std::ostream & operator<<(std::ostream & out, const Time & time);
bool operator<(Time & time1, Time & time2);
bool operator==(Time & time1, Time & time2);
bool operator>(Time & time1, Time & time2);

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
  out << time.hours << ':' << time.minutes;
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

int main(int argc, char * argv[])
{
  int tables_number = 0;
  int price = 0;
  if (argc != 2)
  {
    std::cerr << "Not enough args\n";
    return 1;
  }
  std::fstream in(argv[1]);
  in >> tables_number;
  if (!in)
  {
    std::cerr << "File not found\n";
    return 2;
  }
  if (tables_number < 0)
  {
    std::cerr << "Amount of tables cannot be negative\n";
    return 3;
  }
  Time open_time{0, 0};
  Time close_time{0, 0};
  in >> open_time >> close_time;
  if (!in)
  {
    // возможно сделать по-другому
  }
  in >> price;
  if (price < 0)
  {
    std::cerr << "Price cannot be negative\n"; //??????????
    return 4;
  }
  std::cout << tables_number << '\n';
  std::cout << tables_number << '\n';
  while (!in.eof())
  {

  }
  return 0;
}
