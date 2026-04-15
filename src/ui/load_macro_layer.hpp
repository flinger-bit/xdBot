#pragma once

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class LoadMacroLayer : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("Load Macro");
        return true;
    }

public:
    static LoadMacroLayer* create() {
        auto ret = new LoadMacroLayer();
        if (ret && ret->initAnchored(300.f, 200.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
