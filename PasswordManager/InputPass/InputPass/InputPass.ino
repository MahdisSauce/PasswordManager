#include <LiquidCrystal.h>
#include <EXT_EELib.h>

EXT_EEPROM eeprom(8, 9, 10, 11);                    //Creating an eeprom object

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;   //Pin Defitions for LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);          //Creating a lcd object

String namePass = "";
String pass = "";

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);                             //Starts the Serial communication with baudrate of 9600
    lcd.begin(16, 2);                               //Declaring that the lcd is a 16 by 2 lcd
    while (!Serial);                                //Waits to check if Serial is ready to be used
}

void loop() {
    // put your main code here, to run repeatedly:
    int dataUsed =  eeprom.readEEPROM(0);           //Getting the number of used bytes in the eeprom
    
    lcd.print("Enter Name: ");                      //Prompts the user to enter a username
    namePass = getData();                           //Gets data from Serial monitor
    lcd.clear();                                    //Clears the lcd

    lcd.setCursor(0, 1);                            //Sets the lcd cursor to the next line
    lcd.print("Enter Pass: ");
    pass = getData();
    lcd.clear();

    lcd.setCursor(0, 0);                            //Prints the username and password onto the lcd
    lcd.print(namePass);                            
    lcd.setCursor(0, 1);
    lcd.print(pass);

    int address = dataUsed+1;                       //Variable to index memory locations on the eeprom       
    int data = 0;                                   //counter variable to index data when sending it to the eeprom
    eeprom.enableWrite();
    for(data = 0; data < 16; data++){
        eeprom.writeEEPROM(address, (byte)namePass[data]);      //Sending the data to the eeprom
        address++;                                                  
        delay(2);
    }
    for(data = 0; data < 16; data++){
        eeprom.writeEEPROM(address, (byte)pass[data]);
        address++;
        delay(2);
    }
    dataUsed +=32;                                  //Updates the number of bytes used
    eeprom.writeEEPROM(0, dataUsed);
    eeprom.disableWrite();                          //Disables writing to avoid corruption
    
    getData();                                      //Using get data function as a buffer so user can see what they typed
    lcd.clear();
}

String getData(){                                   //Gets data from serial monitor and returns a string
    byte i = 0;                                     //i is used as counter so I dont return more then 16 characters
    String input = "";
    while(Serial.available()  == 0);                //Does nothing while there is nothing to recieve
    while(Serial.available() > 0 && i < 16){                  //Loops while there is data available
        input += (char)Serial.read();               //Store data in input
        i++;
        delay(10);
    }
    for(i; i < 16; i++)                            //If 16 character werent entered then fills the rest with spaces
        input += " ";
    return input;
}
