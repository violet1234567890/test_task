#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include "time.hpp"
#include "client.hpp"
//НЕПРАВИЛЬНЫЙ ФОРМАТ ВВОДА
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
  int tables_number = 0;
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
  int price = 0;
  Time open_time{0, 0};
  Time close_time{0, 0};
  size_t profit = 0;
  std::vector< bool > is_table_busy(tables_number);
  std::set< Client > clients; //set
  in >> open_time >> close_time;
  std::deque< Client > queue;
  if (!in)
  {
    // возможно сделать по-другому
  }
  std::cout << open_time << '\n';
  in >> price;
  if (price < 0)
  {
    std::cerr << "Price cannot be negative\n"; //??????????
    return 4;
  }

  //std::cout << tables_number << '\n';

  for (size_t i = 0; i < tables_number; i++)
  {
    is_table_busy[i] = false;
  };
  //std::cout << open_time << '\n';
  //std::cout << close_time << '\n';
  //std::cout << price << '\n';
  while (!in.eof())
  {
    Time event_time{0, 0};
    int event_id;
    Client client;
    in >> event_time;
    in >> event_id;
    in >> client;
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
          client.set_time(event_time);
          clients.insert(client);
        }
        break;
      case 2:
        int table_num;
        in >> table_num;
        print_event(std::cout, event_time, event_id, client, table_num);
        if ((event_time < open_time) || (event_time > close_time))
        {
          print_error(std::cout, event_time, "NotOpenYet");
          break;
        }
        if (tables_number < 0)
        {
          ////
        }
        if (clients.find(client) != clients.end())
        {
          if (is_table_busy[table_num - 1])
          {
            print_error(std::cout, event_time, "PlaceIsBusy");
            break;
          }
          if (int table = client.get_table() != 0)
          {
            is_table_busy[table - 1] = false;
          }
          client.set_table(table_num);
          is_table_busy[table_num - 1] = true;
        }
        else
        {
          print_error(std::cout, event_time, "ClientUnknown");
        }
        break;
      case 3:
        print_event(std::cout, event_time, event_id, client);
        if ((event_time < open_time) || (event_time > close_time))
        {
          print_error(std::cout, event_time, "NotOpenYet");
          break;
        }
        if (clients.find(client) != clients.end())
        {
          for (bool k : is_table_busy)
          {
            if (!k)
            {
              print_error(std::cout)
            }
          }
        }
        else
        {
          print_error(std::cout, event_time, "ClientUnknown");
        }
        break;
      default:
        std::cout <<"error\n"; ///////
        //
    }
  }
  return 0;
}
