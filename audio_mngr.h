#pragma once

void initAudioThread();
bool changeMusic(std::string path);
float getTimeInSec();
void play();
void pause();
void stop();
bool isPlaying();
void playMusic();
void toggleMusic();