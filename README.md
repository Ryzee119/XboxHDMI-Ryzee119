# Original Xbox 100% Digital HDMI (Prototype)

* This is a prototype 100% digital Original Xbox HDMI addon PCB. It taps into the GPU pixel bus before it enters the analog video encoder and converts it to a HDMI compliant signal.
* It has not been fully tested and may not work for you. Use at your own risk. This should only really be a starting point to make something better.
* SPDIF digital audio is tapped off the Xbox motherboard and is also encoded into the HDMI signal for the best possible audio quality.
* This is a prototype board which should work on all Xbox revisions however a new flex would need to be designed to accomodate v1.6 motherboards with an Xcalibur encoder.
* 480i does not work reliably. Use a force 480p BIOS.

## Bill of materials
* See [Kitspace](https://kitspace.org/boards/github.com/ryzee119/XboxHDMI-Ryzee119/).
* Flatflex must be ordered separately. See [hardware](./hardware).

## Design Files
* Source code for the STM32F0 driver is [here](./src).
* Hardware design files are [here](./hardware).

## Installation

<p align="center"><img src=".images/install.jpg" alt="installation" width="80%"/></p>

* Photo may not match final PCB layout.
<p align="center"><img src=".images/proto.jpg" alt="installation" width="80%"/></p>
