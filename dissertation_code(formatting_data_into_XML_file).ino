#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

uint32_t snomed_ct_sp02 = 100000;
uint32_t sp02 = 100;
uint32_t snomed_ct_pulse = 20000;
uint32_t pulse = 100;

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	while (!Serial) 
	{
	; // wait for serial port to connect. Needed for native USB port only
	}

	Serial.print("Initializing SD card...");

	// see if the card is present and can be initialized:
	if (!SD.begin(chipSelect)) 
	{
		Serial.println("Card failed, or not present");
		// don't do anything more:
		return;
	}
	Serial.println("card initialized.");
	// Create/Open file
	SD.remove("sensor.xml");

	File myFile = SD.open("sensor.xml", FILE_WRITE);

	// if the file is available, write to it:
	if (myFile) {


		myFile.println("<patient>");
		myFile.println("<sensor>");

		myFile.println();

		myFile.print("<sp02>");
		myFile.print(snomed_ct_sp02);
		myFile.print(":");
		myFile.print(sp02);

		myFile.print("</sp02>");

		myFile.print("</sensor>");

		myFile.println();

		myFile.print("</patient>");

		myFile.close();
		// print to the serial port too:
		Serial.println(snomed_ct_sp02);
		Serial.println(sp02);
		Serial.println(snomed_ct_pulse);
		Serial.println(pulse);
	}
	// if the file isn't open, pop up an error:
	else 
	{
		Serial.println("error opening sensor.xml");
	}
}

void loop() {

}
