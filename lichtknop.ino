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

             }  +  o---+----------------------------------- lichtstate_pin
             }         |
  licht-SSR  }         +---\/\/\--->|---+
             }                          |   
             }  -  o--------------------+--- GND
*/

#include <avr/wdt.h>

bool spacestate = false;
bool lichtstate = false;

const int spacestate_pin = 8;
const int knopje_pin = 2;
const int lichtstate_pin = 4;
const int led_pin = 13;

void ignore_input (unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    digitalWrite(led_pin, millis() % 100 < 50);
  }
}

bool read_knopje() {
  static bool previous = false;
  unsigned long start = millis();

  bool current;

  while (millis() - start < 20) {
    current = !digitalRead(knopje_pin);
    if (current == previous) {
      // button bounced or unchanged; ignore input
      return false;
    }
  }
  return previous = current;
}

void setup() {
  wdt_enable(WDTO_2S);
  pinMode(spacestate_pin, INPUT_PULLUP);
  pinMode(knopje_pin, INPUT_PULLUP);
  pinMode(lichtstate_pin, OUTPUT);
  spacestate = LOW;
  lichtstate = spacestate;
  digitalWrite(lichtstate_pin, lichtstate);
  ignore_input(1000);
}

void loop() {
  wdt_reset();
  digitalWrite(led_pin, millis() % 1000 < 500);

  bool spaceinput = !digitalRead(spacestate_pin);
  if(spaceinput != spacestate) {
    lichtstate = spacestate = spaceinput;
    digitalWrite(lichtstate_pin, lichtstate);
    ignore_input(1000);
  }
  else if(read_knopje()) {
    lichtstate = !lichtstate;
    digitalWrite(lichtstate_pin, lichtstate);
    ignore_input(1000);
  }
}
