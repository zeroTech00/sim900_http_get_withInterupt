#include <SoftwareSerial.h>
#include <string.h>

#define SIM900_TX_PIN 8 //SIM900 TX is connected to Arduino D8
#define SIM900_RX_PIN 7 //SIM900 RX is connected to Arduino D7

SoftwareSerial SIM900(SIM900_TX_PIN,SIM900_RX_PIN);

byte ms;
byte detik,menit;

//Timer2 Overflow Interrupt Vector, called every 32.64 ms
ISR(TIMER2_OVF_vect) {
  ms++;
  if(ms > 30) {ms=0; detik++;}
  if(detik > 59) {detik = 0; menit++;}
  if(menit > 59) {menit = 0;}

  // setiap 10 menit inisialisasi signal internet

  switch(detik) {
    case 1:
      SIM900.println("AT+HTTPINIT"); //Persiapan HTTP request
    break;

    case 4:
      SIM900.print("AT+HTTPPARA=\"URL\",\"http://104.248.159.38/store/");
      SIM900.print(random(0,100)); //dataSensor 1
      SIM900.print("/");
      SIM900.print(random(0,100)); //dataSensor 2
      SIM900.print("/");
      SIM900.print(random(0,100)); //dataSensor 3
      SIM900.print("/");
      SIM900.print(random(0,100)); //dataSensor 4
      SIM900.println(" \" ");
    break;

    case 9:
      SIM900.println("AT+HTTPACTION=0"); //submit request
    break;

    case 14:
      SIM900.println("");
    break;

    case 15:
      SIM900.println("AT+HTTPTERM");//baca data dari website yang diakses
    break;

    case 16:
      detik = 0;
    break;



    
  }
  
}


void settingSIM900() {
  SIM900.println("AT+CSQ"); // Mengecek Kualitas Sinyal
  delay(1000);
//  ShowSerialData();

  SIM900.println("AT+CGATT?"); //Melihat Support tidaknya GPRS
  delay(1000);
//  ShowSerialData();

  SIM900.println("AT+SAPBR=0,1");
  delay(1000);
//  ShowSerialData();

  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(1000);
//  ShowSerialData();

  SIM900.println("AT+SAPBR=3,1,\"APN\",\"m2mdev\"");
  delay(1000);
//  ShowSerialData();

  SIM900.println("AT+SAPBR=1,1");
  delay(3000);
//  ShowSerialData(); 
}


void setup() {
  SIM900.begin(9600);               
  Serial.begin(9600);
  delay(2); 

  // Timer/Counter 2 initialization
  // Clock source: System Clock
  // Clock value: 15.625 kHz
  // Mode: Phase correct PWM top=0xFF
  // OC2A output: Disconnected
  // OC2B output: Disconnected
  // Timer Period: 32.64 ms
  ASSR=(0<<EXCLK) | (0<<AS2);
  TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (1<<WGM20);
  TCCR2B=(0<<WGM22) | (1<<CS22) | (1<<CS21) | (1<<CS20);
  TCNT2=0x00;
  OCR2A=0x00;
  OCR2B=0x00;

  // Timer/Counter 2 Interrupt(s) initialization
  TIMSK2=(0<<OCIE2B) | (0<<OCIE2A) | (1<<TOIE2);

  settingSIM900();
}

void loop() {
  

}

void ShowSerialData()
{
  while(SIM900.available()!=0)
    Serial.write(SIM900.read());
    delay(0); 

}
