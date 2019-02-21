#include <SPI.h>
#include <String.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 };

byte ip[] = { 192, 168, 0, 113};

byte servidor[] = { 192, 168, 0, 60 };

EthernetServer server(8080);
EthernetClient cliente;

float sp02 = 98;
u_int32 sctid = 167025001;

String readString = String(30);

unsigned long previousMillis = 0;
const long interval = 5000;



void setup() 
{
	Ethernet.begin(mac, ip);
	Serial.begin(115200);
}

void loop() 
{

	EthernetClient client = server.available();
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval) 
	{

		previousMillis = currentMillis;

		if (cliente.connect(servidor, 8080)) 
		{
			Serial.println("connected");

			sp02 = sp02 + 20;
			sctid = sctid + 20;

			cliente.print("GET /arduino/saved.php?");
			cliente.print("sp02=");
			cliente.print(sp02);
			cliente.print("&snomedct=");
			cliente.print(sctid);


			Serial.print("sp02 = ");
			Serial.println(sp02);
			Serial.print("SNOMED_CT_CODE = ");
			Serial.println(sctid);

			cliente.println("HTTP/1.1");
			cliente.println("Host: http://192.168.0.60:8080");
			cliente.println("connection: close");
			cliente.println();
			cliente.println();
			cliente.stop();

		}
		else 
		{
			Serial.println("connection failed");

			cliente.stop();
		}
	}
}