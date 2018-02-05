/*
  By Mike Mulvenna
  Hidden dining table in Motor Home controlled by buttons, possibly remote,toggle switch, or voice. 
  When extended switch is pressed
   each actuator must complete befor the next actuator can be activated or parts could collide 
   Drawer face needs to be extended using actuator1
   hide drawer face using actuator2
   Slide containing table needs to be moved out using actuator3
   Table needs to be raised using actuator4 
  When retractd switch is pressed
       reverse the above 
MAYBE NEED TO CHANGE INPUTS TO "PULLUP"
*/
/*
   actuator1 is the face panel of the drawer
   actuator2 is the mechanism that will hide the drawer face panel
   actuator3 is the sliding part of the drawer that contains the table slide
   actuator4 is the mechanism that contains the actuator4 slide
    
*/
/* Things that could go wromg
 *  Power lost 
 *     opening process not finished.......code should protect need to test
 *     closing process not finished.......code should protect need to test
 *     Opening process fully complete.....code should protect need to test
 *     Closing process ful complete.......code should protect need to test
 *  Press the close button while opening is in process.......code should protect and not process the button
 *          need to test
 *  Press the open button while closing is in process.......code should protect and not process the button
 *          need to test
 *  Actuator fails .............................add timing check?
 *  one of the parts gets stuck.................figure out what to do
 *  Switches fail or act strangely .............change to input_pullup
 *  
 */




/* constants won't change during execution
 pin numbers set here to test code before connecting ardunio
 possibly need to change set here for testing

*/

// pins will be on sensor sheild

const int buttonextendedPin= 13;     // pushbutton pin to extended the actuators
const int buttonretractPin = 12;     // pushbutton pin to retract the actuators
const int actuator1relaypin = 5;     //  pins for the relays
const int actuator2relaypin = 6;
const int actuator3relaypin = 7;
const int actuator4relaypin = 8;
const int actuator1sensorpin = 1;    // pins for the potentiometers to get feedback from actuators
const int actuator2sensorpin = 2;
const int actuator3sensorpin = 3;
const int actuator4sensorpin = 4;

// variables will change during execution

boolean actuator1extended = false;
boolean actuator2extended = false;
boolean actuator3extended = false;
boolean actuator4extended = false;
int actuator1endposition = 350; // change to reflect the proper full extended position
int actuator2endposition = 350; // change to reflect the proper full extended position
int actuator3endposition = 350; // change to reflect the proper full extended position
int actuator4endposition = 350; // change to reflect the proper full extended position
int actuator1sensor = 0;
int actuator2sensor = 0;
int actuator3sensor = 0;
int actuator4sensor = 0;

int actuator1goalposition;
int actuator2goalposition;
int actuator3goalposition;
int actuator4goalposition;
int actuator1currentposition = 0;
int actuator2currentposition = 0;
int actuator3currentposition = 0;
int actuator4currentposition = 0;
int buttonextendedState = 0;       // variable for reading the extended pushbutton status
int buttonretractState= 0;         // variable for reading the retract pushbutton status 
/*   sample code ued to start this sketch 
    http://www.instructables.com/id/Control-a-Large-Linear-Actuator-With-Arduino/
*/
void setgoalpositions(int pos1,int pos2,int pos3,int pos4)
   {
    actuator1goalposition = pos1;
    actuator2goalposition = pos2;
    actuator3goalposition = pos3;
    actuator4goalposition = pos4;
   }
  

void setup() {
  //start serial connection
  Serial.begin(9600);
  // initialize both pushbutton pins as an input
  pinMode(buttonextendedPin, INPUT);  //maybe change this to input_pullup
  pinMode(buttonretractPin, INPUT);   //maybe change this to input_pullup 
  // initialize the relay pins as an output:
  pinMode(actuator1relaypin, OUTPUT);
  pinMode(actuator2relaypin, OUTPUT);
  pinMode(actuator3relaypin, OUTPUT);
  pinMode(actuator4relaypin, OUTPUT);
  setgoalpositions(actuator1endposition,actuator2endposition,actuator3endposition,actuator4endposition);
 
}  
void loop(){
  // read the current position of all the feedback actuators
  actuator1currentposition = analogRead(actuator1sensor);
  actuator2currentposition = analogRead(actuator2sensor);
  actuator3currentposition = analogRead(actuator3sensor);
  actuator4currentposition = analogRead(actuator4sensor);
  if (actuator1currentposition >= actuator1goalposition)
  {
     actuator1extended= true;
  }
  if (actuator2currentposition >= actuator2goalposition)
  {
     actuator2extended= true;
  }
  if (actuator3currentposition >= actuator3goalposition)
  {
     actuator3extended= true;
  }
  if (actuator4currentposition >= actuator4goalposition)
  {
     actuator4extended= true;
  }
  
  /* read the state of the switches to open or close the table parts
    probably need to change this logic  for a remote or a toggel switch
  */
  if (buttonextendedState == LOW)
  {
   buttonextendedState = digitalRead(buttonextendedPin);
  } 
  if (buttonretractState == LOW)
  {
    buttonretractState = digitalRead(buttonretractPin);
  }
  // check if the extended pushbutton is pressed. If it is, the button State is HIGH:
  if (buttonextendedState == HIGH and buttonretractState ==LOW) 
  {
    if (actuator1extended == false
      and actuator2extended == false 
      and actuator3extended == false
      and actuator4extended == false) 
    {
       // check the position of the drawer face
       if(actuator1goalposition > actuator1currentposition)
       {
           // keep extending until the the drawer face is opened fully 
           digitalWrite(actuator1relaypin, HIGH);  
       }    
       else
       {
          // drawer face is opened fully so stop actuator1
           digitalWrite(actuator1relaypin, LOW);   
           actuator1extended = true; 
       }
    }
    //now if actuator1 is extended process then extend actuator 2 to hide the drawer face
    if (actuator1extended == true
      and actuator2extended == false 
      and actuator3extended == false
      and actuator4extended == false) 
    {
       if(actuator2goalposition > actuator2currentposition)
       {
           // keep extending until the the drawer face is hidden
           digitalWrite(actuator2relaypin, HIGH);  
       }    
       else
       {
           //  drawer face is hidden so stop actuator 2 
           digitalWrite(actuator2relaypin, LOW);   
           actuator2extended = true; 
       }
    }  
    /*now if actuator1 is extended 
          and actuator 2 is extended 
          process and extend actuator 3 to open the drawer slide
    */
    if (actuator1extended == true
      and actuator2extended == true 
      and actuator3extended == false
      and actuator4extended == false) 
    {
       if(actuator3goalposition > actuator3currentposition)
       {
            // keep extending until drawer slide is fully opened
           digitalWrite(actuator3relaypin, HIGH);  
       }    
       else
       {
           // slide is opened so turn off the actuator
           digitalWrite(actuator3relaypin, LOW);   
           actuator3extended = true; 
       }
    }    
    /*now if actuator1 is extended 
         and actuator2 is extended 
         and actuator3 is extended 
         process and extend actuator4 to rasie the table
    */
    if (actuator1extended == true
      and actuator2extended == true 
      and actuator3extended == true
      and actuator4extended == false) 
    {
       if(actuator4goalposition > actuator4currentposition)
       {
           // keep extending until table is fully raised
           digitalWrite(actuator4relaypin, HIGH);  
       }    
       else
       {
           // tables is fully raised so turn off actuator
           digitalWrite(actuator4relaypin, LOW);   
           actuator4extended = true;
           setgoalpositions(0,0,0,0); 
       }
    }
 } 
 if (buttonretractState == HIGH and buttonextendedState == LOW) {
    if (actuator1extended == true
        and actuator2extended == true 
        and actuator3extended == true
      and actuator4extended == true) 
    {
       if(actuator4currentposition > 0)
       {
           // keep retracting until table is fully lowered
           digitalWrite(actuator4relaypin,HIGH);  
       }    
       else
       {
           actuator4extended = false; 
       }
    }
    if (actuator1extended == true
      and actuator2extended == true 
      and actuator3extended == true
      and actuator4extended == false) 
    {
       if(actuator3currentposition > 0)
       {
           // keep retracting until slide is in
           digitalWrite(actuator3relaypin,HIGH);  
       }    
       else
       {
           actuator3extended = false; 
       }
    }
      

    if (actuator1extended == true
      and actuator2extended == true 
      and actuator3extended == false
      and actuator4extended == false) 
    {
       if(actuator2currentposition > 0)
       {
           // keep retracting until slide is in
           digitalWrite(actuator2relaypin,HIGH);  
       }    
       else
       {
           actuator2extended = false; 
       }
    }
    if (actuator1extended == true
      and actuator2extended == false 
      and actuator3extended == false
      and actuator4extended == false) 
    {
       if(actuator1currentposition > 0)
       {
           // keep retracting until slide is in
           digitalWrite(actuator1relaypin,HIGH);  
       }    
       else
       {
         actuator1extended = false; 
         actuator1goalposition = actuator1endposition;
         actuator2goalposition = actuator2endposition;
         actuator3goalposition = actuator3endposition;
         actuator4goalposition = actuator4endposition;

       }
    }
 } 
} 
