#include <LiquidCrystal.h>
#include <EXT_EELib.h>

#define FORWARD 13
#define BACKWARD 12
#define DEBOUNCE 25

EXT_EEPROM eeprom(8, 9, 10, 11);

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    // put your setup code here, to run once:
    pinMode(BACKWARD, INPUT_PULLUP);
    pinMode(FORWARD, INPUT_PULLUP);
    lcd.begin(16, 2);
    lcd.print("PRESS A BUTTON");
}

void loop() {
    // put your main code here, to run repeatedly:
    byte usedBytes = eeprom.readEEPROM(0);
    byte selectedData = 0;
    while(usedBytes){
        if(!digitalRead(FORWARD)){
            delay(DEBOUNCE);
            if(selectedData < (usedBytes/32)-1)
                selectedData++;
            else
                selectedData = 0;
            displayData(getData((selectedData*32)+1), getData((selectedData*32)+17));
            while(!digitalRead(FORWARD));
            delay(DEBOUNCE);
        }
    
        if(!digitalRead(BACKWARD)){
            delay(DEBOUNCE);
            if(selectedData > 0)
                selectedData--;
            else
                selectedData = (usedBytes/32)-1;
            displayData(getData((selectedData*32)+1), getData((selectedData*32)+17));
            while(!digitalRead(BACKWARD));
            delay(DEBOUNCE);
        }
  
    };
    lcd.print("There aint jack");
    lcd.setCursor(0, 1);
    lcd.print("shit");
    while(1);
}

String getData(byte selectedData){
    String data = "";
    for(int i = selectedData; i < selectedData+16; i++)
        data += (char)eeprom.readEEPROM(i);

    return data;
}

void displayData(String namePass, String pass){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(namePass);
    delay(2);
    lcd.setCursor(0, 1);
    lcd.print(pass);
    delay(2);
}
