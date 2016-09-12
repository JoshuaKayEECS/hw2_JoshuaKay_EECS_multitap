
/*
 * text entry redbear duo code
 * Josh Kay
 * Iteractive Device Design HW2
 * 9-9-16
*/

#define LED D7   // select the pin for the LED
#define num_cols 3
#define num_rows 4

#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif

int colPin[num_cols] = {D10, D9, D8};
int rowPin[num_rows] = {D3,D2,D1,D0};


long debounceDelay = 250;    // the debounce time in ms
int timeout = 2000;
volatile int state = LOW;
int i = 97;
int last_col = -1;
int last_row = -1;
int pressed_time;
int unpressed_time;
int wait_time;
bool waiting = 0;
bool t_trig = 0;
int num_switches = 8;
int switch_position = 0;
int shift = 0;
bool caps = 0;
bool shift_pressed = 0;
bool space_pressed = 0;

void setup() {
  pinMode(LED,OUTPUT);   // declare the LED's pin as output
  for(int c=0;c<num_cols;c++){
    pinMode(colPin[c],INPUT_PULLUP); 
    //attachInterrupt(colPin[c], isr, FALLING); //CHANGE, RISING or FALLING 
  }
  for(int r=0;r<num_rows;r++){
    pinMode(rowPin[r],OUTPUT);
  }   
  Serial.begin(9600);        // connect to the serial port
  digitalWrite(LED,LOW);
}

void loop(){

    //loop through rows
    for(int r=0;r<num_rows;r++){

    //switch row low
      digitalWrite(rowPin[r],LOW);

      //loop through columns
      for(int c=0;c<num_cols;c++){
        //if colummn pin low, button pressed at row r, col c
        if(!digitalRead(colPin[c])){

/*
            Serial.print("Row: ");
            Serial.println(r);
            Serial.print("Column: ");
            Serial.println(c);
*/
          //get time button pressed      
          pressed_time = millis();
          waiting = 1;
          
          // get if the last key is pressed
          if(c == last_col && r == last_row){

            send_char(r,c);

            //Increment ASCHII number
            i++;

            // Start the 4 letter keys over
            if((r== 1 && c==1) || (r ==1 && c == 2)){
              if((i-97==4) || (i-65==4)){
                i = 97;
              }              
            }
            // Start the 3 letter keys over
            else{
              if((i-97==3) || (i-65==3)){
                i = 97;
              }
            }
            t_trig = 0;
        }
        
        // new key pressed
        else{
          i = 97;
          //waiting = 0;
          /*
          Serial.print("space_pressed: ");
          Serial.println(space_pressed);
          Serial.print("t_trig: ");
          Serial.println(t_trig);*/
          if((!t_trig) && !(r==3 && c == 1) && !(r==0 && c==0)){
          // send next character indicator  
            Serial.println(-1);
          }
          t_trig = 0;
          send_char(r,c);
          i++;
        }
        // record last key
        last_col = c;
        last_row = r;
      }
      // debounce loop
      unpressed_time = millis();
      while(!digitalRead(colPin[c]) || ((unpressed_time - pressed_time) < debounceDelay)){
            unpressed_time = millis();
      }
    }
      //move forward after timeout
      wait_time = millis();
      if((wait_time - pressed_time > timeout) && waiting){
        Serial.println(-1);
        i = 97;
        waiting = 0;
        t_trig = 1;
      }
    //write the row pins high again
    digitalWrite(rowPin[r],HIGH);
  }
}

void send_char(int r, int c){
          //shift key
          shift_pressed = 0;
          space_pressed = 0;
          if(r == 0 && c == 0){
            if(!caps){
              shift = 32;
            }
            else{
              shift = 0;
            }
            caps = !caps;
            shift_pressed = 1;
            /*
            Serial.print("caps: ");
            Serial.println(caps);
            Serial.print("shift: ");
            Serial.println(shift);
            */
            t_trig = 0;
          }
          // space key pressed
          else if((r== 3 && c == 1) && shift == 0){
            Serial.println(-1);
            Serial.println(32);
           // Serial.println(-1);
            space_pressed = 1;
            t_trig = 0;
            waiting = 0;
          }
          else if((r == 3 && c == 1) && shift == 32){
            Serial.println(-2);
            waiting = 0;
          }
          // Greater than row 1 make up for 4 letter keys in middle by adding two
          else if(r > 1){
            Serial.println(i+9*r+3*c-1-shift);
          }
          // Greater than 1st 4 letter key in middle, add 1
          else if(r == 1 && c == 2){
            Serial.println(i+9*r+3*c-2-shift);
          }
          // Get ASCHII number for button pushed
          else{
            Serial.println(i+9*r+3*c-3-shift);
          }
}


