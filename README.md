
# Time-Controlled Smart Switch
It's a personal project developed by me for the purpose of turning on/off external devices in my house according to the current time.  

![image](https://github.com/GuiJoseh/PowerPlugController/assets/94620888/6e15b9f1-876a-4757-9cce-11b1258937c9)

To turn on/off the devices, you need to use relays on pins P1.2, P1.3, and P1.4. Each pin has its own turn-on and turn-off times which can be set by the user using the buttons. The current time on the RTC sensor can also be adjusted.  
To compile this project, you need to install SDCC and run the .bat file.  
The .hex file will be created, and you can write this on the microcontroller AT89C51 or test it in some simulation program like Proteus.  
This project was developed on Proteus, so you can find the Proteus file with the project.  
The components used were:  
- 1x Microcontroller AT89C51  
- 1x sensor RTC DS1307  
- 3x Commons buttons  
- 3x 10K resistors  
- 1x buzzer  
- 1x 1K resistor  
- 1x PN2222 transistor  
- 1x 16x2 LCD panel  
