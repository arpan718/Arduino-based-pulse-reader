#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <PulseSensorPlayground.h>

// Pulse Sensor
const int PULSE_PIN = A0;
PulseSensorPlayground pulseSensor;

// TFT Pins
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

// TFT Object
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Variables
int BPM = 0;
int lastBPM = 0;
int waveX = 0;

void setup() {
  Serial.begin(115200);

  // Initialize TFT
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);

  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("BPM MONITOR");
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);

  // Pulse Sensor Setup
  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.setThreshold(550); // Adjust if needed

  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor Ready!");
  }
}

void loop() {
  
  // Read BPM
  int myBPM = pulseSensor.getBeatsPerMinute();

  if (pulseSensor.sawStartOfBeat()) {
    BPM = myBPM;
    Serial.print("♥ BPM: ");
    Serial.println(BPM);

    // Display BPM on TFT
    tft.fillRect(0, 0, 160, 30, ST77XX_BLACK);
    tft.setTextColor(ST77XX_YELLOW);
    tft.setTextSize(3);
    tft.setCursor(10, 5);
    tft.print(BPM);
  }

  // Draw waveform
  drawWaveform( pulseSensor.getLatestSample() );

  delay(20);
}

// Function to draw ECG-like waveform
void drawWaveform(int value) {
  int y = map(value, 0, 1023, 100, 60);

  // Erase old point
  tft.drawPixel(waveX, y, ST77XX_GREEN);

  waveX++;
  if (waveX >= 160) {  
    waveX = 0;
    tft.fillRect(0, 40, 160, 88, ST77XX_BLACK);
  }
}
