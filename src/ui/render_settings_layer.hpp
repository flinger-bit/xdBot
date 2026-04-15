#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class RenderSettingsLayer : public Popup<> , public TextInputDelegate {
protected:
    bool setup() override {
        this->setTitle("Render Settings");
        return true;
    }

public:
    static RenderSettingsLayer* create() {
        auto ret = new RenderSettingsLayer();
        if (ret && ret->initAnchored(396.f, 277.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void onClose(CCObject*) {
        this->onClose(nullptr);
    }
};
