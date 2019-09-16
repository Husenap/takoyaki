#pragma once

namespace ty{

class Widget{
public:
    void ToggleVisibility() { mVisibility = !mVisibility;}

protected:
    bool mVisibility;
};

}