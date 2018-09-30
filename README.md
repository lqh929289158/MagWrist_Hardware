# MagWrist_Hardware

## About MagWrist
This is a project created by **QINGHAO LIU**.
The basic idea is that:

The program of the system has two parts: Hardware(Embedded Board) and Software(PC).

This README _Only_ talks about **HARDWARE** part.

## Install Keil(TI Cortex-M4 Development Environment)

Due to the problem of copyright. Ask your professor for the installation program, which may contains 5 files.

Installation program name | Details
------------------------- | -------
MDK522.EXE | The main setup file of Keil 5.22
MDKCM522.EXE | The setup file for Keil legacy devices
Keil.TM4C_DFP.1.1.0.pack | Tiva C Series device support and examples
SW-EK-TM4C1294XL-2.1.4.178.exe | Firmware development package setup
uniflash_sl.4.1.1250.exe | Uniflash setup(Maybe no use)

## Test the hardware program

1. Download this repository to your PC.
2. Find the **MagWrist_ReadData.uvprojx** file and open it with Keil.
(picture here about the postion of the src file)
3. Double click the **MagWrist_ReadData.c** file on the left side.
4. Configuration.
   1. (Read Data Rate)
   2. (Measurement Scale)
5. Click on the **Rebuild** button on the up-left side.
   1. If **0 Error(s)**, then _Congratulations!_(Ignore the warnings)
   2. If there is any error, try debug it by yourself first.
      1. Check whether there is any syntax error.(Maybe you tapped your keyboard by mistake)
      2. Right click **Target1** on the left side and choose **Options for Target 'Target 1'**.
      3. Click **Device** and check you have chosen **TM4C1294NCPDT**.
      4. Click **C/C++** and check **Include Paths**  is `.\inc;.\driverlib_`.
   3. If you have no idea, ask your professor.
6. Ready to download the program to your embedded board.
      

