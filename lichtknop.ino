bool spacestate = false;
bool lichtstate = false;

const int spacestate_pin = 2;
const int knopje_pin = 3;
const int lichtstate_pin = 4;

void setup() {
  pinMode(spacestate_pin, INPUT);
  pinMode(knopje_pin, INPUT_PULLUP);
  pinMode(lichtstate_pin, OUTPUT);
  spacestate = digitalRead(spacestate_pin);
  lichtstate = spacestate;
}

void loop() {
  if(digitalRead(spacestate_pin) != spacestate) {
    spacestate = !spacestate;
    lichtstate = spacestate;
  }

  if(digitalRead(knopje_pin) == LOW) {
    lichtstate = !lichtstate; 
    delay(1000);
  }

}
