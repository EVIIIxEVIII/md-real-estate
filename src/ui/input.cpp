#include <raylib.h>
#include "../include/raygui.h"

#include "input.hpp"

namespace ui {

void Input::draw(int index, float y, float init_pos) {
	float x = index * (width + 20) + init_pos;
	GuiLabel((Rectangle){ x, y - 24, width, 24 }, title.c_str());

	Rectangle inputBox = { x, y, width, 24 };
	if(GuiTextBox(inputBox, text, sizeof(text), editMode)) {
        editMode = !editMode;
    }
}

}
