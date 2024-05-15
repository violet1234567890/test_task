#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "time.hpp"
#include "client.hpp"

std::ostream & print_error_string(std::ostream & out, std::istream & in)
{
  in.clear();
  std::string error_string;
  std::getline(in, error_string);
  out << error_string << "\n";
  return out;
}
std::ostream & print_error(std::ostream & out, const Time & event_time, const std::string & err)
{
  out << event_time << " 13 " << err << '\n';
  return out;
}
std::ostream & print_event(std::ostream & out, const Time & event_time, int id, const Client & client)
{
  out << event_time << ' ' << id << ' ' << client << '\n';
  return out;
}

std::ostream & print_event(std::ostream & out, const Time & event_time, int id,
    const Client & client, int table)
{
  out << event_time << ' ' << id << ' ' << client << ' ' << table << '\n';
  return out;
}

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "Not enough args\n";
    return 1;
  }
  std::fstream in(argv[1]);
  if (!in)
  {
    std::cerr << "File not found\n";
    return 2;
  }
  int tables_number = 0;
  in >> tables_number;
  if (!in)
  {
    print_error_string(std::cout, in);
    return 3;
  }
  if (tables_number < 0)
  {
    std::cout << tables_number << '\n';
    std::cerr << "Amount of tables cannot be negative\n";
    return 4;
  }
  int price = 0;
  Time open_time{0, 0};
  Time close_time{0, 0};
  std::vector< int > profit(tables_number);
  std::vector< Time > table_time(tables_number);
  std::vector< bool > is_table_busy(tables_number);
  std::map< std::string, Client > clients;
  in >> open_time >> close_time;
  std::deque< Client > queue;
  if (!in)
  {
    if (open_time.minutes != 0 && open_time.hours != 0 && close_time.minutes != 0 && close_time.hours != 0)
    {
      std::cout << open_time << ' ' << close_time << '\n';
    }
    print_error_string(std::cout, in);
    std::cout << "Incorrect format of time\n";
    return 3;
  }
  std::cout << open_time << '\n';
  in >> price;
  if (!in)
  {
    print_error_string(std::cout, in);
    return 3;
  }
  if (price < 0)
  {
    std::cerr << "Price cannot be negative\n";
    return 4;
  }
  for (size_t i = 0; i < tables_number; i++)
  {
    is_table_busy[i] = false;
    profit[i] = 0;
    table_time[i] = {0, 0};
  }
  while (!in.eof())
  {
    Time event_time{0, 0};
    int event_id;
    Client client;
    in >> event_time;
    if (!in)
    {
      if (event_time.minutes != 0 && event_time.hours)
      {
        std::cout << open_time << ' ' << close_time << '\n';
      }
      print_error_string(std::cout, in);
      return 3;
    }
    if (in.peek() == 13)
    {
      std::cout << event_time;
      return 3;
    }
    in >> event_id;
    if (!in)
    {
      std::cout << event_time << ' ';
      print_error_string(std::cout, in);
      return 3;
    }
    if (in.peek() == 13)
    {
      std::cout << event_time << ' ' << event_id << '\n';
      return 3;
    }
    in >> client;
    if (!in)
    {
      std::cout << event_time << ' ' << event_id << ' ';
      print_error_string(std::cout, in);
      return 3;
    }
    for (char i : client.get_name())
    {
      if (!(std::isalpha(i) || std::isdigit(i) || (i == '-') || (i == '_')))
      {
        print_event(std::cout, event_time, event_id, client);
        print_error_string(std::cout, in);
        std::cout << "Incorrect format of name\n";
        return 3;
      }
    }
    if (((event_time < open_time) || (event_time > close_time)) && event_id != 2)
    {
      print_error(std::cout, event_time, "NotOpenYet");
      continue;
    }
    switch (event_id)
    {
      case 1:
        print_event(std::cout, event_time, event_id, client);
        if (clients.find(client.get_name()) != clients.end())
        {
          print_error(std::cout, event_time, "YouShallNotPass");
        }
        else
        {
          clients.insert(std::pair(client.get_name(), client));
        }
        break;
      case 2:
      {
        int table_num;
        if (in.peek() == 13)
        {
          print_event(std::cout, event_time, event_id, client);
          return 3;
        }
        in >> table_num;
        if (!in)
        {
          std::cout << event_time << ' ' << event_id << ' ' << client << ' ';
          print_error_string(std::cout, in);
          break;
        }
        print_event(std::cout, event_time, event_id, client, table_num);
        if ((event_time < open_time) || (event_time > close_time))
        {
          print_error(std::cout, event_time, "NotOpenYet");
          break;
        }
        if (table_num > tables_number || table_num <= 0)
        {
          std::cout << "Incorrect table number\n";
          return 3;
        }
        if (tables_number < 0)
        {
          std::cerr << "Table number cannot be negative\n";
          return 3;
        }
        if (clients.find(client.get_name()) != clients.end())
        {
          if (is_table_busy[table_num - 1])
          {
            print_error(std::cout, event_time, "PlaceIsBusy");
            break;
          }
          Client & client_ref = clients[client.get_name()];
          int table = client_ref.get_table();
          Time time = client_ref.get_time();
          if (table != 0)
          {
            is_table_busy[table - 1] = false;
            profit[table - 1] += price * hours_differ(event_time, time);
            table_time[table - 1].add(time_differ(event_time, time));
          }
          client_ref.set_table(table_num);
          is_table_busy[table_num - 1] = true;
          client_ref.set_time(event_time);
        }
        else
        {
          print_error(std::cout, event_time, "ClientUnknown");
        }
      }
        break;
      case 3:
      {
        bool queue_flag = true;
        print_event(std::cout, event_time, event_id, client);
        if (clients.find(client.get_name()) != clients.end())
        {
          int count = 0;
          for (bool k: is_table_busy)
          {
            if (!k)
            {
              print_error(std::cout, event_time, "ICanWaitNoLonger!");
              queue_flag = false;
              break;
            }
            count++;
          }
          if (!queue_flag)
          {
            break;
          }
          else
          {
            if (queue.size() >= tables_number)
            {
              event_id = 11;
              print_event(std::cout, event_time, event_id, client);
              clients.erase(client.get_name());
            }
            else
            {
              queue.push_back(client);
            }
          }
        }
        else
        {
          print_error(std::cout, event_time, "ClientUnknown");
        }
      }
        break;
      case 4:
      {
        print_event(std::cout, event_time, event_id, client);
        if (clients.find(client.get_name()) != clients.end())
        {
          Client & client_ref = clients[client.get_name()];
          int table = client_ref.get_table();
          Time time = client_ref.get_time();
          if (table != 0)
          {
            profit[table - 1] += price * hours_differ(event_time, time);
            table_time[table - 1].add(time_differ(event_time, time));
            if (!queue.empty())
            {
              Client & new_client_ref = clients[queue[0].get_name()];
              event_id = 12;
              print_event(std::cout, event_time, event_id, queue[0], table);
              new_client_ref.set_table(table);
              new_client_ref.set_time(event_time);
              queue.pop_front();
            }
            else
            {
              is_table_busy[table - 1] = false;
            }
          }
          clients.erase(client.get_name());
        }
        else
        {
          print_error(std::cout, event_time, "ClientUnknown");
        }
      }
        break;
      default:
        std::cout << event_time << ' ' << event_id << ' ' << client << ' ';
        print_error_string(std::cout, in);
        std::cerr <<"Incorrect event\n";
        return 3;
    }
  }
  for (auto k = clients.begin(); k != clients.end(); k++)
  {
    int table = k->second.get_table();
    Time time = k->second.get_time();
    if (table != 0)
    {
      profit[k->second.get_table() - 1] += price * hours_differ(close_time, time);
      table_time[k->second.get_table() - 1].add(time_differ(close_time, time));
    }
    int event_id = 11;
    print_event(std::cout, close_time, event_id, k->second);
  }
  clients.clear();
  std::cout << close_time << '\n';
  for (int i = 0; i < tables_number; i++)
  {
    std::cout << i + 1 << ' ' << profit[i] << ' ' << table_time[i] << '\n';
  }
  return 0;
}