#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

#include "audio_mngr.h"

using namespace sf;
using namespace std;

mutex mtx;

SoundBuffer __Buffer__;
Time __TotalDuration__ ;
float __CurrentTime__ ;
Sound __Sound__;

vector <string> __playlist_queu__ ;
int __curr = -1;

bool __stopped = false;

bool changeMusic(std::string path);



void keepThreadAlive(){
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        __CurrentTime__ = __Sound__.getPlayingOffset().asSeconds();
        if (__Sound__.getStatus() == Sound::Stopped && 
            __curr != __playlist_queu__.size() - 1 && 
            __playlist_queu__.size()&&
            !__stopped)
        {
            next();
        }
        
        
    }
}

void initAudioThread(){
    thread musicThread(keepThreadAlive);
    musicThread.detach();
    cout << "tmam\n";

}

bool loadMusicFile(string path){
    std::lock_guard<std::mutex> lock(mtx);
    __Sound__.stop();
    if (!__Buffer__.loadFromFile(path)) {
    return false;
  }
  __TotalDuration__ = __Buffer__.getDuration();
  __Sound__.setBuffer(__Buffer__);
  return true;

}

bool changeMusic(string path){
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
    __stopped = true ;
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

void addToList(string filepath, int index){
    std::lock_guard<std::mutex> lock(mtx);
    if (index <= __curr)
    {
        __curr++;
    }
    __playlist_queu__.insert(__playlist_queu__.begin() + index , filepath);

}
void addToList(string filepath){
    {
        lock_guard<std::mutex> lock(mtx);
        __playlist_queu__.push_back(filepath);
    }
    if (__playlist_queu__.size() == 1)
    {
        changeMusic(filepath);
        __curr = 0;
    }
    
    
}

void removeFromList(int index){
    if(index > __playlist_queu__.size() || index < 0)
        return;


    mtx.lock();
        __playlist_queu__.erase(__playlist_queu__.begin() + index);
    mtx.unlock();

    if (index == __curr)
    {
        if (__curr != __playlist_queu__.size() && __playlist_queu__.size() != 0)
        {
            changeMusic(__playlist_queu__[__curr ]);
        }else
            stop();
        
    }if (index < __curr)
    {
        mtx.lock();
            __curr--;
        mtx.unlock();
    }
    


}
void next(){
    {
        std::lock_guard<std::mutex> lock(mtx);
        __curr = (__curr+1) % __playlist_queu__ .size();
    }
    changeMusic(__playlist_queu__[__curr]);


}

void prev(){
    if (__curr != 0 )
    {
        mtx.lock();
            --__curr;
        mtx.unlock();
        changeMusic(__playlist_queu__[__curr]);
    }

}



void change_list_order(int old_index , int new_index){
    if(old_index > __playlist_queu__.size() || 
       old_index < 0 || 
       new_index > __playlist_queu__.size() || 
       new_index < 0)
        return;
    
    addToList(__playlist_queu__[old_index] , new_index);
    removeFromList(old_index);
}
