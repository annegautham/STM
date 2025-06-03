#include <Arduino.h>
#include "globals.hpp"
#include "SerialCommands.hpp"


// void checkSerial()
// {
  
//   String serialString;
  
//   if(ESPSerial.available() > 0)
//   {
//     for(int i = 0; i < 2; i++)  // Accepts 2 char commands
//     {
//       delay(1); // This seems to be necessary, not sure why
//       char inChar = ESPSerial.read();
//       serialString += inChar;
//     }
//   }
//   serialCommand(serialString);
//   // serialString = "";
// }

void checkSerial() {
  while (ESPSerial.available()) {
    String serialString = ESPSerial.readStringUntil('\n');
    serialString.trim();  // Remove trailing \r or whitespace

    if (serialString.length() > 0) {
      console.print("[From ESP32] ");
      console.println(serialString);
      serialCommand(serialString);
    }
  }
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
        console.begin(115200);
        console.println("SE");
        serialEnabled = true;
        digitalWriteFast(SERIAL_LED, HIGH);
      }
      
      
      else if(command == "SD") // Disable serial communications
      {
        serialEnabled = false;
        ESPSerial.flush();
        console.println("SD");
        ESPSerial.end();
        digitalWriteFast(SERIAL_LED, LOW);
      } 
      
      
      else if(command == "SS") // Scan size in LSBs
      {
        console.println("SS");
        boolean scanningEnabledOnCommand = scanningEnabled;
        int new_scanSize = ESPSerial.parseInt();
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
        console.println("IP");   
        boolean scanningEnabledOnCommand = scanningEnabled;
        pixelsPerLine = ESPSerial.parseInt() * 2;
        resetScan();
        if(scanningEnabledOnCommand) scanningEnabled = true;
      }
      
      
      else if(command == "LR") // Line rate in Hz
      {
        console.println("LR");
        boolean scanningEnabledOnCommand = scanningEnabled;
        lineRate = (float)ESPSerial.parseInt() / 100.0f; // Line rate is multiplied by 100 for the transmission
        while(pixelCounter != 0); // Wait for the scanner to finish scanning a line
        scanningEnabled = false; // Pause the scan
        updateStepSizes();
        if(scanningEnabledOnCommand) scanningEnabled = true; // Resume scan
      }
     
      
      else if(command == "XO") // X-offset
      {
        console.println("XO");
        boolean scanningEnabledOnCommand = scanningEnabled;
        int previous_xo = xo;
        int new_xo = ESPSerial.parseInt();
        scanningEnabled = false; // Pause the scan
        xo = new_xo;
        moveTip(x - previous_xo + xo, y); // Move over by (xo, yo)
        if(scanningEnabledOnCommand) scanningEnabled = true; // Resume scan
      } 
      
      
      else if(command == "YO") // Y-offset
      {
        console.println("YO");
        boolean scanningEnabledOnCommand = scanningEnabled;
        int previous_yo = yo;
        int new_yo = ESPSerial.parseInt();
        scanningEnabled = false;
        yo = new_yo;
        moveTip(x, y - previous_yo + yo);
        if(scanningEnabledOnCommand) scanningEnabled = true;
      } 
      
      
      else if(command == "SP") // Setpoint in LSBs
      {
        console.println("SP");
        setpoint = ESPSerial.parseInt();
        setpointLog = logTable[abs(setpoint)];       
      } 
      
      
      else if(command == "SB") // Sample bias in LSBs
      {
        console.println("SB");
        bias = ESPSerial.parseInt();
        noInterrupts();
        dac.setOutput((uint16_t)(bias + 32768), DAC_CH_BIAS); // Set the sample bias
        interrupts();          
      }
      
      
      else if(command == "KP") // P gain
      {
        console.println("KP");
        Kp = ESPSerial.parseInt();          
      } 
      
      
      else if(command == "KI") // I gain
      {
        console.println("KI");
        Ki = ESPSerial.parseInt() * dt;        
      }
      
      
      else if(command == "EN") // Enable scanning
      {
        console.println("EN");
        resetScan();
        scanningEnabled = true; 
      } 
      
      
      else if(command == "DL") // Disable scanning
      {
        ESPSerial.println("DL");
        scanningEnabled = false;          
      } 
      
      
      else if(command == "TE") // Tip engage
      {        
        ESPSerial.println("TE");
        engage();          
      } 
      
      
      else if(command == "TR") // Tip retract
      {        
        ESPSerial.println("TR");
        retract();          
      }

      else if (command == "TM") 
      {  // Toggle manual mode
        manualEnabled = !manualEnabled;
        console.print("Manual control ");
        console.println(manualEnabled ? "enabled" : "disabled");
      }
      else if (command == "MP") 
      {  // Manual +1
        if (manualEnabled) {
            stepMotors(1000, 1, 1000);  // 1 step in positive direction
            console.println("Stepped +1");
        }
      }

      else if (command == "MN") 
      {  // Manual -1
        if (manualEnabled) {
          stepMotors(1000, 0, 1000);  // 1 step in negative direction
          console.println("Stepped -1");
        }       
      }
    }

}

/**************************************************************************/
