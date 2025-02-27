#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
const char* ssid = "ECC";
const char* password = "12345678";

// Insert Firebase project API Key
#define API_KEY "AIzaSyA99qeaRr4YBzqXmlLshAfOcQdHQSQ10EU"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://feeder-73a76-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;
String parentPath = "/Setting";
String childPath[1] = {"/command"};
bool signupOK = false;
volatile bool dataChanged = false;
String str[1];

void streamCallback(MultiPathStream stream)
{
  size_t numChild = sizeof(childPath) / sizeof(childPath[0]);
  for (size_t i = 0; i < numChild; i++)
  {
    if (stream.get(childPath[i]))
    {
      str[i] = stream.value.c_str();
      Serial.printf("path: %s, event: %s, type: %s, value: %s%s", stream.dataPath.c_str(), stream.eventType.c_str(), stream.type.c_str(), stream.value.c_str(), i < numChild - 1 ? "\n" : "");
    }
  }

  Serial.println();
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());
  dataChanged = true;
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

const int trig_main = 18;
const int echo_main = 19;

#define A 16
#define B 17

uint32_t pmillis = 0;
bool obstacleFlag = false;

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(21, OUTPUT);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(21, HIGH);

  Serial.begin(115200);
  Serial.println("OK");
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  pinMode(echo_main, INPUT);
  pinMode(trig_main, OUTPUT);

  initWifi();
  InitFirebase();
  digitalWrite(2, HIGH);
  dataChanged = false;
}

void loop() {
  if (dataChanged) {
    dataChanged = false;
    if (str[0] == "FWD") {
      digitalWrite(21, LOW);
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      //      FWD();
    }
    else if (str[0] == "STOP") {
      digitalWrite(21, HIGH);
      //      STOP();
    }
    else if (str[0] == "BACK") {
      digitalWrite(21, LOW);
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      //      BACK();
    }
    else if (str[0] == "RIGHT") {
      digitalWrite(21, LOW);
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      //      RIGHT();
    }
    else if (str[0] == "LEFT") {
      digitalWrite(21, LOW);
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      //      LEFT();
    }
  }
  
  //  Obstacle Detection Section
  if (millis() - pmillis >= 2500) {
    pmillis = millis();
    ObstacleCheck(60.0);
  }

}

// ********************************************************************************************************
void ObstacleCheck(float limit) {
  //    Serial.print("Distance : ");
  //    Serial.println(read_ultra(trig_main, echo_main), 2);
  if (read_ultra(trig_main, echo_main) < limit) {
    Serial.println("Object Detected !");
    digitalWrite(2, HIGH);
    obstacleFlag = true;
    digitalWrite(21, HIGH);
  } else {
//    digitalWrite(21, LOW);
    digitalWrite(2, LOW);
    obstacleFlag = false;
  }
}

double read_ultra(const int trigPin , const int echoPin) {
  double distance = 0;
  unsigned long duration = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = (duration * 0.034) / 2; // Speed of sound wave divided by 2 (go and back)
  return distance;
}

void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void InitFirebase() {
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  if (!Firebase.RTDB.beginMultiPathStream(&stream, parentPath))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setMultiPathStreamCallback(&stream, streamCallback, streamTimeoutCallback);
  Firebase.reconnectWiFi(true);
  Serial.println("Wifi Connected !!");
  delay(1000);
}
