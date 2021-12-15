#pragma once

#ifndef __DIALOG__
#define __DIALOG__

#include <Windows.h>
#include "All_Header.h"

namespace Tower_Defence {

	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	template <class T>
	int getNum(T& a, const char* msg, std::istream& c = std::cin) {
		std::cout << msg;
		do {
			c >> a;
			if (c.eof()) {
				std::cout << std::endl << "End of input" << std::endl;
				return 0;
			}
			if (!c.good() || c.peek() != '\n') {  // input error     
				c.clear();
				//c.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				c.ignore(1000, '\n');  // for some reason, the top line does not work when the Windows.h library is connected
				std::cout << std::endl << "Try again!" << std::endl;
			}
			else {
				c.ignore();  // delete 1 symbol ('\n') from stream
				return 1;
			}
		} while (1);
	}
	

	// array of dialog functions for the game
	int (*fptr[])      (Landscape&);

	int next_turn      (Landscape&);
	int build_tower    (Landscape&);
	int build_wall     (Landscape&);
	int level_up_tower (Landscape&);
	int level_up_castle(Landscape&);
	int repair_wall    (Landscape&);
	int quit           (Landscape&);

	int  dialog(Landscape&);  
	bool get_yes_no_answer();
	void clear();
	void print_money(Landscape&);
	void click_enter();
	void game_over();
	void game_won();

	void print_map_in_console(Landscape&, int coord = -1);

}

#endif __DIALOG__