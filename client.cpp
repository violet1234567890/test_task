#include <iostream>
#include "client.hpp"

Client::Client()
{
  this->table = 0;
}
int Client::get_table()
{
  return this->table;
}

const Time & Client::get_time()
{
  return this->enter_time;
}
void Client::set_time(const Time & time)
{
  this->enter_time.minutes = time.minutes;
  this->enter_time.hours = time.hours;
}

void Client::set_table(int table)
{
  this->table = table;
}

std::istream & operator>>(std::istream & in, Client & client)
{
  in >> client.name;
  return in;
}
std::ostream & operator<<(std::ostream & out, const Client & client)
{
  out << client.name;
  return out;
}

bool operator==(const Client & client1, const Client & client2)
{
  return client1.name == client2.name;
}

bool operator<(const Client & client1, const Client & client2)
{
  return client1.name < client2.name;
}