/*
  Third-Party ATX Extension for the JetKVM
*/

const int I_PWR_LED = 2;
const int I_HDD_LED = 4;
const int O_PWR_BTN = 8;
const int O_RST_BTN = 9;

String serial_input = "";

int new_is_hdd_off = 1;
int new_is_pwr_off = 1;
int new_is_rst_pressed = 0;
int new_is_pwr_pressed = 0;

int old_is_hdd_off = 1;
int old_is_pwr_off = 1;
int old_is_rst_pressed = 0;
int old_is_pwr_pressed = 0;

void setup() {
  Serial.begin(115200);
  pinMode(O_RST_BTN, OUTPUT);           //OUTPUT for PWR_BTN
  pinMode(O_PWR_BTN, OUTPUT);           //OUTPUT for RST_BTN
  pinMode(I_PWR_LED, INPUT_PULLUP);     //INPUT for PWR_LED
  pinMode(I_HDD_LED, INPUT_PULLUP);     //INPUT for HDD_LED
}

void loop() {

  //Save serial input
  while(Serial.available()){
    char c = Serial.read();
    serial_input += c;
    delay(5);
  }

  serial_input.replace("\r", "");
  serial_input.replace("\n", "");
  
  if (serial_input.length() >0){
    //Case 1: Request to push power button
    if (serial_input == "BTN_PWR_ON"){
      digitalWrite(O_PWR_BTN, HIGH);
      new_is_pwr_pressed = 1;
    }

    if (serial_input == "BTN_PWR_OFF"){
      digitalWrite(O_PWR_BTN, LOW);
      new_is_pwr_pressed = 0;
    }
  
    //Case 2: Request to push reset button
    if (serial_input == "BTN_RST_ON"){
      digitalWrite(O_RST_BTN, HIGH);
      new_is_rst_pressed = 1;
    }

    if (serial_input == "BTN_RST_OFF"){
      digitalWrite(O_RST_BTN, LOW);
      new_is_rst_pressed = 0;
    }
  }
  
  //Case 3: HDD LED activity
  if (digitalRead(I_HDD_LED) == LOW) {
    new_is_hdd_off = 0;
  } else {
    new_is_hdd_off = 1;
  }
  
  //Case 4: Power LED activity
  if (digitalRead(I_PWR_LED) == LOW) {
    new_is_pwr_off = 0;
  } else {
    new_is_pwr_off = 1;
  }
  
  //Send response if anything changed
  if (new_is_hdd_off != old_is_hdd_off || new_is_pwr_off != old_is_pwr_off || new_is_rst_pressed != old_is_rst_pressed || new_is_pwr_pressed != old_is_pwr_pressed){
    Serial.print(new_is_hdd_off);
    Serial.print(new_is_pwr_off);
    Serial.print(new_is_rst_pressed);
    Serial.print(new_is_pwr_pressed);
    Serial.print("\n");

    old_is_hdd_off = new_is_hdd_off;
    old_is_pwr_off = new_is_pwr_off;
    old_is_rst_pressed = new_is_rst_pressed;
    old_is_pwr_pressed = new_is_pwr_pressed;
  }

  //Reset Serial input after each cycle
  serial_input = "";
}
