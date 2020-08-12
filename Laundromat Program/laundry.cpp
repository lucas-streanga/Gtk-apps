//Lucas Streanga 	1001612558
//Gonna try to get this to work with gtk
//

#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<gtkmm.h>

#include"store.h"
#include"machines.h"
#include"patrons.h"

using namespace std;

class config_window : public Gtk::Window
{

	public:

	Gtk::VBox layout;
	Gtk::Entry store;
	Gtk::Entry patron;
	Gtk::Button okay;
	string store_file;
	string patron_file;
	Store *s;

	Glib::RefPtr<Gtk::Application> application;

	void get_info()
	{
		store_file = store.get_text();
		patron_file = patron.get_text();
	}

	void switch_to_main_window()
	{
		s = new Store(store_file, patron_file, application);
		application->add_window(*(s->store_window));
		this->hide();
		s->store_window->show_all();
	}

	config_window(Glib::RefPtr<Gtk::Application> app)
	{
		s = NULL;
		resize(300,150);
		set_border_width(10);
		set_title("Config");
		set_position(Gtk::WIN_POS_CENTER);
		store.set_text("<store file here>");
		patron.set_text("<patrons file here>");
		application = app;

		okay.add_label("Continue");

		okay.signal_pressed().connect(sigc::mem_fun(*this, &config_window::get_info));
		okay.signal_pressed().connect(sigc::mem_fun(*this, &config_window::switch_to_main_window));

		layout.pack_start(store);
		layout.pack_start(patron);
		layout.pack_start(okay);

		layout.show_all();
		add(layout);
	}

	virtual ~config_window(){}
};

int main(int argc, char **argv)
{
	//We need a window to find out the configuration...
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	config_window *c = new config_window(app);
	app->run(*c);
	if(c->s != NULL)
	{
		cout << "deleting store object at: " << c->s << endl;
		delete(c->s);
	}
	cout << "deleting config window object at: " << c << endl;
	delete c;

}
