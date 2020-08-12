//Lucas Streanga  1001612558
//This is where main will be held...

#include<iostream>
#include<gtkmm.h>
#include"globals.h"
#include"chipotle.h"
#include"wins.h"

int main(int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv);
  Gtk::Window *mainwin = nullptr;
  win::file_input *config = new win::file_input(app);
  app->run(*config);
  delete config;
}
