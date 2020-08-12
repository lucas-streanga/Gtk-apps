#include<iostream>
#include<gtkmm.h>
#include<vector>
#include<string>

#define KEYBOARD_FILE "keyboard.xml"

using namespace std;

class keyboard
{
public:

    Gtk::Window * win;
    Gtk::Entry * name;
    Gtk::Box *line1;
    Gtk::Box *line2;
    Gtk::Box *line3;
    Gtk::Box *keys;
	  Gtk::Button del;
	  Gtk::Button *clear;
	  Gtk::Button *confirm;
    string user_input;

    vector <Gtk::Button *> buttons;

    void clicked(Gtk::Button *b)
    {
        string cur_label = name->get_text();
        name->set_text(cur_label + b->get_label());
		    rightJustified();
    }

    void delKey()
	  {
		    string cur = name->get_text();
		    if(cur.size() != 0)
		    {
			       cur.erase(cur.size() - 1, 1);
			       name->set_text(cur);
		    }
		    rightJustified();
	  }

	void butClear()
	{
		name->set_text("");
	}

	void butConfirm()
	{
		user_input = name->get_text();
		win->hide();
	}

	void rightJustified()
	{
		name->set_position(name->get_text().size());
	}

    keyboard()
    {
        Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(KEYBOARD_FILE);
        builder->get_widget("keyboard", win);
        builder->get_widget("name", name);
        builder->get_widget("keys", keys);
        builder->get_widget("line1", line1);
        builder->get_widget("line2", line2);
        builder->get_widget("line3", line3);
		    builder->get_widget("clear", clear);
		    builder->get_widget("confirm", confirm);

		    clear->signal_clicked().connect(sigc::mem_fun(*this, &keyboard::butClear));
		    confirm->signal_clicked().connect(sigc::mem_fun(*this, &keyboard::butConfirm));
		    clear->set_focus_on_click(false);
		    name->set_activates_default(true);

		    del.set_label("<-");
		    del.set_focus_on_click(false);
		    del.set_border_width(2);

        char key = 'A';

        for(int i = 0; i < 26; i++)
        {
            Gtk::Button *b = new Gtk::Button(to_string(i+1));
            string lab(1, key);
            b->set_label(lab);
            b->set_alignment(.5,.5);
			      b->set_focus_on_click(false);
			      b->set_border_width(2);
            //cout << "Created button with label: " << b->get_label() << endl;
            if(i < 10)
                line1->pack_start(*b);
            else if (i < 19)
                line2->pack_start(*b);
            else
                line3->pack_start(*b);

            buttons.push_back(b);
            b->signal_clicked().connect(sigc::bind<Gtk::Button *>(sigc::mem_fun(*this, &keyboard::clicked), b));
            key++;
        }
		    del.signal_clicked().connect(sigc::mem_fun(*this, &keyboard::delKey));
		    line3->pack_start(del);
        keys->show_all();
        line1->show_all();
        line2->show_all();
        line3->show_all();
    }
};

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create(argc, argv, "");
    keyboard k;
    app->run(*(k.win));
    cout << k.user_input << endl;
}
