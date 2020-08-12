//Header file for our chipotle store...
//Lucas Streanga 1001612558

#ifndef _CHIPOTLE_H
#define _CHIPOTLE_H

#include<vector>
#include<string>
#include<queue>
#include"globals.h"

class Person
{
public:

  std::string first;
  std::string last;

  Person(std::string first, std::string last);

};

class Order
{
public:

  double price;
  bool is_deliverable;
  bool is_bowl;
  int properties;
  enum burrito_properties
  {
    TOFU = 1,
    CHICKEN = 2,
    STEAK = 4,
    WHITE_RICE = 8,
    BROWN_RICE = 16,
    QUESO = 32,
    SOUR_CREAM = 64,
    SALSA = 128
  };

  void build(int properties, bool is_bowl);

};

class Chipotle
{
public:

  std::queue<Person> drivers;
  std::vector<Order> orders;
  std::string drivers_file;
  bool is_file_openable; //See if file is openable...

  Chipotle(std::string filename);
  bool apply(std::string first, std::string second);
  double delivery_charge();
  double revenue();

};
#endif
