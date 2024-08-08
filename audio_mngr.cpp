#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "audio_mngr.h"

using namespace sf;
using namespace std;

mutex mtx;

SoundBuffer __Buffer__;
Time __TotalDuration__ ;
float __CurrentTime__ ;
Sound __Sound__;


bool loadMusicFile(string path){
    __Sound__.stop();
    if (!__Buffer__.loadFromFile(path)) {
    return false;
  }
  __TotalDuration__ = __Buffer__.getDuration();
  __Sound__.setBuffer(__Buffer__);
  return true;

}

bool changeMusic(string path){
    stop();
    if(!loadMusicFile(path)) return false;

    play();
    return true ;
}

float getTimeInSec(){
    return __TotalDuration__.asSeconds();
}

float getCurrTime(){
    return __CurrentTime__;
}

void play(){
    std::lock_guard<std::mutex> lock(mtx);
    __Sound__.play();
}


void stop(){
    std::lock_guard<std::mutex> lock(mtx);
    __Sound__.stop();
}

void pause(){
    std::lock_guard<std::mutex> lock(mtx);
    __Sound__.pause();
}

void toggleMusic(){
    if (isPlaying())
        pause();
    else play();
}

bool isPlaying(){
    return __Sound__.getStatus() == Sound::Playing;
}

void playMusic() {
    
    play();
    initAudioThread ();
}
void keepThreadAlive(){
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        __CurrentTime__ = __Sound__.getPlayingOffset().asSeconds();
    }
}
void initAudioThread (){
    thread musicThread(keepThreadAlive);
    musicThread.detach();

}

