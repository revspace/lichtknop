# Lichtknop (light button)

This projects contains code (and somehow also documentation) for the light ~~switches~~ switching logic at RevSpace.

The lights should:
* Switch on when the space opens
* Switch off when the space closes
* Toggle their state when one of the push buttons is pushed
* Do all of the above reliable (not on an esp8266, regardless of network, etc)
* Switch off for 25 seconds when the nomz button is pushed
* Tell us their state so we can calculate power usage elsewhere

## `arduino`

This code runs on the two arduinos inside the two *main switch thingies*. It listens to spacestate and the push buttons, and switches a solid state relay which then switches a contactor (Schneider Electric LC1D09) which then switches the lights.

## `esp8266`

This code runs on the nodemcuv2 clone in the *auxiliary switch thingie*. It listens for MQTT topics (currently only `revspace/button/nomz`) and turns off the lights by interrupting the spacestate signal. It also listens for the helper contacts on the contactor to report the state on MQTT.

![Photo of the switch thingies](./photo.jpg)
