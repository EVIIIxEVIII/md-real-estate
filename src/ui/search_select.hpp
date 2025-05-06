#pragma once

#include <raylib.h>
#include <string>
#include <vector>

namespace ui {

class SearchSelect {

public:
    SearchSelect(std::vector<std::string> options, std::string title, int* selected):
        options(std::move(options)),
        filtCnt(options.size()),
        title(title),
        selectedOrig(selected)
    {
        filtIdx.resize(this->options.size());
    };

    void before_draw();
    void draw(int index, float y, float init_pos);
    static bool StrContains(const char *s, const char *sub);

private:
    std::string search = "";
    std::vector<std::string> options;

    bool editSearch   = false;
    int  scrollIndex  = 0;
    int  activeIndex;
    int  focusIndex   = -1;
    int* selectedOrig;

    std::vector<int> filtIdx;
	int  filtCnt = 0;
    const std::string title;

};
}
