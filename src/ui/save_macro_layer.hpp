#pragma once

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class SaveMacroLayer : public Popup {
protected:
    bool init() {
        if (!Popup::init(300.f, 200.f))
            return false;

        this->setTitle("Save Macro");

        return true;
    }

public:
    static SaveMacroLayer* create() {
        auto ret = new SaveMacroLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
