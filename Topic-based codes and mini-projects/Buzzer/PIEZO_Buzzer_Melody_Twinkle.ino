/*
 * Twinkle Twinkle Little Star on Piezo Buzzer
 *
 * This sketch plays the melody "Twinkle Twinkle Little Star" using a piezoelectric buzzer.
 * Different frequencies are used to generate musical notes, creating a simple song.
 *
 * Hardware Connections:
 *   Piezo Buzzer   | Aries Board
 *   --------------------------------
 *   VCC            | 3.3V
 *   GND            | GND
 *   IN (Signal)    | GPIO0
 *
 * Working Principle:
 * The buzzer is rapidly toggled between HIGH and LOW states at specific frequencies
 * to produce different musical notes. The frequency determines the pitch of the note,
 * while the duration controls how long each note plays.
 */

#define BUZZER 0   // Connect INPUT pin of buzzer to GPIO-0

// Define frequencies (in Hz) for musical notes in the fourth octave
#define NOTE_C4  262   // C note in 4th octave (middle C)
#define NOTE_D4  294   // D note in 4th octave
#define NOTE_E4  330   // E note in 4th octave
#define NOTE_F4  349   // F note in 4th octave
#define NOTE_G4  392   // G note in 4th octave
#define NOTE_A4  440   // A note in 4th octave
#define NOTE_B4  494   // B note in 4th octave
#define NOTE_C5  523   // C note in 5th octave

// Twinkle Twinkle Little Star melody sequence
int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,  // Twinkle twinkle little star
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4   // How I wonder what you are
};

// Note durations in milliseconds (how long each note should play)
int durations[] = {
  250, 250, 250, 250, 250, 250, 500,  // Duration for first line
  250, 250, 250, 250, 250, 250, 500   // Duration for second line
};

// Define the number of notes in our melody
const int noteCount = 14;

// The setup function runs once when the board powers on or is reset
void setup() {
  pinMode(BUZZER, OUTPUT);   // Configure buzzer pin as an output
}

// The loop function runs continuously after setup completes
void loop() {
  // Play the entire melody once by iterating through each note
  for (int i = 0; i < noteCount; i++) {
    playTone(melody[i], durations[i]);   // Play each note with its duration
    
    // Brief pause between notes to distinguish them
    delay(50);
  }
  
  // Longer pause before repeating the melody
  delay(1000);
}

/**
 * Function to play a tone of specific frequency and duration
 * 
 * @param frequency - The frequency of the tone in Hz
 * @param duration - How long to play the tone in milliseconds
 * 
 * This function generates a square wave at the specified frequency
 * by toggling the buzzer pin at precise intervals
 */
void playTone(int frequency, int duration) {
  // Calculate the period of one cycle in microseconds
  long period = 1000000 / frequency;
  long elapsedTime = 0;
  
  // Generate the square wave for the specified duration
  while (elapsedTime < duration * 1000) {
    digitalWrite(BUZZER, HIGH);       // Turn buzzer ON
    delayMicroseconds(period / 2);    // Wait for half the period
    digitalWrite(BUZZER, LOW);        // Turn buzzer OFF
    delayMicroseconds(period / 2);    // Wait for the other half of the period
    elapsedTime += period;            // Track the elapsed time
  }
}
