# LoRa-P2P

> test code for Wemos D1 (and my first repository by the way =D )

It's a first version, which need to be cleaned a little bit, but it works.

The goal is to test LoRa range, with 2 LoRa nodes, one being the sender, the other the receiver.
They are made with a Wemos D1 Mini, a RFM95 module and a LoRa-Node PCB by Hallard (https://github.com/hallard/WeMos-Lora).

The sender sends messages and blinks a led (in blue) at the same time.

The receiver blinks a led (in green) when receiving a message.

The button (GPIO2) on the sender switches between differents sending frequencies:

0.2 Hz (default), 1Hz, 0.5Hz, 0.33Hz
