#include <LiquidCrystal.h>
#include <EXT_EELib.h>                              //Including libraries

#define FORWARD 13                                  //Left Button   
#define BACKWARD 12                                 //Right Button
#define DEBOUNCE 15                                 //Decounce timing

EXT_EEPROM eeprom(8, 9, 10, 11);                    //Creating an eeprom object

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;   //Pin Defitions for LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);          //Creating a lcd object

void setup() {
    // put your setup code here, to run once:
    pinMode(BACKWARD, INPUT_PULLUP);                //Setting pins to input with built-in resistor
    pinMode(FORWARD, INPUT_PULLUP);
    lcd.begin(16, 2);                   //Declaring that the lcd is a 16 by 2 lcd
    lcd.print("PRESS A BUTTON");        //Prompting the user
}

void loop() {
    // put your main code here, to run repeatedly:
    byte usedBytes = eeprom.readEEPROM(0);          //Getting the number of used bytes in the eeprom
    byte selectedData = 0;                          //Setting the selected byte
    while(usedBytes){                               //If there is data on the eeprom then loop
        if(!digitalRead(FORWARD)){                  //Check if the forward button is pressed
            delay(DEBOUNCE);                        //Delay an amount to prevent bounce 
            if(selectedData < (usedBytes/32)-1)     //Check if selected data is less than the amount of written data
                selectedData++;
            else
                selectedData = 0;                   //If it gets to the last data slot it will wrap back to the first piece of data         
            displayData(getData((selectedData*32)+1), getData((selectedData*32)+17));   //Displays the data
            while(!digitalRead(FORWARD));           //Check if the button is still being pressed
            delay(DEBOUNCE);
        }
    
        if(!digitalRead(BACKWARD)){                 //Does same as previous if statement but for indexing the other way
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
    lcd.print("There aint jack");                   //If eeprom is empty prints on lcd
    lcd.setCursor(0, 1);
    lcd.print("stuff");
    while(1);
}

String getData(byte selectedData){                  //Reads data from eeprom starting from given memory address and adds it to string(This loops 16 times)
    String data = "";
    for(int i = selectedData; i < selectedData+16; i++) 
        data += (char)eeprom.readEEPROM(i);

    return data;
}

void displayData(String namePass, String pass){     //This prints the data to the lcd with some slight formatting
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(namePass);
    delay(2);
    lcd.setCursor(0, 1);
    lcd.print(pass);
    delay(2);
}
