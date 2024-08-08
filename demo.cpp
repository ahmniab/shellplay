#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border

using namespace ftxui;

std::atomic<bool> is_playing(false);
std::mutex mtx;
sf::Sound sound;
float total_time;
float current_time;
ScreenInteractive screen = ScreenInteractive::FitComponent();


void playMusic(const std::string& filepath /*, ScreenInteractive screen*/) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "Error loading audio file" << std::endl;
        return;
    }
    total_time =buffer.getDuration().asSeconds();
    

    // Set the buffer and play the sound
    {
        std::lock_guard<std::mutex> lock(mtx);
        sound.setBuffer(buffer);
        sound.play();
        is_playing = true;
    }

    // Keep the application running long enough for the sound to play
    while (sound.getStatus() == sf::Sound::Playing || sound.getStatus() == sf::Sound::Paused) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        current_time = sound.getPlayingOffset().asSeconds();
        screen.PostEvent(Event::Custom);
    }
    
  
}

void toggleMusic(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(mtx);
    if (is_playing) {
        sound.pause();
        is_playing = false;
    } else {
        if (sound.getStatus() == sf::Sound::Paused) {
            sound.play();
        } 
        is_playing = true;
    }
}

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
    std::string filepath = "/home/kali/src/audio_cpp/1000.wav";
    std::thread musicThread(playMusic, filepath );
    musicThread.detach();
        

    std::string button_label = "Play";

    auto button = Button(&button_label, [&] {
        if (is_playing) {
            toggleMusic(filepath);
            button_label = "Play";
        } else {
            toggleMusic(filepath);
            button_label = "Pause";
        }
    }, Style());

    auto component = Renderer(button, [&] {
        return vbox({
                text( std::to_string(total_time) +" / "+ std::to_string(current_time)),
                   button->Render() | flex,
               }) | flex | border;
    });

    screen.Loop(component);
    if (musicThread.joinable()) {
    musicThread.join();
  }

    return 0;
}
