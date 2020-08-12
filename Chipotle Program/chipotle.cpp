//Lucas Streanga  1001612558

#include<vector>
#include<queue>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include"chipotle.h"
#include"globals.h"

using namespace std;

//First our person constructor...
Person::Person(string first, string last)
{
  this->first = first;
  this->last = last;
}

void Order::build(int properties, bool is_bowl)
{
  this->is_bowl = true;
  this->properties = true;

  if(is_bowl)
    price = BOWL_PRICE;
  else
    price = BURRITO_PRICE;
}

Chipotle::Chipotle(string filename)
{
  drivers_file = filename;
  ifstream file;
  file.open(filename);

  if(!file.is_open())
  {
    cout << "Unable to open file: \"" << filename << "\"\nExitting..." << endl;
    is_file_openable = false;
  }
  else
    is_file_openable = true;

  if(is_file_openable)
  {
    while(!file.eof())
    {
      string line;
      string first;
      string last;

      getline(file, line);
      stringstream delim_line(line);
      getline(delim_line, first, ',');
      getline(delim_line, last);

      Person driver(first, last);

      if(first != "")
        drivers.push(driver);
      }

  file.close();
  }
}

double Chipotle::delivery_charge()
{
  double charge = 0;

  for(int i = 0; i < orders.size(); i++)
  {
    if(orders[i].is_deliverable)
    {
      if(orders[i].is_bowl)
        charge += BOWL_PRICE;
      else
        charge += BURRITO_PRICE;
    }
  }
  charge = charge * (PERCENT / 100);

  return charge;
}

double Chipotle::revenue()
{
  double charge = 0;

  for(int i = 0; i < orders.size(); i++)
  {
    if(orders[i].is_bowl)
      charge += BOWL_PRICE;
    else
      charge += BURRITO_PRICE;
  }

  return charge;
}

bool Chipotle::apply(string first, string last)
{
  fstream file;
  file.open(drivers_file);

  if(!file.is_open())
  {
    cout << "\nUnable to open file for writing: \"" << drivers_file << endl;
    return false;
  }
  else
  {
    Person new_driver(first, last);

    drivers.push(new_driver);

    //Now to write to file...

    file.seekg(0, file.end);
    file << first << " " << last << endl;

    file.close();
    return true;
  }
}
