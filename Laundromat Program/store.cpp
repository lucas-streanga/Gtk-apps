#include<vector>
#include<iostream>
#include<gtkmm.h>
#include<sstream>
#include<fstream>
#include<memory>
#include<iomanip>

#include"store.h"
#include"patrons.h"
#include"machines.h"

//Store class definitions here

using namespace std;

Store::Store(string laundry_file, string patrons_file, Glib::RefPtr<Gtk::Application> app)
{

	application = app;
	information = NULL;

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(STORE_XML);

	//We have an alloc here - must delete
	builder->get_widget("login", store_window);

	//Now we need to get the rest of the widgets that we will be using
	builder->get_widget("entry_id", entry_id);
	builder->get_widget("confirm_button", confirm);
	builder->get_widget("quit", quit);
	builder->get_widget("num_fast", num_fast);
	builder->get_widget("num_slow", num_slow);
	builder->get_widget("num_very_slow", num_very_slow);
	builder->get_widget("info", info);

	store_window->resize(RES_X, RES_Y);

	//Now our signal handlers...
	quit->signal_clicked().connect(sigc::mem_fun(*this, &Store::on_button_quit));
	confirm->signal_clicked().connect(sigc::mem_fun(*this, &Store::on_button_confirm));
	info->signal_clicked().connect(sigc::mem_fun(*this, &Store::show_info));

  //quit->override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);
  //quit->override_background_color(Gdk::RGBA("red"));

	ifstream laundry;
	ifstream patrons;
	laundry.open(laundry_file);

	Gtk::MessageDialog file_error(*store_window, "Error opening file",false,Gtk::MESSAGE_INFO);

	if(!laundry.is_open())
	{
		file_error.set_secondary_text("Unable to open file: " + laundry_file);
		file_error.run();
		cout << "Unable to open file: " << laundry_file << "\nexitting..." << endl;
		exit(1);
	}

	patrons.open(patrons_file);

	if(!patrons.is_open())
	{
		file_error.set_secondary_text("Unable to open file: " + patrons_file);
		file_error.run();
		cout << "Unable to open file: " << patrons_file << "\nexitting..." << endl;
		exit(1);
	}
	//Now we need to read in the information

	while(!laundry.eof())
	{
		string line;
		string id;
		string speed;

		getline(laundry, line);

		if(line != "")
		{
			Machine *m = NULL;
			stringstream delim(line);
			getline(delim, id, ',');
			getline(delim, speed, ',');

			int num = stoi(id);

			if(speed == "very slow")
				m = new Very_Slow_Machine();
			else if(speed == "slow")
				m = new Slow_Machine();
			else if(speed == "fast")
				m = new Fast_Machine();
			else
				cout << "Unknown speed: " << speed << endl;

			if(m != NULL)
			{
				m->set_id(num);
				machines.push_back(m);
			}
		}
	}

	cout << "Successfully read in Laundomat info: " << laundry_file << endl;

	while(!patrons.eof())
	{
		string line;
		string name;
		string id;
		string balance;
		string det;

		getline(patrons, line);

		if(line != "")
		{
			Person *p = NULL;
			stringstream delim(line);
			getline(delim, name, ',');
			getline(delim, id, ',');
			getline(delim, balance, ',');
			getline(delim, det, ',');

			//We need to resolve the id to figure out which member to create
			string temp = id;
			temp.erase(0, 1); //Get rid of the char.
			int num = stoi(temp);
			double bal = stod(balance);
			double detergent = stod(det);

			if(num <= 500)
				p = new Discount(name, id, bal, detergent, machines);
			else if(num <= 900)
				p = new Regular(name, id, bal, detergent, machines);
			else if(num < 1000)
				p = new Elite(name, id, bal, detergent, machines);
			else
				cout << "Unable to resolve id: " << id << endl;

			if(p != NULL)
				this->patrons.push_back(p);
		}
	}

	cout << "Successfully read in patrons: " << patrons_file << endl;
	available();
	//store_window->activate_focus().connect(sigc::mem_fun(*this, &Store::available);
}

info_window::info_window(Store *s)
{
        resize(400,400);
        set_position(Gtk::WIN_POS_CENTER);
        set_title("Store Information");
		total_machines.set_label("Total Machines: " + to_string(s->machines.size()));
		//Now we need to get the other number of machines
		num_f = 0;
		num_s = 0;
		num_vs = 0;
		for(int i = 0; i < s->machines.size(); i++)
		{
			if(s->machines[i]->speed == FAST)
				num_f++;
		}
		for(int i = 0; i < s->machines.size(); i++)
		{
			if(s->machines[i]->speed == SLOW)
				num_s++;
		}
		for(int i = 0; i < s->machines.size(); i++)
		{
			if(s->machines[i]->speed == VERY_SLOW)
				num_vs++;
		}

		stringstream fc;
		stringstream sc;
		stringstream vsc;
		fc << fixed << setprecision(2);
		sc << fixed << setprecision(2);
		vsc << fixed << setprecision(2);

		fc << "Fast:\nCost: $" << FAST_COST << "\nDetergent: " << FAST_DET << " ounces";
		sc << "Slow:\nCost: $" << SLOW_COST << "\nDetergent: " << SLOW_DET << " ounces";
		vsc << "Very Slow:\nCost: $" << VERY_SLOW_COST << "\nDetergent: " << VERY_SLOW_DET << " ounces";

		number_machines.set_label("Fast: " + to_string(num_f) + "\nSlow: " + to_string(num_s) + "\nVery Slow: " + to_string(num_vs));

		fast_cost.set_label(fc.str());
		slow_cost.set_label(sc.str());
		very_slow_cost.set_label(vsc.str());

		quit_button.signal_clicked().connect(sigc::mem_fun(*this, &info_window::quit));
		quit_button.set_label("Quit");

		/*Glib::RefPtr<Gtk::CssProvider> p_css_provider = Gtk::CssProvider::create();
		p_css_provider->load_from_path("button.css");
		quit_button.get_style_context()->add_provider(p_css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
		fast_cost.get_style_context()->add_provider(p_css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);*/

		layout.pack_start(total_machines);
		layout.pack_start(number_machines);
		layout.pack_start(fast_cost);
		layout.pack_start(slow_cost);
		layout.pack_start(very_slow_cost);
		layout.pack_start(quit_button);
        layout.show_all();
        add(layout);
}

void info_window::quit()
{
	this->hide();
}

void Store::show_info()
{
	//We will create a new window and show all the store information...

	information = new info_window(this);
	application->add_window(*information);
	information->show();
}

Person * Store::find_patron(string id)
{
    Person *p = NULL;

    for(int i = 0; i < patrons.size(); i++)
    {
        if(patrons[i]->id == id)
            p = patrons[i];
    }

    if(p == NULL)
        cout << "No known patron with id " << id << endl;

    return p;
}

void Store::available()
{
	int very_slow = 0;
	int slow = 0;
	int fast = 0;

	cout << "Updating store's available machines." << endl;

    cout << "\n\n***Available machines:" << endl;
    for(int i = 0; i < machines.size(); i++)
    {
        if(!machines[i]->in_use)
        {
            if(machines[i]->speed == VERY_SLOW)
                very_slow++;
            else if(machines[i]->speed == SLOW)
                slow++;
            else
                fast++;
        }

    }

	num_very_slow->set_label(to_string(very_slow));
	num_slow->set_label(to_string(slow));
	num_fast->set_label(to_string(fast));

    cout << "Fast Machines: " << fast << "\nSlow Machines: " << slow << "\nVery Slow Machines: " << very_slow << endl;
}

void Store::on_button_quit()
{
	Gtk::MessageDialog dialog (*store_window, "Are you sure you want to quit?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);

	int result = dialog.run();

	if(result == Gtk::RESPONSE_YES)
	{
		if(information != NULL)
			information->hide();

		store_window->hide();
	}
}

void Store::on_button_confirm()
{
	//We need to get the id...
	string id = entry_id->get_text();

	if(find_patron(id) != NULL)
	{
		find_patron(id)->run(application, this);
	}
	else
	{
		Gtk::MessageDialog dialog(*store_window, "Unable to Find Patron", false, Gtk::MESSAGE_QUESTION);
		dialog.set_secondary_text("No known patron with ID: " + id);
		dialog.run();
	}
}

Store::~Store()
{
	if(store_window != NULL)
	{
		cout << "Deleting store window at: " << store_window << endl;
		delete store_window;
	}

	if(information != NULL)
	{
		cout << "Deleting info window at: " << information << endl;
		delete information;
	}

	cout << "Deleting machines..." << endl;
	cout << "Deleting patrons..." << endl;

    for(int i = 0; i < machines.size(); i++)
        delete(machines[i]);

    for(int i = 0; i < patrons.size(); i++)
        delete(patrons[i]);
}
