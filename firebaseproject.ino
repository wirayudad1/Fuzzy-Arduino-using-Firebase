//sesuikan posisi pin select
#include <CD74HC4067.h>
#include <ESP8266WiFi.h>                                                // esp8266 library
#include <FirebaseArduino.h>                                             // firebase library  
#define FIREBASE_HOST "webserver-722a2.firebaseio.com"             // the project name address from firebase id
#define FIREBASE_AUTH "FG7FFgRbyK18BzMoa5zQN5yxcapLI4gI6eoL0jmU"       // the secret key generated from firebase

#define WIFI_SSID "seven_play"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "berbahaya03"
int data[3];
String text,text2;

CD74HC4067 mux(16,5,4,0);
//gunakan A0 sebagai input
int analogPin = A0;
  
//variabel untuk menyimpan nilai input
int nilaiInput = 0;
double minnilai[4][4];
double temper[3], kelembapan[3];
double fanon_1[3], fanof[3], fanon_2[4];
double maxfanon_1 = 0, maxfanof = 0, maxfanon_2 = 0;
String kondisi;
double dataout[121];
void setup() {
  //jadikan pin select sebagai output
    //aktifkan komunikasi serial
  Serial.begin(9600);
  pinMode(A0, INPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST);   
  

}
void datarulecl()
{
  maxfanon_1 = 0; maxfanof = 0; maxfanon_2 = 0;
}
double Member1(double x, int kondisi)//Series 1 untuk tiap grafik teechart
{
  double fuz = 0;
  if (kondisi == 1)//suhu(dingin)
  {
    if (x <= 25)
    {
      fuz = 1;
    }
    else if (x > 25 && x <= 30)
    {
      fuz = 1 - ((x - 25) / 5);
    }
    else fuz = 0;

  }
  else if (kondisi == 2)//kelembaban(kering)
  {
    if (x <= 30)
    {
      fuz = 1;
    }
    else if (x > 30 && x <= 45)
    {
      fuz = 1 - ((x - 30) / 15);
    }
    else
      fuz = 0;
  }
  else if (kondisi == 3)//kondisi kipas mati
  {
    if (x <= 30)
    {
      fuz = 1;
    }
    else if (x > 30 && x <= 50)
    {
      fuz = 1 - ((x - 30) / 20);
    }
    else
    {
      fuz = 0;
    }
  }
  return fuz;
}
double Member2(double x, int kondisi)//series2 untuk tiap grafik teechart
{
  double fuz = 0;
  if (kondisi == 1){//suhu hangat
    if (x <= 25){
      fuz = 0;
    }
    else if (x > 25 && x <= 30){
      fuz = 1 - ((30 - x) / 5);
    }
    else if (x > 30 && x <= 35){
      fuz = 1 - ((x - 30) / 5);
    }
    else fuz = 0;
  }
  else if (kondisi == 2)//kelembaban(hangat)
  {
    if (x <= 30){
      fuz = 0;
    }
    else if (x > 30 && x <= 45){
      fuz = 1 - ((45 - x) / 15);
    }
    else if (x > 45 && x <= 70){
      fuz = 1 - ((x - 45) / 25);
    }
    else fuz = 0;
  }
  else if (kondisi == 3)//1 kipas nyala
  {
    if (x <= 45){
      fuz = 0;
    }
    else if (x > 45 && x <= 65){
      fuz = 1 - ((65 - x) / 20);
    }
    else if (x > 65 && x <= 80){
      fuz = 1 - ((x - 65) / 15);
    }
    else fuz = 0;
  }
  return fuz;
}
double Member3(double x, int kondisi)//series3 untuk tiap grafik teechart
{
  double fuz = 0;
  if (kondisi == 1){//suhu(panas)
    if (x <= 30){
      fuz = 0;
    }
    else if (x > 30 && x <= 35){
      fuz = 1 - ((35 - x) / 5);
    }
    else fuz = 1;
  }
  else if (kondisi == 2)//kelembaban(basah)
  {
    if (x <= 45){
      fuz = 0;
    }
    else if (x > 45 && x <= 70){
      fuz = 1 - ((70 - x) / 25);
    }
    else fuz = 1;
  }
  else if (kondisi == 3)//2 kipas nyala
  {
    if (x <= 70){
      fuz = 0;
    }
    else if (x > 70 && x <= 90){
      fuz = 1 - ((90 - x) / 20);
    }
    else fuz = 1;
  }
  return fuz;
}
void fuzzifikasi(double  suhu,double kelembaban)
{
  temper[0] = Member1(suhu, 1);
  temper[1] = Member2(suhu, 1);
  temper[2] = Member3(suhu, 1);
  kelembapan[0] = Member1(kelembaban, 2);
  kelembapan[1] = Member2(kelembaban, 2);
  kelembapan[2] = Member3(kelembaban, 2);
}
void ruletable()
{
  datarulecl();
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      minnilai[i][j] = min(kelembapan[i], temper[j]);
    }
  }
  fanof[0] = minnilai[0][0]; fanon_1[0] = minnilai[0][1]; fanon_2[0] = minnilai[0][2];
  fanof[1] = minnilai[1][0]; fanon_1[1] = minnilai[1][1]; fanon_2[1] = minnilai[1][2];
  fanof[2] = minnilai[2][0]; fanon_2[3] = minnilai[2][1]; fanon_2[2] = minnilai[2][2];
  for (int a = 0; a < 3; a++)
  {
    maxfanof = max(fanof[a], maxfanof);
    maxfanon_1 = max(fanon_1[a], maxfanon_1);
  }
  for (int a = 0; a < 4; a++)
  {
    maxfanon_2 = max(fanon_2[a], maxfanon_2);
  }
}
void defuzzifikasi()
{
 if(maxfanof >  maxfanon_1 && maxfanof > maxfanon_2)kondisi = "Kipas Mati";
 else if(maxfanon_1 >  maxfanon_2 && maxfanon_1 > maxfanof)kondisi = "1 kipas  menyala";
 else if(maxfanon_2 >  maxfanon_1 && maxfanon_2 > maxfanof)kondisi = "2 kipas  menyala";
}  
void sendfirebase(double t,double h){
  
  String suhu=String(t) + String("°C");
   String kelembaban=String(h) + String("%");
  Firebase.setString("dht/temperature", suhu);
  Firebase.setString("dht/kelembaban", kelembaban);  
  Firebase.setString("dht/kondisi", kondisi);                                    
  delay(50);
  
}
void loop() {
    mux.channel(0);
    data[0] = analogRead(A0);
    mux.channel(9);
    data[2] = analogRead(A0);
    double conv_suhu=map(data[0],0,1024,0,100);
    double conv_lembab=map(data[2],0,1024,0,120);
   Serial.print("Suhu: ");Serial.print(conv_suhu); Serial.print("  Kelembaban: ");Serial.println(conv_lembab);
   fuzzifikasi(conv_suhu,conv_lembab);
   ruletable();
   defuzzifikasi();
   sendfirebase(conv_suhu,conv_lembab);
   Serial.print("kondisi: ");Serial.println(kondisi);
   delay(200);
}
