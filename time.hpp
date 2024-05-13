#ifndef TEST_TASK_YADRO_TIME_HPP
#define TEST_TASK_YADRO_TIME_HPP
#include <iosfwd>
struct Time {
    size_t minutes;
    size_t hours;
  int hours_differ(const Time & time2);
  Time time_differ(const Time & time2);
  Time add(const Time & time2);
  };
  std::istream & operator>>(std::istream & in, Time & time);
  std::ostream & operator<<(std::ostream & out, const Time & time);
  bool operator<(Time & time1, Time & time2);
  bool operator==(Time & time1, Time & time2);
  bool operator>(Time & time1, Time & time2);
#endif
