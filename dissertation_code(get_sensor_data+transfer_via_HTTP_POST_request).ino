#include <MySignals.h>
#include <MySignals_BLE.h>
#include <Ethernet.h>
#include <SPI.h>


// Write here the MAC address of BLE device to find
char MAC_SPO2[14] = "00A050072626";

	byte mac[] = { 0x90,0xA2,0xDA,0x0D,0x0D,0xB1}; //Replace with Ethernet shield MAC
	EthernetClient client;
	IPAddress ip(192, 168, 0, 113); //ethernet ip address attatched my ethernet shield

	// EDIT: 'Server' address to match your domain
	char server[] = "192.168.0.60"; // This could also be 168.1.18/~me if you are running a server on your computer on local network.



	uint8_t available_spo2 = 0;
	uint8_t connected_spo2 = 0;
	uint8_t connection_handle_spo2 = 0;
	uint8_t pulse_spo2 = 0;
	uint8_t spo2 = 0;

	String yourdatacolumn = "sp02=";
	String sp02level;

	#define SPO2_HANDLE 15

	void setup()
	{
		MySignals.begin();

		Serial.begin(115200);
		Ethernet.begin(mac,ip);
		delay(1000);
		Serial.println("connnecting....");
		postData();

		MySignals.initSensorUART();
		MySignals.enableSensorUART(BLE);

		MySignals_BLE.hardwareReset();
		MySignals_BLE.initialize_BLE_values();


		if (MySignals_BLE.initModule() == 1)
		{

			if (MySignals_BLE.sayHello() == 1)
			{
				MySignals.println("BLE init ok");
				Serial.println("BLE init ok");
			}
			else
			{
				MySignals.println("BLE init fail");
				Serial.println("BLE init fail");

				while (1)
				{
				};
			}
		}
		else
		{

			MySignals.println("BLE init fail");
			Serial.println("BLE init fail");

			while (1)
			{
			};
		}
		delay(5000);
		available_spo2 = MySignals_BLE.scanDevice(MAC_SPO2, 1000, TX_POWER_MAX);
		delay(5000);
		MySignals.disableMuxUART();
		Serial.print("SPO2 available:");
		Serial.println(available_spo2);
		MySignals.enableMuxUART();

		delay(5000);

		if (available_spo2 == 1)
		{
			MySignals.disableMuxUART();
			Serial.println("SPO2 found.Connecting");
			MySignals.enableMuxUART();


			if (MySignals_BLE.connectDirect(MAC_SPO2) == 1)
			{
				connected_spo2 = 1;
				connection_handle_spo2 = MySignals_BLE.connection_handle;

				MySignals.println("Connected");

				Serial.println("connected to sp02 sensor");

				delay(6000);

				//To subscribe the spo2 measure write "1" in SPO2_HANDLE
				char attributeData[1] =
				{
					0x01
				};

				if (MySignals_BLE.attributeWrite(connection_handle_spo2, HANDLE, attributeData, 1) == 0)
				{
					unsigned long previous = millis();
					do
					{

						if (MySignals_BLE.waitEvent(1000) == BLE_EVENT_ATTCLIENT_ATTRIBUTE_VALUE)
						{
							char attributeData[1] = { 0x00 };


							MySignals_BLE.attributeWrite(connection_handle_spo2, SPO2_HANDLE, attributeData , 1);

							uint8_t pulse_low = MySignals_BLE.event[12];
							pulse_low &= 0b01111111;

							uint8_t pulse_high = MySignals_BLE.event[11];
							pulse_high &= 0b01000000;

							if (pulse_high == 0)
							{
								pulse_spo2 = pulse_low;
							}

							if (pulse_high == 0b01000000)
							{
								pulse_spo2 = pulse_low + 0b10000000;
							}

							spo2 = MySignals_BLE.event[13];
							spo2 &= 0b01111111;

							if ((pulse_spo2 >= 25) && (pulse_spo2 <= 250)
							&& (pulse_spo2 >= 35) && (pulse_spo2 <= 100))
							{
								MySignals.disableMuxUART();

								Serial.println();
								Serial.print(F("SpO2: "));
								Serial.print(spo2);
								Serial.print(F("% "));
								Serial.print(F("Pulse: "));
								Serial.print(pulse_spo2);
								Serial.println(F("ppm "));

								uint16_t errorCode = MySignals_BLE.disconnect(connection_handle_spo2);

								Serial.print(F("Disconnecting error code: "));
								Serial.println(errorCode, HEX);


								MySignals.enableMuxUART();
								connected_spo2 = 0;

							}
						}
					}
				}
			}
			while ((connected_spo2 == 1) && ((millis() - previous) < 1000));

			connected_spo2 = 0;

		}
		else
		{
			MySignals.println("Error subscribing");
		}
		else
		{
			connected_spo2 = 0;

			MySignals.println("Not Connected");
		}
		else if (available_spo2 == 0)
		{
			//Do nothing
		}
		else
		{

			MySignals_BLE.hardwareReset();
			MySignals_BLE.initialize_BLE_values();
			delay(100);

		}

		uint32_t normal_SP02 = 167025001; //normal SP02 level
		uint32_t abnormal_SP02 = 167025000; //abnormal SP02 level
		//uint64_t pulse_Snomedct = 297701000000109; //pulse finding

		delay(10000);


		if((spo2 >=94) && (spo2 <= 100))
		{
			Serial.println(normal_SP02);

			sp02level = yourdatacolumn + normal_SP02 + ": " + spo2;
		}
		else
		{
			Serial.println(abnormal_SP02);
			sp02level = yourdatacolumn + abnormal_SP02 + ": " + spo2;
		}

		if (client.connect(server, 8080)) 
		{
			Serial.println("connecting to MySQL database...");

			// EDIT: The POST 'URL' to the location of your insert_mysql.php on your web-host
			client.println("POST /update/update.php HTTP/1.1");

			// EDIT: 'Host' to match your domain
			client.println("Host:192.168.0.60:8080");
			client.println("User-Agent: Arduino/1.0");
			client.println("Connection: close");
			client.println("Content-Type: application/x-www-formurlencoded;");
			client.print("Content-Length: ");
			client.println(sp02level.length());
			client.println();
			client.println(sp02level);

		}
		else 
		{
			// If you couldn't make a connection:
			Serial.println("Connection failed");
			Serial.println("Disconnecting.");
			client.stop();
		}

		delay(1000);
	}

	void loop()
	{
		//only want setup to run and collect data once, therefore loop will be left empty.
	}

	void postData() 
	{
		//only want setup to run and collect data once, therefore postData will be left empty.
	}