#include<gtkmm.h>
#include<iostream>
#include<vector>

#ifndef MACHINE_H
#define MACHINE_H

#define VERY_SLOW_COST .25
#define SLOW_COST .35
#define FAST_COST .50

#define VERY_SLOW_DET 1.0
#define SLOW_DET 1.5
#define FAST_DET 2.0

#define VERY_SLOW 1
#define SLOW 2
#define FAST 3

class Machine
{
	public:
	
	int speed;
	bool in_use;
	double cost;
	double det_cost;
	int id;

	Machine();

	void set_id(int id);
};

class Fast_Machine : public Machine
{
	public:

	Fast_Machine();
};

class Slow_Machine : public Machine
{
	public:

	Slow_Machine();
};

class Very_Slow_Machine : public Machine
{
	public:

	Very_Slow_Machine();
};

#endif

