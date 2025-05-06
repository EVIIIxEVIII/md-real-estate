#pragma once

#include "../include/raygui.h"
#include <raylib.h>
#include <string>
#include <vector>

namespace ui {

class SearchSelect {

public:
    SearchSelect(std::vector<std::string> options):
        options(std::move(options)),
        filtCnt(options.size())
    {
        filtIdx.resize(this->options.size());
    };

    void before_draw();
    void draw(int index, float y);
    static bool StrContains(const char *s, const char *sub);

private:
    std::string search = "";
    std::vector<std::string> options;

    bool editSearch   = false;
    int  scrollIndex  = 0;
    int  activeIndex  = -1;
    int  focusIndex   = -1;
    int  selectedOrig = -1;

    std::vector<int> filtIdx;
	int  filtCnt = 0;

};
}
