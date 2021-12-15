#include "include/dialog.h"

using namespace Tower_Defence;

int main() {
	Landscape land;
	
	int choice;
	while (true) {
		choice = dialog(land);
		if (!fptr[choice](land)) break;
	}

	return 0;
}