#pragma once

#include <string>

namespace ui {

class Input {
    public:
        Input(std::string title, float width, char* text):
            title(title),
            width(width),
            text(text)
        {};

        void draw(int index, float y, float init_pos);

    private:
        const float width;

        std::string title;
        bool editMode = false;
        char* text;
};

}
