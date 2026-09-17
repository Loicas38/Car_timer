# Car timer

This project aims at developing, using Arduino, a system which measures the time needed for a car to complete a lap on a circuit, with checkpoints. In addition, it allows to create several users, who will have their personal records, the historic of their runs, ... Global records are also implemented. Finally, there is a single player mode, and many parameters.

## Game functioning

The system works with ultrasonic sensors, which allow to detect when a car passes by the different units. So, at first the car passes by the main unit, which launches the timer. Then, it has to pass by the checkpoints in the good order, and it will give it its intermediate times. If the first checkpoint was not checked, the second one can't be checked by default. It can be changed into the parameters. Identically, if the checkpoint is touched while it's checked, the check isn't valid by default. The race can end both with a checkpoint or the central unit.

There is also a SD card on the system, so it records your times and stats on it, and you can both check it later on your computer (with an app I will write later) or when playing on the Arduino.

The SD card also allows you to save your parameters, users, ...

To communicate with the game, you will need IR. Personally, I use IrCode Finder, which is available on PlayStore. Here are the codes I use (and you will need to put into the app) : (the tab isn't up to date at the time)

Name | Code 
| :---: | :---: |
| A | 00FFA25D |
| $\uparrow$ | 00FF02FD |
| Settings | 00FFA857 |
| $\leftarrow$ | 00FFE01F |
| Ok | 00FF50AF |
| $\rightarrow$ | 00FF906F |


## Technical details

