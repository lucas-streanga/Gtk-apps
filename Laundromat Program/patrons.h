#include<gtkmm.h>
#include<iostream>
#include<vector>
#include"store.h"
#include"machines.h"

#ifndef PATRON_H
#define PATRON_H

#define PATRON_XML "patron.xml"

class Store;

class Person
{
	public:

	std::vector <Machine *> machines_available;
	std::vector <Machine *> machines_in_use;

	std::string name;
	std::string id;
	double balance;
	double det_available;
	int access;
	std::string member;

	Gtk::Window *store_window;
	Glib::RefPtr<Gtk::Application> application;
	Store *store;

	Gtk::Window *patron_window;
    Gtk::Label *num_fast;
    Gtk::Label *num_slow;
    Gtk::Label *num_very_slow;
    Gtk::Button *wash_button;
    Gtk::Button *pickup_button;
	Gtk::Button *back;
	Gtk::Label *member_label;
	Gtk::Label *bal_label;
	Gtk::Label *det_label;
	
	//Constructor
	Person(std::string name, std::string id, double balance, double det_available); 
	
	//Functions
	Machine * wash(Machine *m);
	virtual void choose_machine() = 0;
	void available();
	bool check_price(Machine *m);
	bool check_det(Machine *m);
	void pickup();
	void go_back();
	Machine * find_available_machine(int speed);
	void run(Glib::RefPtr<Gtk::Application> app, Store *);
	void receipt(Machine *m);
	void update_resources();
};

class Discount : public Person
{
	public:

	//Constructor
	Discount(std::string name, std::string id, double balance, double det_available, std::vector <Machine *> machines);

	void choose_machine();
};

class Regular : public Person
{
	public:

	//Constructor
	Regular(std::string name, std::string id, double balance, double det_available, std::vector <Machine *> machines);

	void choose_machine();
};

class Elite : public Person
{
	public:

	//Constructor
	Elite(std::string name, std::string id, double balance, double det_available, std::vector <Machine *> machines);

	void choose_machine();
};

#endif




