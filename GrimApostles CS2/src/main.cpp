#include "gui.h"

int main() {
	gui::initializeGUI();
	//Game logic will be found in the render function
	gui::Render();
	return 0;
}


