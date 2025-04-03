#include "sound_module.h"
#include "globals.h"

// Define melody (12 notes) and note durations.
int melody[] = {NOTE_E5, NOTE_E5, 0, NOTE_E5, 0, NOTE_C5, NOTE_E5, 0, NOTE_G5, 0, 0, NOTE_G4};
int noteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4};

void playStartupMelody() {
  for (int thisNote = 0; thisNote < 12; thisNote++) {
    int noteDuration = 60 * 1000 / BPM / noteDurations[thisNote];
    tone(speakerPin, (melody[thisNote] != 0) ? (500000 / melody[thisNote]) : 0, noteDuration - RELEASE);
    delay(noteDuration);
  }
  noTone(speakerPin);
  digitalWrite(speakerPin, LOW);
}
