#include <ofMain.h>

#include "controller_app.h"


int main() {
	ofSetupOpenGL(1024, 768, OF_WINDOW);

	ofRunApp(new ControllerApp());
}
