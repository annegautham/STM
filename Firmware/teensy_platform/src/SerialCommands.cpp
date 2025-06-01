#include <Arduino.h>
#include "globals.hpp"
#include "SerialCommands.hpp"


void checkSerial()
{
  
  String serialString;
  
  if(Serial.available() > 0)
  {
    for(int i = 0; i < 2; i++)  // Accepts 2 char commands
    {
      delay(1); // This seems to be necessary, not sure why
      char inChar = Serial.read();
      serialString += inChar;
    }
  }
  serialCommand(serialString);
  serialString = "";
}

/**************************************************************************/

void serialCommand(String str)
{
  
  String command = "00";
  
  if (str.length() > 0)
  {     
      // Get the first 2 characters of str and store them in command:
      for(int i = 0; i < 2; i++)
      {
        command[i] = str[i];
      }
      
      
      if(command == "SE") // Enable serial communications
      {
        Serial.begin(115200);
        Serial.println("SE");
        serialEnabled = true;
        digitalWriteFast(SERIAL_LED, HIGH);
      }
      
      
      else if(command == "SD") // Disable serial communications
      {
        serialEnabled = false;
        Serial.flush();
        Serial.println("SD");
        Serial.end();
        digitalWriteFast(SERIAL_LED, LOW);
      } 
      
      
      else if(command == "SS") // Scan size in LSBs
      {
        Serial.println("SS");
        boolean scanningEnabledOnCommand = scanningEnabled;
        int new_scanSize = Serial.parseInt();
        int xNew, yNew;
        
        // Calculate position to move to:
        xNew = (int)(((float)(x - xo) * (float)new_scanSize) / (float)scanSize) + xo;
        yNew = (int)(((float)(y - yo) * (float)new_scanSize) / (float)scanSize) + yo;
        
        scanningEnabled = false; // Pause the scan
        
        if(new_scanSize > scanSize) // Update scanSize, then move
        {
          scanSize = new_scanSize;
          moveTip(xNew, yNew);
        }
        else // Move, then update scanSize
        {
          moveTip(xNew, yNew);
          scanSize = new_scanSize;
        }
        if(scanningEnabledOnCommand) scanningEnabled = true; // Resume scanning
      }
      
      
      else if(command == "IP") // Image pixels
      {     
        Serial.println("IP");   
        boolean scanningEnabledOnCommand = scanningEnabled;
        pixelsPerLine = Serial.parseInt() * 2;
        resetScan();
        if(scanningEnabledOnCommand) scanningEnabled = true;
      }
      
      
      else if(command == "LR") // Line rate in Hz
      {
        Serial.println("LR");
        boolean scanningEnabledOnCommand = scanningEnabled;
        lineRate = (float)Serial.parseInt() / 100.0f; // Line rate is multiplied by 100 for the transmission
        while(pixelCounter != 0); // Wait for the scanner to finish scanning a line
        scanningEnabled = false; // Pause the scan
        updateStepSizes();
        if(scanningEnabledOnCommand) scanningEnabled = true; // Resume scan
      }
     
      
      else if(command == "XO") // X-offset
      {
        Serial.println("XO");
        boolean scanningEnabledOnCommand = scanningEnabled;
        int previous_xo = xo;
        int new_xo = Serial.parseInt();
        scanningEnabled = false; // Pause the scan
        xo = new_xo;
        moveTip(x - previous_xo + xo, y); // Move over by (xo, yo)
        if(scanningEnabledOnCommand) scanningEnabled = true; // Resume scan
      } 
      
      
      else if(command == "YO") // Y-offset
      {
        Serial.println("YO");
        boolean scanningEnabledOnCommand = scanningEnabled;
        int previous_yo = yo;
        int new_yo = Serial.parseInt();
        scanningEnabled = false;
        yo = new_yo;
        moveTip(x, y - previous_yo + yo);
        if(scanningEnabledOnCommand) scanningEnabled = true;
      } 
      
      
      else if(command == "SP") // Setpoint in LSBs
      {
        Serial.println("SP");
        setpoint = Serial.parseInt();
        setpointLog = logTable[abs(setpoint)];       
      } 
      
      
      else if(command == "SB") // Sample bias in LSBs
      {
        Serial.println("SB");
        bias = Serial.parseInt();
        noInterrupts();
        dac.setOutput((uint16_t)(bias + 32768), DAC_CH_BIAS); // Set the sample bias
        interrupts();          
      }
      
      
      else if(command == "KP") // P gain
      {
        Serial.println("KP");
        Kp = Serial.parseInt();          
      } 
      
      
      else if(command == "KI") // I gain
      {
        Serial.println("KI");
        Ki = Serial.parseInt() * dt;        
      }
      
      
      else if(command == "EN") // Enable scanning
      {
        Serial.println("EN");
        resetScan();
        scanningEnabled = true; 
      } 
      
      
      else if(command == "DL") // Disable scanning
      {
        Serial.println("DL");
        scanningEnabled = false;          
      } 
      
      
      else if(command == "TE") // Tip engage
      {        
        Serial.println("TE");
        engage();          
      } 
      
      
      else if(command == "TR") // Tip retract
      {        
        Serial.println("TR");
        retract();          
      }

      else if (command == "ME") {
        manualEnabled = true;
        Serial.println("Manual control enabled");
        }

        else if (command == "MP") {  // Manual +1
        if (manualEnabled) {
            stepMotors(1000, 1, 1);  // 1 step in positive direction
            Serial.println("Stepped +1");
        }
        }

        else if (command == "MN") {  // Manual -1
        if (manualEnabled) {
            (1000, 1, 0);  // 1 step in negative direction
            Serial.println("Stepped -1");
        }
        }
    }

}

/**************************************************************************/
