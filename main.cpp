#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <set>
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
}

std::ostream & print_event(std::ostream & out, const Time & event_time, int id,
    const Client & client, int table)
{
  out << event_time << ' ' << id << ' ' << client << ' ' << table << '\n';
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
    std::cerr << "Amount of tables cannot be negative\n";
    return 4;
  }
  int price = 0;
  Time open_time{0, 0};
  Time close_time{0, 0};
  std::vector< int > profit(tables_number);
  std::vector< Time > table_time(tables_number);
  std::vector< bool > is_table_busy(tables_number);
  std::set< Client > clients; //using set because we need alphabet order
  in >> open_time >> close_time;
  std::deque< Client > queue; //using
  if (!in)
  {
    print_error_string(std::cout, in);
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
    std::cerr << "Price cannot be negative\n"; //??????????
    return 4;
  }
  for (size_t i = 0; i < tables_number; i++)
  {
    is_table_busy[i] = false;
    profit[i] = 0;
    table_time[i] = {0, 0};
  };
  while (!in.eof())
  {
    Time event_time{0, 0};
    int event_id;
    Client client;
    in >> event_time;
    if (!in)
    {
      print_error_string(std::cout, in);
      return 3;
    }
    in >> event_id;
    if (!in)
    {
      std::cout << event_time << ' ';
      print_error_string(std::cout, in);
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
      if (!(std::isalpha(i) || std::isdigit(i)))
      {
        print_event(std::cout, event_time, event_id, client);
        print_error_string(std::cout, in);
        return 3;
      }
    }
    switch (event_id)
    {
      case 1:
        print_event(std::cout, event_time, event_id, client);
        if ((event_time < open_time) || (event_time > close_time))
        {
          print_error(std::cout, event_time, "NotOpenYet");
          break;
        }
        if (clients.find(client) != clients.end())
        {
          print_error(std::cout, event_time, "YouShallNotPass");
        }
        else
        {
          clients.insert(client);
        }
        break;
      case 2:
        int table_num;
        in >> table_num;
        if (!in)
        {
          std::cout << event_time << ' ' << event_id << ' ' << client << ' ';
          print_error_string(std::cout, in);
          return 3;
        }
        print_event(std::cout, event_time, event_id, client, table_num);
        if ((event_time < open_time) || (event_time > close_time))
        {
          print_error(std::cout, event_time, "NotOpenYet");
          break;
        }
        if (tables_number < 0)
        {
          std::cerr << "Table number cannot be negative\n";
          break;
        }
        if (clients.find(client) != clients.end())
        {
          if (is_table_busy[table_num - 1])
          {
            print_error(std::cout, event_time, "PlaceIsBusy");
            break;
          }
          int table = clients.find(client)->get_table();
          Time time = clients.find(client)->get_time();
          if (table != 0)
          {
            is_table_busy[table - 1] = false;
            profit[table - 1] += price * hours_differ(event_time, time);
            table_time[table - 1].add(time_differ(event_time, time));
          }
          clients.erase(client);
          client.set_table(table_num);
          is_table_busy[table_num - 1] = true;
          client.set_time(event_time);
          clients.insert(client); //????
        }
        else
        {
          print_error(std::cout, event_time, "ClientUnknown");
        }
        break;
      case 3:
      {
        bool queue_flag = true;
        print_event(std::cout, event_time, event_id, client);
        if ((event_time < open_time) || (event_time > close_time))
        {
          print_error(std::cout, event_time, "NotOpenYet");
          break;
        }
        if (clients.find(client) != clients.end())
        {
          for (bool k: is_table_busy)
          {
            if (!k)
            {
              print_error(std::cout, event_time, "ICanWaitNoLonger!");
              queue_flag = false;
              break;
            }
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
              clients.erase(client);
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
        if ((event_time < open_time) || (event_time > close_time))
        {
          print_error(std::cout, event_time, "NotOpenYet");
          break;
        }
        if (clients.find(client) != clients.end())
        {
          int table = clients.find(client)->get_table();
          Time time = clients.find(client)->get_time();
          if (table != 0)
          {
            profit[table - 1] += price * hours_differ(event_time, time);
            table_time[table - 1].add(time_differ(event_time, time));
            if (!queue.empty())
            {
              clients.erase(queue[0]);
              event_id = 12;
              print_event(std::cout, event_time, event_id, queue[0], table);
              queue[0].set_table(table);
              queue[0].set_time(event_time);
              clients.insert(queue[0]);
              queue.pop_front();
            }
          }
          clients.erase(client);
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
        std::cout <<"Incorrect event\n";
    }
  }
  for (auto k = clients.begin(); k != clients.end(); k++)
  {
    int table = k->get_table();
    Time time = k->get_time();
    if (table != 0)
    {
      profit[k->get_table() - 1] += price * hours_differ(close_time, time);
      table_time[k->get_table() - 1].add(time_differ(close_time, time));
    }
    int event_id = 11;
    print_event(std::cout, close_time, event_id, *k);
  }
  clients.clear();
  std::cout << close_time << '\n';
  for (int i = 0; i < tables_number; i++)
  {
    std::cout << i + 1 << ' ' << profit[i] << ' ' << table_time[i] << '\n';
  }
  return 0;
}