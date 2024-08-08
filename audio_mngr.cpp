#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace sf;
using namespace std;

SoundBuffer __Buffer__;
Time __TotalDuration__ ;
Time __CurrentTime__ ;
Sound __Sound__;

bool loadMusicFile(string path){
    if (!__Buffer__.loadFromFile(path)) {
    return false;
  }
  __TotalDuration__ = __Buffer__.getDuration();
  __Sound__.setBuffer(__Buffer__);
  return true;

}

/*
* pre : MusicFile loaded successfully
* return : The total duration of the loaded audio file
* return type : float
*/
float getTimeInSec(){
    return __TotalDuration__.asSeconds();
}

/*
* pre : MusicFile loaded successfully
*/
void play(){
    __Sound__.play();
}

/*
* pre : MusicFile loaded successfully
*/
void stop(){
    __Sound__.stop();
}

/*
* pre : MusicFile loaded successfully
*/
bool isPlaying(){
    return __Sound__.getStatus() == Sound::Playing;
}


