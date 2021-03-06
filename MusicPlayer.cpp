#include "Arduino.h"
#include "MusicPlayer.h"
#include "pitches.h"

MusicPlayer::MusicPlayer(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  currState = NO_TRACK_PLAYING;
}

void MusicPlayer::init(bool bMusicPlayerActive)
{
  _disableSound = !bMusicPlayerActive;
}

void MusicPlayer::playTrack(unsigned int track[][3], unsigned int numNotes)
{
  _track = track;
  currNote = 0;
  _numNotes = 19;
  currState = ON_FOR;
  prevTime = millis();
  currDuration = _track[currNote][ON_FOR];
  tone(_pin, _track[currNote][PITCH]);
}

void MusicPlayer::updateTrack()
{
  unsigned long currTime = millis();
  //Serial.println(currState, DEC);
  
  switch(currState)
  {
    case ON_FOR:
    {
      // if current note is done playing then pause between notes
      if(int(currTime - prevTime) >= currDuration)
      {
        currDuration = _track[currNote][OFF_FOR];
        currState = OFF_FOR;
        prevTime = currTime;
        // stop the tone playing:
        noTone(_pin);
      }
      // else Continue playing current note
      break;
    }
    case OFF_FOR:
    {
      // if current delay is done then play next note
      if(int(currTime - prevTime) >= currDuration)
      {
        currNote++;
        // if no more notes, stop playing
        if(currNote>=_numNotes)
        {
          currState = NO_TRACK_PLAYING;
        }
        else //else continue onto next note
        {
          currDuration = _track[currNote][ON_FOR];
          currState = ON_FOR;
          prevTime = currTime;
          // start playing next note
          tone(_pin, _track[currNote][PITCH]); 
        }
      }
      // else noTone continues
      break;
    }
     case NO_TRACK_PLAYING:
    {
      break;
    }
    default:
    {
      break;
    }
  }
}


