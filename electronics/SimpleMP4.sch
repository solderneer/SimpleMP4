EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L USB_OTG J?
U 1 1 5A2F7CFC
P 1050 1100
F 0 "J?" H 850 1550 50  0000 L CNN
F 1 "USB_OTG" H 850 1450 50  0000 L CNN
F 2 "" H 1200 1050 50  0001 C CNN
F 3 "" H 1200 1050 50  0001 C CNN
	1    1050 1100
	1    0    0    -1  
$EndComp
$Comp
L L L?
U 1 1 5A2F7D48
P 1600 900
F 0 "L?" V 1550 900 50  0000 C CNN
F 1 "600R/100M" V 1675 900 50  0000 C CNN
F 2 "" H 1600 900 50  0001 C CNN
F 3 "" H 1600 900 50  0001 C CNN
	1    1600 900 
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5A2F7D9D
P 1700 1550
F 0 "#PWR?" H 1700 1300 50  0001 C CNN
F 1 "GND" H 1700 1400 50  0000 C CNN
F 2 "" H 1700 1550 50  0001 C CNN
F 3 "" H 1700 1550 50  0001 C CNN
	1    1700 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  1500 2400 1500
Wire Wire Line
	1700 1400 1700 1550
Connection ~ 1050 1500
Wire Wire Line
	1350 1300 1350 1500
Connection ~ 1350 1500
$Comp
L R R?
U 1 1 5A2F7DE4
P 1500 1350
F 0 "R?" V 1580 1350 50  0000 C CNN
F 1 "1M" V 1500 1350 50  0000 C CNN
F 2 "" V 1430 1350 50  0001 C CNN
F 3 "" H 1500 1350 50  0001 C CNN
	1    1500 1350
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A2F7E16
P 1700 1250
F 0 "R?" V 1780 1250 50  0000 C CNN
F 1 "1M" V 1700 1250 50  0000 C CNN
F 2 "" V 1630 1250 50  0001 C CNN
F 3 "" H 1700 1250 50  0001 C CNN
	1    1700 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 1100 1700 1100
Connection ~ 1700 1500
Wire Wire Line
	1350 1200 1500 1200
Connection ~ 1500 1500
$Comp
L C C?
U 1 1 5A2F7EE0
P 1950 1250
F 0 "C?" H 1975 1350 50  0000 L CNN
F 1 "104" H 1950 1150 50  0000 L CNN
F 2 "" H 1988 1100 50  0001 C CNN
F 3 "" H 1950 1250 50  0001 C CNN
	1    1950 1250
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A2F7F17
P 2150 1000
F 0 "R?" V 2230 1000 50  0000 C CNN
F 1 "10K" V 2150 1000 50  0000 C CNN
F 2 "" V 2080 1000 50  0001 C CNN
F 3 "" H 2150 1000 50  0001 C CNN
	1    2150 1000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A2F7F41
P 2150 1350
F 0 "R?" V 2230 1350 50  0000 C CNN
F 1 "12K" V 2150 1350 50  0000 C CNN
F 2 "" V 2080 1350 50  0001 C CNN
F 3 "" H 2150 1350 50  0001 C CNN
	1    2150 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 900  1450 900 
Wire Wire Line
	1750 900  1950 900 
Wire Wire Line
	1950 850  1950 1100
Wire Wire Line
	1950 1500 1950 1400
Wire Wire Line
	1950 850  2150 850 
Connection ~ 1950 900 
Wire Wire Line
	2150 1150 2150 1200
Connection ~ 1950 1500
$Comp
L C C?
U 1 1 5A2F8068
P 2400 1350
F 0 "C?" H 2425 1450 50  0000 L CNN
F 1 "104" H 2425 1250 50  0000 L CNN
F 2 "" H 2438 1200 50  0001 C CNN
F 3 "" H 2400 1350 50  0001 C CNN
	1    2400 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1200 2800 1200
Connection ~ 2150 1200
Connection ~ 2150 1500
$Comp
L +5V #PWR?
U 1 1 5A2F8222
P 2150 800
F 0 "#PWR?" H 2150 650 50  0001 C CNN
F 1 "+5V" H 2150 940 50  0000 C CNN
F 2 "" H 2150 800 50  0001 C CNN
F 3 "" H 2150 800 50  0001 C CNN
	1    2150 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 850  2150 800 
Connection ~ 2400 1200
Text Label 1350 1200 0    60   ~ 0
DM
Text Label 1350 1100 0    60   ~ 0
DP
Text Notes 850  1900 0    60   ~ 0
Check footprint of the USB conn J1
Text Label 2800 1200 2    60   ~ 0
USB_DET
$EndSCHEMATC
