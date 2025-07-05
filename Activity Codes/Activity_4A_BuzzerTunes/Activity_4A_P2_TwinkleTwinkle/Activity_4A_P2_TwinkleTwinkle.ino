#define BUZZER 1 // Connect INPUT pin of buzzer to GPIO-1

// Define frequencies for musical notes
#define NOTE_C4  262
#define NOTE_Cs4 277
#define NOTE_D4  294
#define NOTE_Ds4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_Fs4 370
#define NOTE_Gs4 415  
#define NOTE_A4  440
#define NOTE_As4 466
#define NOTE_B4  494
#define NOTE_C5  523

// Melody sequence
int melody[] = {
  NOTE_A4, NOTE_B4, NOTE_A4, NOTE_Gs4, NOTE_A4,
  NOTE_A4, NOTE_B4, NOTE_A4, NOTE_Gs4, NOTE_A4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_A4, NOTE_B4, NOTE_A4, NOTE_Gs4, NOTE_Fs4,

  NOTE_A4, NOTE_B4, NOTE_A4, NOTE_Gs4, NOTE_A4,
  NOTE_A4, NOTE_B4, NOTE_A4, NOTE_Gs4, NOTE_A4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_A4, NOTE_B4, NOTE_A4, NOTE_Gs4, NOTE_Fs4,

  NOTE_E4, NOTE_Fs4, NOTE_Fs4, NOTE_Gs4, NOTE_Gs4,
  NOTE_Gs4, NOTE_Fs4, NOTE_Fs4, NOTE_Gs4, NOTE_Gs4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_B4, NOTE_C5,

  NOTE_E4, NOTE_Fs4, NOTE_Fs4, NOTE_Gs4, NOTE_Gs4,
  NOTE_Gs4, NOTE_Fs4, NOTE_Fs4, NOTE_Gs4, NOTE_Gs4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_B4, NOTE_C5
};

// Adjusted durations for a faster tempo
int durations[] = {
  100, 100, 100, 100, 200,
  100, 100, 100, 100, 200,
  100, 100, 100, 100, 200,
  100, 100, 100, 100, 200,

  100, 100, 100, 100, 200,
  100, 100, 100, 100, 200,
  100, 100, 100, 100, 200,
  100, 100, 100, 100, 200,

  100, 200, 100, 200, 300,
  100, 100, 100, 200, 100,
  200, 100, 200, 100, 200,

  100, 200, 100, 200, 300,
  100, 100, 100, 200, 100,
  200, 100, 200, 100, 200,
};

// Number of notes in the melody
const int noteCount = sizeof(melody) / sizeof(melody[0]);

void setup() {
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // Play the melody
  for (int i = 0; i < noteCount; i++) {
    playTone(melody[i], durations[i]);
    
    // Shorter pause between notes
    delay(30);
  }
  
  // Shorter pause before repeating
  delay(500);
}

// Function to play a tone with specific frequency and duration
void playTone(int frequency, int duration) {
  long period = 1000000 / frequency;
  long elapsedTime = 0;

  while (elapsedTime < duration * 1000) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(period / 2);
    elapsedTime += period;
  }
}
