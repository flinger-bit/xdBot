#pragma once

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class SaveMacroLayer : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("Save Macro");
        return true;
    }

public:
    static SaveMacroLayer* create() {
        auto ret = new SaveMacroLayer();
        if (ret && ret->initAnchored(300.f, 200.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
