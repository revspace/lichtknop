/*
                                                   coll.
             }  +  o---+---\/\/\------------+     +-------- spacestate_pin
             }         |                    |    /
  spacestate }         +---\/\/\--->|---+   V ⇉ K
             }                          |   |    \ em.
             }  -  o--------------------+---+     +--- GND

             }     o---+----------------------------------- knopje_pin
             }         |
  knopje     }         : |-
             }         |
             }     o---+--- GND

             }  +  o--------------------------------------- lichtstate_pin
             }         |
  licht-SSR  }         +---\/\/\--->|---+
             }                          |   
             }  -  o--------------------+--- GND
*/

bool spacestate = false;
bool lichtstate = false;

const int spacestate_pin = 8;
const int knopje_pin = 3;
const int lichtstate_pin = 4;
const int led_pin = 13;

void ignore_input (unsigned long ms) {
  unsigned long target = millis() + ms;
  while (millis() - target >= 0) digitalWrite(led_pin, millis() % 200 < 100);
}

void setup() {
  pinMode(spacestate_pin, INPUT_PULLUP);
  pinMode(knopje_pin, INPUT_PULLUP);
  pinMode(lichtstate_pin, OUTPUT);
  spacestate = digitalRead(spacestate_pin);
  lichtstate = spacestate;
  digitalWrite(lichtstate_pin, lichtstate);
}

void loop() {
  digitalWrite(led_pin, millis() % 1000 < 500);

  bool spaceinput = !digitalRead(spacestate_pin);
  if(spaceinput != spacestate) {
    lichtstate = spacestate = spaceinput;
    digitalWrite(lichtstate_pin, lichtstate);
    ignore_input(1000);
  }
  else if(digitalRead(knopje_pin) == LOW) {
    lichtstate = !lichtstate;
    digitalWrite(lichtstate_pin, lichtstate);
    ignore_input(1000);
  }
}
