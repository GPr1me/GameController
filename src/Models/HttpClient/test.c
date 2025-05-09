// #include <HttpClient.h>
// #include <Ethernet.h>
// #include <EthernetClient.h>

void setup() {
	// Initialize the SPI for the Ethernet module
	// byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

	// while (Ethernet.begin(mac) != 1) {
	// 	Serial.println("Failed to configure Ethernet using DHCP");
	// 	delay(15000);
	// }
}

// void test_http() {
// 	const char *url = "https://192.168.40.52";
// 	const char *path = "/Game/Test?message=Hello_from_ESP32&number=1234";
	
// 	const int timeout = 30*1000;
// 	const int delay = 1000;

// 	int err =0;
  
// 	EthernetClient c;
// 	HttpClient http(c);
	
// 	err = http.get(kHostname, kPath);
// 	if (err == 0)
// 	{
// 	  Serial.println("startedRequest ok");
  
// 	  err = http.responseStatusCode();
// 	  if (err >= 0)
// 	  {
// 		Serial.print("Got status code: ");
// 		Serial.println(err);
  
// 		// Usually you'd check that the response code is 200 or a
// 		// similar "success" code (200-299) before carrying on,
// 		// but we'll print out whatever response we get
  
// 		err = http.skipResponseHeaders();
// 		if (err >= 0)
// 		{
// 		  int bodyLen = http.contentLength();
// 		  Serial.print("Content length is: ");
// 		  Serial.println(bodyLen);
// 		  Serial.println();
// 		  Serial.println("Body returned follows:");
		
// 		  // Now we've got to the body, so we can print it out
// 		  unsigned long timeoutStart = millis();
// 		  char c;
// 		  // Whilst we haven't timed out & haven't reached the end of the body
// 		  while ( (http.connected() || http.available()) &&
// 				 ((millis() - timeoutStart) < kNetworkTimeout) )
// 		  {
// 			  if (http.available())
// 			  {
// 				  c = http.read();
// 				  // Print out this character
// 				  Serial.print(c);
				 
// 				  bodyLen--;
// 				  // We read something, reset the timeout counter
// 				  timeoutStart = millis();
// 			  }
// 			  else
// 			  {
// 				  // We haven't got any data, so let's pause to allow some to
// 				  // arrive
// 				  delay(kNetworkDelay);
// 			  }
// 		  }
// 		}
// 		else
// 		{
// 		  Serial.print("Failed to skip response headers: ");
// 		  Serial.println(err);
// 		}
// 	  }
// 	  else
// 	  {    
// 		Serial.print("Getting response failed: ");
// 		Serial.println(err);
// 	  }
// 	}
// 	else
// 	{
// 	  Serial.print("Connect failed: ");
// 	  Serial.println(err);
// 	}
// 	http.stop();
// }