#include "Reboot_to_boot.h"



// the setup function runs once when you press reset or power the board
void setup() {
  
//reboot_setup(int delay, char pin, char flag).delay is the timer timeout for reboot,pin is which pin is for LED to set ,flag is for selecting/unselecting led functionality. 
// timer timeout for delay of 10 is 2.4 sec.

  reboot_setup(20,1,1);
  
 
}



// the loop function runs over and over again forever
void loop() {
   delay(1000);

 
}
