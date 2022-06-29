#include <LiquidCrystal.h>
#include <EXT_EELib.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

EXT_EEPROM eeprom(8, 9, 10, 11);
String namePass = "";
String pass = "";

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    lcd.begin(16, 2);
    while (!Serial);
}

void loop() {
    // put your main code here, to run repeatedly:
    int dataUsed =  eeprom.readEEPROM(0);
    
    lcd.print("Enter Name: ");
    namePass = getData();
    lcd.clear();  

    lcd.setCursor(0, 1);
    lcd.print("Enter Pass: ");
    pass = getData();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(namePass);
    lcd.setCursor(0, 1);
    lcd.print(pass);

    int address = dataUsed+1;
    int data = 0;
    eeprom.enableWrite();
    for(data = 0; data < 16; data++){
        eeprom.writeEEPROM(address, (byte)namePass[data]);
        address++;
        delay(2);
    }
    for(data = 0; data < 16; data++){
        eeprom.writeEEPROM(address, (byte)pass[data]);
        address++;
        delay(2);
    }
    dataUsed +=32;
    eeprom.writeEEPROM(0, dataUsed);
    eeprom.disableWrite();
    
    getData();
    lcd.clear();
}

String getData(){
    byte i = 0;
    String input = "";
    while(Serial.available()  == 0);
    while(Serial.available() > 0){
        input += (char)Serial.read();
        i++;
        delay(10);
    }
    for(i; i < 16; i++)
        input += " ";
    if(i > 16)
        pass = "ERR";
    return input;
}
