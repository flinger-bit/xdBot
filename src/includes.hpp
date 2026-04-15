#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Event.hpp>

// ... tus otros includes ...

using namespace geode::prelude;

// ... resto igual ...

class Global {
    Global() {}
public:
    static auto& get() {
        static Global instance;
        return instance;
    }

    // ... tus declaraciones iguales ...

    geode::Popup* layer = nullptr;

    // ... resto igual ...
};
