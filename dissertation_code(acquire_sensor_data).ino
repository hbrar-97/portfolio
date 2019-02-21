#include <MySignals.h>
#include <MySignals_BLE.h>

char MAC_SPO2[14] = "00A050072626"; // MAC Address of SP02 sensor module


uint8_t available_spo2 = 0; //variable declared for available SP02
uint8_t connected_spo2 = 0;
uint8_t connection_handle_spo2 = 0;
uint8_t pulse_spo2 = 0;
uint8_t spo2 = 0;

#define SPO2_HANDLE 15

void setup()
{
	MySignals.begin(); //initiates the mysignals HW shield


	Serial.begin(115200); //BAUD rate of arduino set to 115200

	MySignals.initSensorUART();
	MySignals.enableSensorUART(BLE); //enables BLE module

	MySignals_BLE.hardwareReset();
	MySignals_BLE.initialize_BLE_values();


	if (MySignals_BLE.initModule() == 1)
	{

		if (MySignals_BLE.sayHello() == 1)
		{
			MySignals.println("BLE init ok");
		}
		else
		{
			MySignals.println("BLE init fail");

			while (1)
			{
			};
		}
	}
	else
	{
		MySignals.println("BLE init fail");

		while (1)
		{
		};
		available_spo2 = MySignals_BLE.scanDevice(MAC_SPO2, 1000, TX_POWER_MAX); //function implemented to scan for available sp02 sensor devices

		MySignals.disableMuxUART();
		Serial.print("SPO2 devices available:");
		Serial.println(available_spo2); //number of available devices correspoonding to the MAC Address provided is printed out into the serial monitor
		MySignals.enableMuxUART();

		if (available_spo2 == 1)
		{
			MySignals.disableMuxUART();
			Serial.println("SPO2 found.Connecting");
			MySignals.enableMuxUART();


			if (MySignals_BLE.connectDirect(MAC_SPO2) == 1) //MySignals shield BLE module attempts to connect to SP02 sensor with declared MAC value
			{
				connected_spo2 = 1;
				connection_handle_spo2 = MySignals_BLE.connection_handle;

				MySignals.println("Connected");

				delay(6000); //delay of 6 seconds added to ensure connection is established

				char attributeData[1] =
				{
					0x01
				};

				if (MySignals_BLE.attributeWrite(connection_handle_spo2, SPO2_HANDLE, attributeData, 1) == 0) //To connect to the SP02, the SP02_HANDLE must write "1"
				{
					unsigned long previous = millis(); //unsigned long allows for the storage of 32 bit integers and millis() fnc allows for time to be counted for approx 50 days.
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

								uint16_t errorCode =
								MySignals_BLE.disconnect(connection_handle_spo2);

								Serial.print(F("Disconnecting error code: "));
								Serial.println(errorCode, HEX);

								MySignals.enableMuxUART();
								connected_spo2 = 0;

							}
							
						}
						while ((connected_spo2 == 1) && ((millis() - previous) < 10000));

						connected_spo2 = 0;

					}
					else
					{
						MySignals.println("Error subscribing");
					}
				}
				else
				{
					connected_spo2 = 0;

					MySignals.println("Not Connected");
				}
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


			delay(1000);

			uint32_t normal_SP02 = 167025001; //normal SP02 level
			uint32_t abnormal_SP02 = 167025000; //abnormal SP02 level
			//uint64_t pulse_Snomedct = 297701000000109; //pulse finding

			delay(10000);

			if((spo2 >=94) && (spo2 <= 100))
			{
				Serial.println(normal_SP02);
			}
			else
			{
				Serial.println(abnormal_SP02);
			}

		}
	}
}
void loop()
{
	//only want setup to run and collect data once, therefore loop will be left empty.
}