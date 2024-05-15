#ifndef TEST_TASK_CLIENT_HPP
#define TEST_TASK_CLIENT_HPP
#include <string>
#include <iosfwd>
#include "time.hpp"
class Client
{
 public:
  Client();
  std::string get_name() const;
  int get_table() const;
  void set_table(int table);
  const Time & get_time() const;
  void set_time(const Time & time);
  friend std::istream & operator>>(std::istream &, Client & client);
  friend std::ostream & operator<<(std::ostream &, const Client & client);
  friend bool operator==(const Client & client1, const Client & client2);
  friend bool operator<(const Client & client1, const Client & client2); //comparator for std::map
 private:
  std::string name;
  int table;
  Time enter_time;
};
#endif
