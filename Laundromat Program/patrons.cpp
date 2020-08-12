#include<vector>
#include<iostream>
#include<gtkmm.h>
#include<iomanip>
#include<sstream>
#include<fstream>

//#include"store.h"
#include"patrons.h"
#include"machines.h"

using namespace std;

Person::Person(string name, string id, double balance, double det_available)
{
	this->name = name;
	this->id = id;
	this->balance = balance;
	this->det_available = det_available;
}

Machine * Person::wash(Machine *m) 
{
	if(m == NULL) //No machine passed in...
	{
		cout << "Machine not available..." << endl;
		Gtk::MessageDialog dialog(*patron_window, "Machine Not Available",false, Gtk::MESSAGE_INFO);
		dialog.run();
	}
	else
	{
		if(check_price(m) && check_det(m) && !m->in_use)
		{
			cout << "Washing..." << endl;
			m->in_use = true;
			balance -= m->cost;
			det_available -= m->det_cost;
			machines_in_use.push_back(m);
			update_resources();
		}
	}
	return m;
}

void Person::update_resources()
{
	if(bal_label != NULL && det_label != NULL)
	{
		stringstream s_bal;
		stringstream s_det;
		s_bal << fixed << setprecision(2) << balance;
		s_det << fixed << setprecision(2) << det_available;

		bal_label->set_label("Balance Available: $" + s_bal.str());
		det_label->set_label("Detergent Available: " + s_det.str() + " oz");
	}
}

void Person::available()
{
	int slow = 0;
	int very_slow = 0;
	int fast = 0;
    cout << "\n***Available machines:" << endl;
    for(int i = 0; i < machines_available.size(); i++)
	{
		if(!machines_available[i]->in_use)
		{
			if(machines_available[i]->speed == VERY_SLOW)
				very_slow++;
			else if(machines_available[i]->speed == SLOW)
				slow++;
			else
				fast++;
		}

	}

		cout << "Fast Machines: " << fast << "\nSlow Machines: " << slow << "\nVery Slow Machines: " << very_slow << endl;

	if(num_fast != NULL && num_slow != NULL && num_very_slow != NULL);
	{
		num_fast->set_label(to_string(fast));
		num_slow->set_label(to_string(slow));
		num_very_slow->set_label(to_string(very_slow));
	}
		
}

bool Person::check_price(Machine *m)
{
	if(m->cost <= balance)
		return true;
	else
	{
		Gtk::MessageDialog dialog(*patron_window, "Insufficient Funds",false,Gtk::MESSAGE_INFO, Gtk::BUTTONS_YES_NO);
		dialog.set_secondary_text("Would you like to deposit?");
		int num = dialog.run();

		if(num == Gtk::RESPONSE_YES)	
		{
			Gtk::MessageDialog d2(*patron_window, "How Much?", Gtk::MESSAGE_INFO);
			Gtk::Entry how_much;
			how_much.set_text("Dollars");
			d2.get_action_area()->pack_start(how_much, false, false, 0);
			d2.show_all();

			int response = d2.run();
			if(response == Gtk::RESPONSE_OK)
			{
				double funds;
				try
				{
					funds = stod(how_much.get_text());
				}
				catch(invalid_argument)
				{
					cout << "Invalid value" << endl;
					Gtk::MessageDialog d3(*patron_window, "Invalid Input", Gtk::MESSAGE_INFO);
					d3.run();
				}
				balance += funds;
				if(balance >= m->cost)
					return true;
				else
				{
					update_resources();
					return false;
				}
			}

		}
		return false;

	}
}

bool Person::check_det(Machine *m)
{
	if(m->det_cost <= det_available)
		return true;
	else
	{
		Gtk::MessageDialog dialog(*patron_window, "Insufficient Detergent",false,Gtk::MESSAGE_INFO, Gtk::BUTTONS_YES_NO);
		dialog.set_secondary_text("Would you like to get more?");
		int num = dialog.run();

		if(num == Gtk::RESPONSE_YES)	
		{
			Gtk::MessageDialog d2(*patron_window, "How Much?", Gtk::MESSAGE_INFO);
			Gtk::Entry how_much;
			how_much.set_text("Ounces");
			d2.get_action_area()->pack_start(how_much, false, false, 0);
			d2.show_all();

			int response = d2.run();
			if(response == Gtk::RESPONSE_OK)
			{
				double new_det;
				try
				{
					new_det = stod(how_much.get_text());
				}
				catch(invalid_argument)
				{
					cout << "Invalid value" << endl;
					Gtk::MessageDialog d3(*patron_window, "Invalid Input", Gtk::MESSAGE_INFO);
					d3.run();
				}
				det_available += new_det;
				if(det_available >= m->det_cost)
					return true;
				else
				{
					update_resources();
					return false;
				}
			}

		}
		return false;

	}
}

void Person::pickup()
{

    if(machines_in_use.size() > 0)
    {
        machines_in_use[machines_in_use.size() - 1]->in_use = false; //Set it to be free again
        Machine *m = machines_in_use[machines_in_use.size() - 1];
		machines_in_use.pop_back();

		Gtk::MessageDialog dialog(*patron_window, "Would you like a receipt?",false,Gtk::MESSAGE_INFO, Gtk::BUTTONS_YES_NO);

		int num = dialog.run();
		if(num == Gtk::RESPONSE_YES)
			receipt(m);
    }
    else
    {
		Gtk::MessageDialog dialog(*patron_window, "No loads to pick up...", false,Gtk::MESSAGE_INFO);
		dialog.run();
	}

	available();
            
}

Machine * Person::find_available_machine(int speed)
{
    for(int i = 0; i < machines_available.size(); i++)
    {
        if(machines_available[i]->speed == speed && !machines_available[i]->in_use)
            return machines_available[i];        
	}

	return NULL;
}

void Person::run(Glib::RefPtr<Gtk::Application> app, Store *this_store)
{
	store = this_store;
	application = app;
	store_window = store->store_window;

	cout << "Running on patron: " << id << endl;

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(PATRON_XML);
	builder->get_widget("patron", patron_window);
	builder->get_widget("num_fast", num_fast);
	builder->get_widget("num_slow", num_slow);
	builder->get_widget("num_very_slow", num_very_slow);
	builder->get_widget("wash", wash_button);
	builder->get_widget("pickup", pickup_button);
	builder->get_widget("member", member_label);
	builder->get_widget("back", back);
	builder->get_widget("det_label", det_label);
	builder->get_widget("bal_label", bal_label);

	patron_window->resize(RES_X, RES_Y);
	patron_window->set_title("Patron " + id);
	member_label->set_label("Welcome " + member + " Member");
	available();
	update_resources();

	app->add_window(*patron_window);
	store_window->hide();	

	if(machines_in_use.size() > 0) //They have something to pick up
	{
		Gtk::MessageDialog dialog(*patron_window, "Load To Pick Up", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_YES_NO);
		dialog.set_secondary_text("You currently have " + to_string(machines_in_use.size()) + " laundry loads\nWould you like to pick up?");

		int result = dialog.run();

		if(result == Gtk::RESPONSE_YES)
			pickup();
	}
	wash_button->signal_clicked().connect(sigc::mem_fun(*this, &Person::choose_machine));
	back->signal_clicked().connect(sigc::mem_fun(*this, &Person::go_back));
	pickup_button->signal_clicked().connect(sigc::mem_fun(*this, &Person::pickup));               

}

void Person::go_back()
{
	application->add_window(*store_window);
	store->available();
	store_window->show();
	patron_window->hide();
	cout << "Deleting patron window at: " << patron_window << endl;
	delete patron_window;
}

void Person::receipt(Machine *m)
{
	string filename;
	filename = filename + id;
	filename = filename + "-receipt.txt";

	ofstream receipt;
	receipt.open(filename, ios_base::app);

	if(!receipt.is_open())
		cout << "Unable to write receipt: " << filename << endl;
	else
	{
		receipt << fixed << setprecision(2) << name << ": paid " << m->cost << endl;
		cout << "Receipt written. Thank you for your business!" << endl;
	}

	receipt.close();
}

Discount::Discount(string name, string id, double balance, double det_available, vector <Machine *> machines):Person(name, id, balance, det_available)
{
	//We need to copy the pointers of the machines that are slow 

	for(int i = 0; i < machines.size(); i++)
	{
		if(machines[i]->speed == VERY_SLOW || machines[i]->speed == SLOW) //As in, is very slow or slow;
			machines_available.push_back(machines[i]);
	}

	member = "discount";
	access = 1;
}

void Discount::choose_machine()
{

	//We need to create an appropriate dialog box for choosing a speed...

	Gtk::MessageDialog dialog(*patron_window, "Please Choose a Speed", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_NONE);	
	dialog.add_button("Very Slow", 1);
	dialog.add_button("Slow", 2);
	
	int num = dialog.run();
	if(num == 1)
		wash(find_available_machine(VERY_SLOW));
	else if(num == 2)
		wash(find_available_machine(SLOW));

	available();

}

Regular::Regular(string name, string id, double balance, double det_available, vector <Machine *> machines):Person(name,id, balance, det_available)
{
	for(int i = 0; i < machines.size(); i++)
    {
        if(machines[i]->speed == SLOW || machines[i]->speed == FAST) //As in, slow or fast
            machines_available.push_back(machines[i]);
    }

    member = "regular";
    access = 2;
}

void Regular::choose_machine()
{
    Gtk::MessageDialog dialog(*patron_window, "Please Choose a Speed", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_NONE);	
	dialog.add_button("Slow", 1);
	dialog.add_button("Fast", 2);
	
	int num = dialog.run();
	if(num == 1)
		wash(find_available_machine(SLOW));
	else if(num == 2)
		wash(find_available_machine(FAST));

	available();
                
}

Elite::Elite(string name, string id, double balance, double det_available, vector <Machine *> machines):Person(name, id, balance, det_available)
{
	for(int i = 0; i < machines.size(); i++)
    {
        machines_available.push_back(machines[i]); // all machines are available
    }

    member = "elite";
    access = 3;
}

void Elite::choose_machine()
{
    Gtk::MessageDialog dialog(*patron_window, "Please Choose a Speed", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_NONE);	
	dialog.add_button("Very Slow", 1);
	dialog.add_button("Slow", 2);
    dialog.add_button("Fast", 3);
	
	int num = dialog.run();
	if(num == 1)
		wash(find_available_machine(VERY_SLOW));
	else if(num == 2)
		wash(find_available_machine(SLOW));
    else if(num == 3)
        wash(find_available_machine(FAST));
    
	available();
}


