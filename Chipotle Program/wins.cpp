//Lucas Streanga  1001612558

#include<gtkmm.h>
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>
#include"globals.h"
#include"chipotle.h"
#include"wins.h"

using namespace std;
using namespace win;

void win::switch_windows(Glib::RefPtr<Gtk::Application> app, Gtk::Window *first, Gtk::Window *second)
{
  app->add_window(*second);
  second->show_all();
  first->hide();
}

file_input::file_input(Glib::RefPtr<Gtk::Application> app) //Main window is the window we want to swtich to after config is done..
{
  this->app = app;
  main_window = nullptr;
  store = nullptr;
  resize(300,150);
  set_border_width(10);
  set_title("Config");
  set_position(Gtk::WIN_POS_CENTER);
  file_entry.set_text("<Drivers File>");

  okay.add_label("Continue");

  okay.signal_pressed().connect(sigc::mem_fun(*this, &file_input::get_info));
  okay.signal_pressed().connect(sigc::mem_fun(*this, &file_input::switch_to_main));

  layout.pack_start(file_entry);
  layout.pack_start(okay);

  layout.show_all();
  add(layout);
}

void file_input::switch_to_main()
{
  cout << "Config will switch to main window now...\n";
  store = new Chipotle(file);
  if(store->is_file_openable)
  {
  	main_window = new mainwin(app, store);
  	switch_windows(app, (Gtk::Window *)this, main_window);
  }
  else
  {
    Gtk::MessageDialog file_error(*this, "Error opening file",false,Gtk::MESSAGE_INFO);
    file_error.set_secondary_text("Unable to open file: " + store->drivers_file);
        file_error.run();
    cout << "Unable to open file error." << endl;
    //std::exit(1);
    this->hide();
  }
}

void file_input::get_info()
{
  file = file_entry.get_text();
}

file_input::~file_input()
{
  if(main_window)
  {
    cout << "Deleting main window at: " << main_window << endl;
    delete main_window;
  }
  else
  {
    //No main window to delete the store, so we should delete them
    if(store)
    {
      cout << "Deleting store at: " << store << endl;
      delete store;
    }
  }
}

//Now for the main window...
mainwin::mainwin(Glib::RefPtr<Gtk::Application> app, Chipotle *store)
{
  cout << "Creating main window..." << endl;
  this->store = store;
  this->app = app;

  resize(RES_X, RES_Y);
  set_border_width(10);
  set_title("Chipotle");
  set_position(Gtk::WIN_POS_CENTER);

  apply.add_label("Apply");
  order.add_label("Order");
  exit.add_label("Exit");

  //Signal handlers...
  exit.signal_pressed().connect(sigc::mem_fun(*this, &mainwin::exit_win));
  apply.signal_pressed().connect(sigc::mem_fun(*this, &mainwin::button_apply));
  order.signal_pressed().connect(sigc::mem_fun(*this, &mainwin::button_order));



  layout.pack_start(order);
  layout.pack_start(apply);
  layout.pack_start(exit);
  layout.show_all();
  add(layout);
/*
  if(!store->is_file_openable)
  {
    Gtk::MessageDialog file_error(*this, "Error opening file",false,Gtk::MESSAGE_INFO);
    file_error.set_secondary_text("Unable to open file: " + store->drivers_file);
		file_error.run();
    cout << "Unable to open file error." << endl;
    //std::exit(1);
	this->hide();
  }
*/

}

void mainwin::exit_win()
{
  //We need to print out the revenue and delivery
  Gtk::MessageDialog dialog(*this, "Profit Details", Gtk::MESSAGE_INFO);
  stringstream money;
  money << "Revenue: $" << setprecision(2) << fixed << store->revenue() << "\nDelivery Charges: $" << store->delivery_charge();
  dialog.set_secondary_text(money.str());
  dialog.run();

  cout << "Exit button pressed... exitting..." << endl;
  this->hide();
}

void mainwin::button_apply()
{
  //Create a dialog box with an entry box...
  Gtk::MessageDialog application(*this, "Please enter your full name", Gtk::MESSAGE_INFO);
  Gtk::Entry name;
  name.set_text("<name>");
  application.get_action_area()->pack_start(name, false, false, 0);
  application.show_all();

  int response = application.run();
  if(response == Gtk::RESPONSE_OK)
  {
    Gtk::MessageDialog appSuccessful(*this, "Application Status", Gtk::MESSAGE_INFO);
    stringstream s(name.get_text());
    string first, second;
    s >> first;
    s >> second;
    if(store->apply(first, second))
    {
      appSuccessful.set_secondary_text("Application Successful!");
      appSuccessful.run();
    }
    else
    {
      appSuccessful.set_secondary_text("Application Unsuccessful...");
      appSuccessful.run();
    }
  }

}

void mainwin::button_order()
{
  orderwin * OrderWindow = new orderwin(app, store, this);
  switch_windows(app, this, OrderWindow);
}

mainwin::~mainwin()
{
  //Delete our objects
  if(store)
  {
    cout << "Deleting store object at: " << store << endl;
    delete store;
  }
}

orderwin::orderwin(Glib::RefPtr<Gtk::Application> app, Chipotle *store, Gtk::Window *main_window)
{
  cout << "Creating order window..." << endl;
  this->store = store;
  this->app = app;
  this->main_window = main_window;

  resize(RES_X, RES_Y);
  set_border_width(10);
  set_title("Your Order");
  set_position(Gtk::WIN_POS_CENTER);
  bowl.set_label("Bowl?");
  tofu.set_label("Tofu?");
  chicken.set_label("Chicken?");
  steak.set_label("Steak?");
  white_rice.set_label("White Rice?");
  brown_rice.set_label("Brown Rice?");
  sour_cream.set_label("Sour Cream?");
  salsa.set_label("Salsa?");
  queso.set_label("Queso?");

  confirm.signal_pressed().connect(sigc::mem_fun(*this, &orderwin::button_confirm));

  confirm.add_label("Confirm Order");
  layout.pack_start(bowl);
  layout.pack_start(tofu);
  layout.pack_start(steak);
  layout.pack_start(chicken);
  layout.pack_start(white_rice);
  layout.pack_start(brown_rice);
  layout.pack_start(queso);
  layout.pack_start(sour_cream);
  layout.pack_start(salsa);
  layout.pack_start(confirm);
  layout.show_all();
  add(layout);

}

void orderwin::button_confirm()
{
  //We need to go through and see which buttons are pressed...
  bool is_bowl = false;
  int properties = 0;
  string properties_confirm;

  if(bowl.get_active())
  {
    is_bowl = true;
    properties_confirm += "Bowl:\n";
  }
  else
    properties_confirm += "Burrito:\n";
  if(tofu.get_active())
  {
    properties = properties | Order::TOFU;
    properties_confirm += "Tofu\n";
  }
  if(chicken.get_active())
  {
    properties = properties | Order::CHICKEN;
    properties_confirm += "Chicken\n";
  }
  if(steak.get_active())
  {
    properties = properties | Order::STEAK;
    properties_confirm += "Steak\n";
  }
  if(white_rice.get_active())
  {
    properties = properties | Order::WHITE_RICE;
    properties_confirm += "White Rice\n";
  }
  if(brown_rice.get_active())
  {
    properties = properties | Order::BROWN_RICE;
    properties_confirm += "Brown Rice\n";
  }
  if(queso.get_active())
  {
    properties = properties | Order::QUESO;
    properties_confirm += "Queso\n";
  }
  if(salsa.get_active())
  {
    properties = properties | Order::SALSA;
    properties_confirm += "Salsa\n";
  }
  if(sour_cream.get_active())
  {
    properties = properties | Order::SOUR_CREAM;
    properties_confirm += "Sour Cream\n";
  }

  Gtk::MessageDialog dialog(*this, "Order Confirmation",false,Gtk::MESSAGE_INFO, Gtk::BUTTONS_YES_NO);
  dialog.set_secondary_text(properties_confirm);
  int num = dialog.run();

  if(num == Gtk::RESPONSE_YES)
  {
    //The order is correct
    Order o;
    o.build(properties, is_bowl);
    Gtk::MessageDialog delivery(*this, "Delivery",false, Gtk::MESSAGE_INFO);

    if(!store->drivers.empty()) //There are drivers...
    {
      o.is_deliverable = true;
      cout << "Ok, " << store->drivers.front().first << " will be delivering your order." << endl;
      delivery.set_secondary_text(store->drivers.front().first + " will be delivering your order.");
      delivery.run();
      store->drivers.pop();
    }
    else
    {
      delivery.set_secondary_text("No available drivers at this time!");
      delivery.run();
      cout << "No available drivers at this time!" << endl;
    }
    store->orders.push_back(o);
  }

  switch_windows(app, this, main_window);
  cout << "Deleting this order window at: " << this;
  delete this;

}
