# Car timer

This project aims at developping, using Arduino, a system that measures the time it takes a car to complete a lap.

## Functionning

Everything is written in c++, using Arduino components.

To know when a car is passing by the beginning, a checkpoint or the arrival, I use ultrasonic distance sensors. The whole things are connecting using WiFi.

## Game

You can have user, so you can compare performances. There are many settings you can use. You will need an IR command to be able to communicate with the system. I personnaly use my phone, with the IrCode Finder app on Play store. Here is the list of the codes used : 

Name | Code 
| :---: | :---: |
| A | 00FFA25D |
| $\uparrow$ | 00FF02FD |
| Settings | 00FFA857 |
| $\leftarrow$ | 00FFE01F |
| Ok | 00FF50AF |
| $\rightarrow$ | 00FF906F |

... to be finished later

The SD card aims at saving all the times which were performed, and to save the config used for the game. 

There is also a solo game mode, which consists in making the timer start again each time you end the race, so you never need to stop.

## Components, power

To be writen later