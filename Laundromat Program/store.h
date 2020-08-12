#include<gtkmm.h>
#include<iostream>
#include<vector>
#include<memory>

#ifndef STORE_H
#define STORE_H

#include"patrons.h"
#include"machines.h"

#define RES_X 1024
#define RES_Y 768
#define STORE_XML "store.xml"

class Store
{
	std::vector <Person *> patrons;

	public:

	std::vector <Machine *> machines;

	Gtk::Window *store_window;
	Gtk::Window *information;
	Gtk::Button *quit;
	Gtk::Button *confirm;
	Gtk::Button *info;
	Gtk::Entry *entry_id;
	Gtk::Label *num_fast;
	Gtk::Label *num_slow;
	Gtk::Label *num_very_slow;

	Glib::RefPtr<Gtk::Application> application;	

	Store(std::string laundry_file, std::string patrons_file, Glib::RefPtr<Gtk::Application> app); //Constructor
	
	Person * find_patron(std::string id);
	void available();

	void on_button_quit();
	void on_button_confirm();
	void show_info();

	~Store();
};

class info_window : public Gtk::Window
{
    public:

    Gtk::VBox layout;
    Gtk::Label total_machines;

	Gtk::Label number_machines;
	Gtk::Label fast_cost;
	Gtk::Label slow_cost;
	Gtk::Label very_slow_cost;
	Gtk::Button quit_button;

	int num_f;
	int num_s;
	int num_vs;

	void quit();

    info_window(Store *);
};


#endif
	
