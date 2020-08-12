//Lucas Streanga  1001612558
//Header file for our windows for the chipotle program...
#ifndef _WINS_CHIPOTLE_H
#define _WINS_CHIPOTLE_H

#include<gtkmm.h>
#include"globals.h"
#include"chipotle.h"

namespace win //We will define our windows in this namespace...
{
  //function to switch windows in our application...
  void switch_windows(Glib::RefPtr<Gtk::Application> app, Gtk::Window *first, Gtk::Window *second);

  class file_input : public Gtk::Window
  {
  public:

    Gtk::VBox layout;
    Gtk::Entry file_entry;
    Gtk::Button okay;
    Glib::RefPtr<Gtk::Application> app;
	Chipotle *store;
    Gtk::Window *main_window;
    std::string file;

    file_input(Glib::RefPtr<Gtk::Application> app);
    void get_info();
    void switch_to_main();
    ~file_input();
  };

  class mainwin : public Gtk::Window
  {
  public:

    Gtk::VBox layout;
    Gtk::Button apply;
    Gtk::Button order;
    Gtk::Button exit;
    Glib::RefPtr<Gtk::Application> app;
    Chipotle *store;

    mainwin(Glib::RefPtr<Gtk::Application> app, Chipotle *store);
    void set_store(Chipotle *store);
    void exit_win();
    void button_apply();
    void button_order();
    ~mainwin();
  };

  class orderwin : public Gtk::Window
  {
  public:

    Gtk::VBox layout;
    Gtk::CheckButton bowl;
    Gtk::CheckButton tofu;
    Gtk::CheckButton chicken;
    Gtk::CheckButton steak;
    Gtk::CheckButton white_rice;
    Gtk::CheckButton brown_rice;
    Gtk::CheckButton queso;
    Gtk::CheckButton sour_cream;
    Gtk::CheckButton salsa;
    Gtk::Button confirm;
    Glib::RefPtr<Gtk::Application> app;
    Chipotle *store;
    Gtk::Window *main_window;

    orderwin(Glib::RefPtr<Gtk::Application> app, Chipotle *store, Gtk::Window *main_window);
    void button_confirm();

  };

}

#endif
