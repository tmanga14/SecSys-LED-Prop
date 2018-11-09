#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR 
#endif

#define PIN 6

//MODES
// Mode 0 = static color, knob will change brightness
// Mode 1 = fade in and out, no delay in between, knob will change length of delay
// Mode 2 = fade in and out, adjustable delay, knob will change length of middle delay
// Mode 3 = sporatic blinking


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, PIN,
	NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
	NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
	NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255),matrix.Color(0,0,0)
};


int clickButtonState;             // the current reading from the input pin
int lastClickButtonState = LOW;

int currentMode; //used for changing what mode the light is in

//Rotary Encoder Stuff
#define CLK 0
#define DT 1
#define SW 2
#define Button 10

//button for switching modes
#define modeButton 5

float blinkDelay1;
float blinkDelay2;
float scatterDelay;
int state;
int buttonState;
int Laststate;
int LastButtonState;
bool isPressing;

float counter;
float counterStop;
int brightness;
float colorVal;
float staticColorVal;
int switcher;

void setup() {
	currentMode = 1;
	blinkDelay1 = 1;
	blinkDelay2 = 1;
	scatterDelay = 10;

	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	pinMode(SW, INPUT);
	pinMode(modeButton, INPUT);
	isPressing = false;
	Laststate = digitalRead(CLK);
	LastButtonState = LOW;

	Serial.begin(9600);
	brightness = 16;
	colorVal = 0;

	staticColorVal = 110;
	counter = 0;
	counterStop = 255;
	switcher = 1;
	matrix.begin();
	matrix.setBrightness(brightness);
	matrix.drawPixel(1, 1, colors[1]);
	matrix.drawRect(0, 0, 4, 7, matrix.Color(colorVal, 0, 0));
}


void loop() {
	buttonState = digitalRead(modeButton);
	if (buttonState == HIGH && LastButtonState != buttonState) { //this will run when turning counterclockwise
		if (currentMode < 3) {
			currentMode++;
		}
		else {
			currentMode = 0;
		}
	}

	if (currentMode == 0) {

		state = digitalRead(CLK);
		if (state != Laststate)
		{
			if (digitalRead(DT) != state)
			{
				if (staticColorVal > 0) {
					staticColorVal--;
				}
			}
			else
			{
				if (staticColorVal < 255) {
					staticColorVal++;
				}
			}

		}
		matrix.fillScreen(0);
		matrix.fillRect(0, 0, 5, 8, matrix.Color(staticColorVal, 0, 0));

		matrix.show();
	}
	else if (currentMode == 1) {
		state = digitalRead(CLK);
		if (state != Laststate)
		{
			if (digitalRead(DT) != state)
			{
				if (blinkDelay1 > 0.2) {
					blinkDelay1 = blinkDelay1 - 0.005;
					Serial.println(blinkDelay1);
				}
			}
			else
			{
				blinkDelay1 = blinkDelay1 + 0.005;
				Serial.println(blinkDelay1);
			}

		}

		matrix.fillScreen(0);
		matrix.fillRect(0, 0, 5, 8, matrix.Color(colorVal, 0, 0));
		if (counter < counterStop && switcher != -1) {
			colorVal = colorVal + switcher * blinkDelay1;

			counter = counter + blinkDelay1;
			if (colorVal > counterStop) {
				colorVal = counterStop;
				counter = counterStop;
			}
		}
		else if (counter > 0 && switcher != 1) {
			colorVal = colorVal + switcher * blinkDelay1;
			counter = counter - blinkDelay1;
			if (colorVal < 0) {
				colorVal = 0;
				counter = 0;
			}
		}
		else
		{
			switcher = switcher * -1;
		}

		matrix.show();
	}
	//adjust delay between pulses
	else if (currentMode == 2) {
		state = digitalRead(CLK);
		if (state != Laststate)
		{
			if (digitalRead(DT) != state)
			{
				if (blinkDelay2 > 1) {
					blinkDelay2 = blinkDelay2 - 1;
					Serial.println(blinkDelay2);
				}
			}
			else
			{
				blinkDelay2 = blinkDelay2 + 1;
				Serial.println(blinkDelay2);
			}

		}

		matrix.fillScreen(0);
		matrix.fillRect(0, 0, 5, 8, matrix.Color(colorVal, 0, 0));
		if (counter < counterStop && switcher != -1) {
			colorVal = colorVal + switcher * blinkDelay1;

			counter = counter + blinkDelay1;
			if (colorVal > counterStop) {
				colorVal = counterStop;
				counter = counterStop;
			}
		}
		else if (counter > 0 && switcher != 1) {
			colorVal = colorVal + switcher * blinkDelay1;
			counter = counter - blinkDelay1;
			if (colorVal < 0) {
				colorVal = 0;
				counter = 0;
			}
		}
		else
		{
			switcher = switcher * -1;
		}

		

		matrix.show();
		if (counter == 0) {
			delay(blinkDelay2);
		}
	}
	else if (currentMode == 3) {
		state = digitalRead(CLK);
		if (state != Laststate)
		{
			if (digitalRead(DT) != state)
			{
				if (scatterDelay > 10) {
					scatterDelay = scatterDelay - 10;
					Serial.println(scatterDelay);
				}
			}
			else
			{
				scatterDelay = scatterDelay + 10;
				Serial.println(scatterDelay);
			}

		}
		matrix.fillScreen(0);
		//this randomization is enough to create an interesting blinking pattern
		matrix.drawPixel(random(5), random(8), colors[0]);
		matrix.drawPixel(random(5), random(8), colors[0]);
		matrix.drawPixel(random(5), random(8), colors[0]);
		matrix.drawPixel(random(5), random(8), colors[0]);
		matrix.drawPixel(random(5), random(8), colors[0]);
		matrix.drawPixel(random(5), random(8), colors[0]);
		matrix.drawPixel(random(5), random(8), colors[0]);
		matrix.drawPixel(random(5), random(8), colors[0]);
		matrix.show();
		delay(scatterDelay); //the dial-set delay is used here, where it just uses a normal delay
		
	}

	LastButtonState = buttonState; //this sets the current button state to last button state for use on the next loop
}
