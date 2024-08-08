
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

#include "../audio_mngr.h"

#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border

using namespace ftxui;


ScreenInteractive screen = ScreenInteractive::FitComponent();


ButtonOption Style() {
    auto option = ButtonOption::Animated();
    option.transform = [](const EntryState& s) {
        auto element = text(s.label);
        if (s.focused) {
            element |= bold;
        }
        return element | center | borderEmpty | flex;
    };
    return option;
}

int main() {
    initAudioThread();
    

    std::string button_label = "Play";

    auto button = Button(&button_label, [&] {
        if (isPlaying()) {
            toggleMusic();
            button_label = "Play";
        } else {
            toggleMusic();
            button_label = "Pause";
        }
    }, Style());

    auto component = Renderer(button, [&] {
        return vbox({
                text( std::to_string((int)getTimeInSec()) +" / "+ std::to_string((int)getCurrTime())),
                   button->Render() | flex,
               }) | flex | border;
    });
    changeMusic("/home/kali/src/audio_cpp/1000.wav");
    screen.Loop(component);


    return 0;
}
