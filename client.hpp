#ifndef TEST_TASK_YADRO_CLIENT_HPP
#define TEST_TASK_YADRO_CLIENT_HPP
#include <string>
#include <iosfwd>
#include "time.hpp"
class Client
{
 public:
  Client();
  int get_table();
  void set_table(int table);
  const Time & get_time();
  void set_time(const Time & time);
  friend std::istream & operator>>(std::istream &, Client & client);
  friend std::ostream & operator<<(std::ostream &, const Client & client);
  friend bool operator==(const Client & client1, const Client & client2);
  friend bool operator<(const Client & client1, const Client & client2);
 private:
  std::string name;
  int table;
  Time enter_time;
};


#endif
