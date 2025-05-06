#include "search_select.hpp"


#include "../include/raygui.h"
#include <raylib.h>

namespace ui {

bool SearchSelect::StrContains(const char *s, const char *sub)
{
	return (TextLength(sub) == 0) || (TextFindIndex(s, sub) >= 0);
}

void SearchSelect::before_draw() {
    filtIdx.clear();
    for (std::size_t i = 0; i < options.size(); ++i)
        if (StrContains(options[i].c_str(), search.c_str()))
            filtIdx.push_back(static_cast<int>(i));

    filtCnt = static_cast<int>(filtIdx.size());
    if (activeIndex >= filtCnt) activeIndex = -1;
}

void SearchSelect::draw() {
    Rectangle searchRect = { 16, 16, 180, 24 };
    if (GuiTextBox(searchRect, search.data(), sizeof(search), editSearch))
        editSearch = !editSearch;

    Rectangle listRect = { 16, 48, 180, 100 };
    std::vector<const char*> view;
    for (int i = 0; i < filtCnt; ++i)
        view.push_back(options[filtIdx[i]].c_str());

	int prevActive = activeIndex;
    GuiListViewEx(listRect, view.data(), filtCnt,
                  &scrollIndex, &activeIndex, &focusIndex);

    if (activeIndex != prevActive && activeIndex >= 0) {
        selectedOrig = filtIdx[activeIndex];
        search = options.at(selectedOrig);
    }

    GuiLabel((Rectangle){ 220, 16, 180, 24 },
             (selectedOrig >= 0)
               ? TextFormat("Selected: %s", options[selectedOrig].c_str())
               : "Selected: <none>");
}

}
