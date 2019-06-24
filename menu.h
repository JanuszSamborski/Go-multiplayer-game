#include<iostream>
#include<vector>
#include<fcntl.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<termios.h>
#include<sys/ioctl.h>

using namespace std;

int getch()
{
	int c = 0;

	struct termios org_opts, new_opts;
	int res = 0;

	res = tcgetattr(STDIN_FILENO, &org_opts);
	assert(res == 0);

	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
	c = getchar();

	res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res == 0);
	return(c);
}

class menu
{

	protected:

	vector<string> options;
	int amOfOpt, choice, result;
	bool accepted;

	void drawMenu()
	{
		int i;

		for(i = 0; i < amOfOpt; ++i)
		{
			if(i != choice)
				centerText(options[i], i + 2);
			else
			{
				cout<<"\033[1;32m";
				centerText('[' + options[i] + ']', i+2);
				cout<<"\033[0m\n";
			}
		}
	}

	void getInput()
	{
		int g;

		g = getch();
		if(g == 27)
		{
			getch();
			g = getch();
		}

		if(g == 65 || g == 119)			// UP || W
		{
			--choice;
			accepted = 0;
		}
		else if(g == 66 || g == 115)	// DOWN || S
		{
			++choice;
			accepted = 0;
		}
		else if(g == 10 || g == 32)		// ENTER || SPACE
			accepted = 1;

		if(choice < 0)
			choice = amOfOpt - 1;
		else if(choice >= amOfOpt)
			choice = 0;
	}

	void placeCursor(int x, int y)
	{
		cout<<"\033[" + to_string(y) + ";" + to_string(x) + "H";
	}

	void centerText(string s, int row)
	{
		int columns, where;
		struct winsize size;
		
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

		columns = size.ws_col + 1;

		where = columns/2 - s.size()/2;

		placeCursor(where, row);

		cout<<s;
	}

public:

	menu()
	{
		accepted = 0;
		choice = 0;
	}

	void setOptions(string s[], int siz)
	{
		options.assign(s, s+siz);
		amOfOpt = options.size();
	}

	/*
	*	A graphical way of asking user for a value
	*		
	*	\return int corresponding to chosen option
	*/
	int ask()
	{
		while(accepted != 1)
		{
			drawMenu();
			getInput();
			system("clear");
			//cout<<"\033[2J\033[1;1H";
		}

		accepted = 0;
		result = choice;
		choice = 0;
		return result;
	}
};