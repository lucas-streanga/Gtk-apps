#include<vector>
#include<iostream>
#include<gtkmm.h>
#include"store.h"
#include"patrons.h"
#include"machines.h"

using namespace std;

Machine::Machine()
{
	in_use = false;
}

void Machine::set_id(int id)
{
	this->id = id;
}

Fast_Machine::Fast_Machine():Machine()
{
	cost = FAST_COST;
	det_cost = FAST_DET;
	speed = FAST;
}

Slow_Machine::Slow_Machine():Machine()
{
	cost = SLOW_COST;
	det_cost = SLOW_DET;
	speed = SLOW;
}

Very_Slow_Machine::Very_Slow_Machine():Machine()
{
	cost = VERY_SLOW_COST;
	det_cost = VERY_SLOW_DET;
	speed = VERY_SLOW;
}

