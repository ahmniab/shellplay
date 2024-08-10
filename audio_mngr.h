#pragma once

void initAudioThread();
float getTimeInSec();
float getCurrTime();
bool changeMusic(std::string path);
void play();
void pause();
void stop(); // DON'T USE STOP AT ALL (I create it to close music stream and exit) 
bool isPlaying();
void playMusic();
void toggleMusic();
void addToList(std::string filepath);
void addToList(std::string filepath,int index);
void removeFromList(int index);
void change_list_order(int old_index , int new_index);
void next();
void prev();
