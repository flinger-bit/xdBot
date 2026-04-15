#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class RenderSettingsLayer : public Popup, public TextInputDelegate {
protected:
    bool init() {
        if (!Popup::init(396.f, 277.f))
            return false;

        this->setTitle("Render Settings");

        return true;
    }

public:
    static RenderSettingsLayer* create() {
        auto ret = new RenderSettingsLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
