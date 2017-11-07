EESchema Schematic File Version 2
LIBS:xling
LIBS:74xgxx
LIBS:74xx
LIBS:ac-dc
LIBS:actel
LIBS:adc-dac
LIBS:allegro
LIBS:Altera
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:battery_management
LIBS:bbd
LIBS:bosch
LIBS:brooktre
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:conn
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:diode
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:graphic_symbols
LIBS:hc11
LIBS:infineon
LIBS:intel
LIBS:interface
LIBS:intersil
LIBS:ir
LIBS:Lattice
LIBS:leds
LIBS:LEM
LIBS:linear
LIBS:logic_programmable
LIBS:maxim
LIBS:mechanical
LIBS:memory
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic24mcu
LIBS:microchip_pic32mcu
LIBS:microchip
LIBS:microcontrollers
LIBS:modules
LIBS:motor_drivers
LIBS:motorola
LIBS:motors
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp_armmcu
LIBS:nxp
LIBS:onsemi
LIBS:opto
LIBS:Oscillators
LIBS:philips
LIBS:Power_Management
LIBS:power
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:rfcom
LIBS:RFSolutions
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:switches
LIBS:texas
LIBS:transf
LIBS:transistors
LIBS:triac_thyristor
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:wiznet
LIBS:Worldsemi
LIBS:Xicor
LIBS:xilinx
LIBS:zetex
LIBS:Zilog
LIBS:xling-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
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
L ATMEGA128-16AU U?
U 1 1 59FC4968
P 4610 7170
F 0 "U?" H 3460 9070 50  0000 L BNN
F 1 "ATMEGA128-16AU" H 5080 5220 50  0000 L BNN
F 2 "TQFP64" H 4610 7170 50  0001 C CIN
F 3 "" H 4610 7170 50  0001 C CNN
	1    4610 7170
	1    0    0    -1  
$EndComp
$Comp
L MCP73831 J?
U 1 1 59FC873F
P 2820 1920
F 0 "J?" H 2620 2220 60  0000 C CNN
F 1 "MCP73831" H 2820 1620 60  0000 C CNN
F 2 "" H 2820 1920 60  0001 C CNN
F 3 "" H 2820 1920 60  0001 C CNN
	1    2820 1920
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59FC8778
P 3430 2420
F 0 "R?" V 3330 2420 50  0000 C CNN
F 1 "2K2" V 3530 2420 50  0000 C CNN
F 2 "" V 3360 2420 50  0001 C CNN
F 3 "" H 3430 2420 50  0001 C CNN
	1    3430 2420
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59FC87A3
P 2070 3550
F 0 "R?" V 2160 3550 50  0000 C CNN
F 1 "470R" V 1960 3550 50  0000 C CNN
F 2 "" V 2000 3550 50  0001 C CNN
F 3 "" H 2070 3550 50  0001 C CNN
	1    2070 3550
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 59FC87D6
P 2070 2070
F 0 "R?" V 1970 2070 50  0000 C CNN
F 1 "470R" V 2170 2070 50  0000 C CNN
F 2 "" V 2000 2070 50  0001 C CNN
F 3 "" H 2070 2070 50  0001 C CNN
	1    2070 2070
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 59FC8802
P 1720 2270
F 0 "C?" H 1745 2370 50  0000 L CNN
F 1 "10nF" H 1745 2170 50  0000 L CNN
F 2 "" H 1758 2120 50  0001 C CNN
F 3 "" H 1720 2270 50  0001 C CNN
	1    1720 2270
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 59FC8830
P 3730 2420
F 0 "C?" H 3755 2520 50  0000 L CNN
F 1 "10nF" H 3755 2320 50  0000 L CNN
F 2 "" H 3768 2270 50  0001 C CNN
F 3 "" H 3730 2420 50  0001 C CNN
	1    3730 2420
	1    0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 59FC8872
P 2070 3070
F 0 "D?" H 2070 3170 50  0000 C CNN
F 1 "green" H 2070 2920 50  0000 C CNN
F 2 "" H 2070 3070 50  0001 C CNN
F 3 "" H 2070 3070 50  0001 C CNN
	1    2070 3070
	0    -1   -1   0   
$EndComp
$Comp
L LED D?
U 1 1 59FC88AD
P 2070 2520
F 0 "D?" H 2070 2620 50  0000 C CNN
F 1 "red" H 2070 2370 50  0000 C CNN
F 2 "" H 2070 2520 50  0001 C CNN
F 3 "" H 2070 2520 50  0001 C CNN
	1    2070 2520
	0    -1   -1   0   
$EndComp
$Comp
L USB_OTG J?
U 1 1 59FC88F3
P 1070 1970
F 0 "J?" H 870 2420 50  0000 L CNN
F 1 "USB_OTG" H 870 2320 50  0000 L CNN
F 2 "" H 1220 1920 50  0001 C CNN
F 3 "" H 1220 1920 50  0001 C CNN
	1    1070 1970
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 59FC893B
P 1720 2720
F 0 "#PWR01" H 1720 2470 50  0001 C CNN
F 1 "GND" H 1720 2570 50  0000 C CNN
F 2 "" H 1720 2720 50  0001 C CNN
F 3 "" H 1720 2720 50  0001 C CNN
	1    1720 2720
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 59FC896D
P 2070 3860
F 0 "#PWR02" H 2070 3610 50  0001 C CNN
F 1 "GND" H 2070 3710 50  0000 C CNN
F 2 "" H 2070 3860 50  0001 C CNN
F 3 "" H 2070 3860 50  0001 C CNN
	1    2070 3860
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 59FC89D1
P 3430 2870
F 0 "#PWR03" H 3430 2620 50  0001 C CNN
F 1 "GND" H 3430 2720 50  0000 C CNN
F 2 "" H 3430 2870 50  0001 C CNN
F 3 "" H 3430 2870 50  0001 C CNN
	1    3430 2870
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 59FC955F
P 1070 2720
F 0 "#PWR04" H 1070 2470 50  0001 C CNN
F 1 "GND" H 1070 2570 50  0000 C CNN
F 2 "" H 1070 2720 50  0001 C CNN
F 3 "" H 1070 2720 50  0001 C CNN
	1    1070 2720
	1    0    0    -1  
$EndComp
Text Notes 1130 910  0    98   ~ 20
3.7V Li-Ion, Li-Polymer Battery Charger Curcuit with Load Sharing
$Comp
L GND #PWR05
U 1 1 59FDFACD
P 9270 5170
F 0 "#PWR05" H 9270 4920 50  0001 C CNN
F 1 "GND" H 9270 5020 50  0000 C CNN
F 2 "" H 9270 5170 50  0001 C CNN
F 3 "" H 9270 5170 50  0001 C CNN
	1    9270 5170
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 59FDFCE7
P 8850 5400
F 0 "C?" V 8760 5470 50  0000 L CNN
F 1 "2.2uF" V 8950 5450 50  0000 L CNN
F 2 "" H 8888 5250 50  0001 C CNN
F 3 "" H 8850 5400 50  0001 C CNN
	1    8850 5400
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 59FDFD8A
P 8370 5570
F 0 "C?" V 8260 5630 50  0000 L CNN
F 1 "2.2uF" V 8500 5630 50  0000 L CNN
F 2 "" H 8408 5420 50  0001 C CNN
F 3 "" H 8370 5570 50  0001 C CNN
	1    8370 5570
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 59FDFDCA
P 8700 6150
F 0 "C?" V 8630 6220 50  0000 L CNN
F 1 "2.2uF" V 8790 6220 50  0000 L CNN
F 2 "" H 8738 6000 50  0001 C CNN
F 3 "" H 8700 6150 50  0001 C CNN
	1    8700 6150
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 59FDFE0C
P 8700 5880
F 0 "C?" V 8630 5950 50  0000 L CNN
F 1 "2.2uF" V 8780 5950 50  0000 L CNN
F 2 "" H 8738 5730 50  0001 C CNN
F 3 "" H 8700 5880 50  0001 C CNN
	1    8700 5880
	0    1    1    0   
$EndComp
$Comp
L GND #PWR06
U 1 1 59FE0AAB
P 8180 6090
F 0 "#PWR06" H 8180 5840 50  0001 C CNN
F 1 "GND" H 8180 5940 50  0000 C CNN
F 2 "" H 8180 6090 50  0001 C CNN
F 3 "" H 8180 6090 50  0001 C CNN
	1    8180 6090
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 59FE1429
P 9120 4970
F 0 "#PWR07" H 9120 4820 50  0001 C CNN
F 1 "+3.3V" H 9120 5110 50  0000 C CNN
F 2 "" H 9120 4970 50  0001 C CNN
F 3 "" H 9120 4970 50  0001 C CNN
	1    9120 4970
	1    0    0    -1  
$EndComp
Text Notes 10460 5230 0    51   ~ 0
Controller:\nVDD:\nMode:\nAddress:
Text Notes 11050 5230 0    51   ~ 0
SH1106G\n2.8-3.3V\nI2C\n0x3D
$Comp
L GND #PWR08
U 1 1 59FE1E56
P 7940 6590
F 0 "#PWR08" H 7940 6340 50  0001 C CNN
F 1 "GND" H 7940 6440 50  0000 C CNN
F 2 "" H 7940 6590 50  0001 C CNN
F 3 "" H 7940 6590 50  0001 C CNN
	1    7940 6590
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59FE2450
P 7660 6560
F 0 "R?" V 7560 6560 50  0000 C CNN
F 1 "10K" V 7750 6560 50  0000 C CNN
F 2 "" V 7590 6560 50  0001 C CNN
F 3 "" H 7660 6560 50  0001 C CNN
	1    7660 6560
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 59FE2522
P 7660 6820
F 0 "#PWR09" H 7660 6570 50  0001 C CNN
F 1 "GND" H 7660 6670 50  0000 C CNN
F 2 "" H 7660 6820 50  0001 C CNN
F 3 "" H 7660 6820 50  0001 C CNN
	1    7660 6820
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR010
U 1 1 59FE25F2
P 7660 6240
F 0 "#PWR010" H 7660 6090 50  0001 C CNN
F 1 "+3.3V" H 7660 6380 50  0000 C CNN
F 2 "" H 7660 6240 50  0001 C CNN
F 3 "" H 7660 6240 50  0001 C CNN
	1    7660 6240
	1    0    0    -1  
$EndComp
Text Label 8750 6550 0    59   ~ 0
CS_3.3V
Text Label 8750 6650 0    59   ~ 0
RST_3.3V
Text Label 8750 6750 0    59   ~ 0
DC/SA_0_3.3V
$Comp
L R R?
U 1 1 59FE3261
P 8610 7850
F 0 "R?" V 8520 7850 50  0000 C CNN
F 1 "174K" V 8700 7850 50  0000 C CNN
F 2 "" V 8540 7850 50  0001 C CNN
F 3 "" H 8610 7850 50  0001 C CNN
	1    8610 7850
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 59FE32EF
P 8100 7990
F 0 "C?" V 8020 8070 50  0000 L CNN
F 1 "2.2uF" V 8160 8070 50  0000 L CNN
F 2 "" H 8138 7840 50  0001 C CNN
F 3 "" H 8100 7990 50  0001 C CNN
	1    8100 7990
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 59FE334E
P 8100 8230
F 0 "C?" V 8010 8310 50  0000 L CNN
F 1 "10uF" V 8180 8310 50  0000 L CNN
F 2 "" H 8138 8080 50  0001 C CNN
F 3 "" H 8100 8230 50  0001 C CNN
	1    8100 8230
	0    1    1    0   
$EndComp
$Comp
L GND #PWR011
U 1 1 59FE3A79
P 7620 8500
F 0 "#PWR011" H 7620 8250 50  0001 C CNN
F 1 "GND" H 7620 8350 50  0000 C CNN
F 2 "" H 7620 8500 50  0001 C CNN
F 3 "" H 7620 8500 50  0001 C CNN
	1    7620 8500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 59FE409A
P 9290 8480
F 0 "#PWR012" H 9290 8230 50  0001 C CNN
F 1 "GND" H 9290 8330 50  0000 C CNN
F 2 "" H 9290 8480 50  0001 C CNN
F 3 "" H 9290 8480 50  0001 C CNN
	1    9290 8480
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59FE800F
P 4090 2420
F 0 "R?" V 4000 2420 50  0000 C CNN
F 1 "R" V 4180 2420 50  0000 C CNN
F 2 "" V 4020 2420 50  0001 C CNN
F 3 "" H 4090 2420 50  0001 C CNN
	1    4090 2420
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59FE9BFF
P 4840 2110
F 0 "R?" V 4920 2110 50  0000 C CNN
F 1 "R" V 4840 2110 50  0000 C CNN
F 2 "" V 4770 2110 50  0001 C CNN
F 3 "" H 4840 2110 50  0001 C CNN
	1    4840 2110
	0    1    1    0   
$EndComp
Text Label 8310 2630 2    59   ~ 0
BAT_POS
Text Label 8310 2800 2    59   ~ 0
BAT_GND
$Comp
L TPS63031 J?
U 1 1 59FF0CBE
P 6540 1900
F 0 "J?" H 6350 2260 60  0000 C CNN
F 1 "TPS63031" H 6690 1460 60  0000 C CNN
F 2 "" H 6470 1610 60  0001 C CNN
F 3 "" H 6470 1610 60  0001 C CNN
	1    6540 1900
	1    0    0    -1  
$EndComp
$Comp
L L L?
U 1 1 59FF5879
P 6570 1330
F 0 "L?" V 6650 1330 50  0000 C CNN
F 1 "1.5uH" V 6510 1330 50  0000 C CNN
F 2 "" H 6570 1330 50  0001 C CNN
F 3 "" H 6570 1330 50  0001 C CNN
	1    6570 1330
	0    -1   -1   0   
$EndComp
$Comp
L C C?
U 1 1 59FF5ED7
P 5560 2390
F 0 "C?" H 5585 2490 50  0000 L CNN
F 1 "10uF" H 5585 2290 50  0000 L CNN
F 2 "" H 5598 2240 50  0001 C CNN
F 3 "" H 5560 2390 50  0001 C CNN
	1    5560 2390
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 59FF5F5A
P 5820 2390
F 0 "C?" H 5845 2490 50  0000 L CNN
F 1 "0.1uF" H 5845 2290 50  0000 L CNN
F 2 "" H 5858 2240 50  0001 C CNN
F 3 "" H 5820 2390 50  0001 C CNN
	1    5820 2390
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR013
U 1 1 59FF6FED
P 7870 1700
F 0 "#PWR013" H 7870 1550 50  0001 C CNN
F 1 "+3.3V" H 7870 1840 50  0000 C CNN
F 2 "" H 7870 1700 50  0001 C CNN
F 3 "" H 7870 1700 50  0001 C CNN
	1    7870 1700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 59FF71A2
P 7410 2120
F 0 "C?" H 7435 2220 50  0000 L CNN
F 1 "10uF" H 7435 2020 50  0000 L CNN
F 2 "" H 7448 1970 50  0001 C CNN
F 3 "" H 7410 2120 50  0001 C CNN
	1    7410 2120
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 59FF7318
P 7640 2120
F 0 "C?" H 7665 2220 50  0000 L CNN
F 1 "10uF" H 7665 2020 50  0000 L CNN
F 2 "" H 7678 1970 50  0001 C CNN
F 3 "" H 7640 2120 50  0001 C CNN
	1    7640 2120
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 59FF76C0
P 7410 2850
F 0 "#PWR014" H 7410 2600 50  0001 C CNN
F 1 "GND" H 7410 2700 50  0000 C CNN
F 2 "" H 7410 2850 50  0001 C CNN
F 3 "" H 7410 2850 50  0001 C CNN
	1    7410 2850
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59FF6B80
P 6320 7700
F 0 "R?" V 6230 7700 50  0000 C CNN
F 1 "1K" V 6400 7700 50  0000 C CNN
F 2 "" V 6250 7700 50  0001 C CNN
F 3 "" H 6320 7700 50  0001 C CNN
	1    6320 7700
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59FF6C05
P 6560 7700
F 0 "R?" V 6470 7700 50  0000 C CNN
F 1 "1K" V 6640 7700 50  0000 C CNN
F 2 "" V 6490 7700 50  0001 C CNN
F 3 "" H 6560 7700 50  0001 C CNN
	1    6560 7700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR015
U 1 1 59FF6F02
P 6440 7400
F 0 "#PWR015" H 6440 7250 50  0001 C CNN
F 1 "+3.3V" H 6440 7540 50  0000 C CNN
F 2 "" H 6440 7400 50  0001 C CNN
F 3 "" H 6440 7400 50  0001 C CNN
	1    6440 7400
	1    0    0    -1  
$EndComp
$Comp
L NTHD4P02F J?
U 1 1 5A0054BD
P 4830 1550
F 0 "J?" H 4650 1920 60  0000 C CNN
F 1 "NTHD4P02F" H 4850 1300 60  0000 C CNN
F 2 "" H 4890 1470 60  0001 C CNN
F 3 "" H 4890 1470 60  0001 C CNN
	1    4830 1550
	1    0    0    -1  
$EndComp
$Comp
L Crystal Y?
U 1 1 5A0083D0
P 2840 6070
F 0 "Y?" V 3060 6070 50  0000 C CNN
F 1 "8MHz" V 2830 5830 50  0000 C CNN
F 2 "" H 2840 6070 50  0001 C CNN
F 3 "" H 2840 6070 50  0001 C CNN
	1    2840 6070
	0    -1   -1   0   
$EndComp
$Comp
L C C?
U 1 1 5A008587
P 2310 5920
F 0 "C?" V 2230 5990 50  0000 L CNN
F 1 "22pF" V 2380 5990 50  0000 L CNN
F 2 "" H 2348 5770 50  0001 C CNN
F 3 "" H 2310 5920 50  0001 C CNN
	1    2310 5920
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 5A00862C
P 2310 6220
F 0 "C?" V 2220 6290 50  0000 L CNN
F 1 "22pF" V 2380 6290 50  0000 L CNN
F 2 "" H 2348 6070 50  0001 C CNN
F 3 "" H 2310 6220 50  0001 C CNN
	1    2310 6220
	0    1    1    0   
$EndComp
$Comp
L GND #PWR016
U 1 1 5A008D3B
P 4360 9260
F 0 "#PWR016" H 4360 9010 50  0001 C CNN
F 1 "GND" H 4360 9110 50  0000 C CNN
F 2 "" H 4360 9260 50  0001 C CNN
F 3 "" H 4360 9260 50  0001 C CNN
	1    4360 9260
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR017
U 1 1 5A0091D9
P 5220 4450
F 0 "#PWR017" H 5220 4300 50  0001 C CNN
F 1 "+3.3V" H 5220 4590 50  0000 C CNN
F 2 "" H 5220 4450 50  0001 C CNN
F 3 "" H 5220 4450 50  0001 C CNN
	1    5220 4450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 5A0094D2
P 1980 6550
F 0 "#PWR018" H 1980 6300 50  0001 C CNN
F 1 "GND" H 1980 6400 50  0000 C CNN
F 2 "" H 1980 6550 50  0001 C CNN
F 3 "" H 1980 6550 50  0001 C CNN
	1    1980 6550
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A00C129
P 1980 5190
F 0 "R?" V 1880 5190 50  0000 C CNN
F 1 "10K" V 2080 5190 50  0000 C CNN
F 2 "" V 1910 5190 50  0001 C CNN
F 3 "" H 1980 5190 50  0001 C CNN
	1    1980 5190
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR019
U 1 1 5A00C2C0
P 1980 4980
F 0 "#PWR019" H 1980 4830 50  0001 C CNN
F 1 "+3.3V" H 1980 5120 50  0000 C CNN
F 2 "" H 1980 4980 50  0001 C CNN
F 3 "" H 1980 4980 50  0001 C CNN
	1    1980 4980
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5A00C900
P 4900 4720
F 0 "C?" H 4925 4820 50  0000 L CNN
F 1 "100nF" H 4925 4620 50  0000 L CNN
F 2 "" H 4938 4570 50  0001 C CNN
F 3 "" H 4900 4720 50  0001 C CNN
	1    4900 4720
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 5A00C98A
P 5220 4720
F 0 "C?" H 5245 4820 50  0000 L CNN
F 1 "10uF" H 5245 4620 50  0000 L CNN
F 2 "" H 5258 4570 50  0001 C CNN
F 3 "" H 5220 4720 50  0001 C CNN
	1    5220 4720
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 5A00CB3F
P 5220 5010
F 0 "#PWR020" H 5220 4760 50  0001 C CNN
F 1 "GND" H 5220 4860 50  0000 C CNN
F 2 "" H 5220 5010 50  0001 C CNN
F 3 "" H 5220 5010 50  0001 C CNN
	1    5220 5010
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x03_Odd_Even J?
U 1 1 5A00DAAC
P 4610 10280
F 0 "J?" H 4660 10480 50  0000 C CNN
F 1 "AVR ISP" H 4660 10080 50  0000 C CNN
F 2 "" H 4610 10280 50  0001 C CNN
F 3 "" H 4610 10280 50  0001 C CNN
	1    4610 10280
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR021
U 1 1 5A00E067
P 5030 9920
F 0 "#PWR021" H 5030 9770 50  0001 C CNN
F 1 "+3.3V" H 5030 10060 50  0000 C CNN
F 2 "" H 5030 9920 50  0001 C CNN
F 3 "" H 5030 9920 50  0001 C CNN
	1    5030 9920
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 5A00E2F0
P 5030 10460
F 0 "#PWR022" H 5030 10210 50  0001 C CNN
F 1 "GND" H 5030 10310 50  0000 C CNN
F 2 "" H 5030 10460 50  0001 C CNN
F 3 "" H 5030 10460 50  0001 C CNN
	1    5030 10460
	1    0    0    -1  
$EndComp
Text Label 3760 10280 0    60   ~ 0
ISP_SCK
Text Label 3760 10380 0    60   ~ 0
ISP_RST
Text Label 2840 5470 0    60   ~ 0
ISP_RST
$Comp
L R R?
U 1 1 5A00F636
P 3020 8170
F 0 "R?" V 2940 8170 50  0000 C CNN
F 1 "1K" V 3020 8170 50  0000 C CNN
F 2 "" V 2950 8170 50  0001 C CNN
F 3 "" H 3020 8170 50  0001 C CNN
	1    3020 8170
	0    1    1    0   
$EndComp
Text Label 6600 6470 2    60   ~ 0
ISP_SCK
Text Label 8750 7050 0    60   ~ 0
OLED_SCL
Text Label 8750 7150 0    60   ~ 0
OLED_SDA
Text Label 7040 8170 2    60   ~ 0
OLED_SCL
Text Label 7040 8270 2    60   ~ 0
OLED_SDA
$Comp
L C C?
U 1 1 5A01213D
P 4590 4720
F 0 "C?" H 4615 4820 50  0000 L CNN
F 1 "4.7nF" H 4615 4620 50  0000 L CNN
F 2 "" H 4628 4570 50  0001 C CNN
F 3 "" H 4590 4720 50  0001 C CNN
	1    4590 4720
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A0129A2
P 7850 9680
F 0 "R?" V 7760 9680 50  0000 C CNN
F 1 "27K" V 7940 9680 50  0000 C CNN
F 2 "" V 7780 9680 50  0001 C CNN
F 3 "" H 7850 9680 50  0001 C CNN
	1    7850 9680
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A012A39
P 8200 9970
F 0 "R?" V 8110 9970 50  0000 C CNN
F 1 "100K" V 8290 9970 50  0000 C CNN
F 2 "" V 8130 9970 50  0001 C CNN
F 3 "" H 8200 9970 50  0001 C CNN
	1    8200 9970
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 5A012B2D
P 8510 10260
F 0 "C?" H 8535 10360 50  0000 L CNN
F 1 "4.7nF" H 8535 10160 50  0000 L CNN
F 2 "" H 8548 10110 50  0001 C CNN
F 3 "" H 8510 10260 50  0001 C CNN
	1    8510 10260
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR023
U 1 1 5A012C23
P 7850 9330
F 0 "#PWR023" H 7850 9180 50  0001 C CNN
F 1 "+3.3V" H 7850 9470 50  0000 C CNN
F 2 "" H 7850 9330 50  0001 C CNN
F 3 "" H 7850 9330 50  0001 C CNN
	1    7850 9330
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 5A012CB5
P 7850 10620
F 0 "#PWR024" H 7850 10370 50  0001 C CNN
F 1 "GND" H 7850 10470 50  0000 C CNN
F 2 "" H 7850 10620 50  0001 C CNN
F 3 "" H 7850 10620 50  0001 C CNN
	1    7850 10620
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW?
U 1 1 5A012D9B
P 7850 10290
F 0 "SW?" H 7770 10210 50  0000 L CNN
F 1 "Up" H 7850 10480 50  0000 C CNN
F 2 "" H 7850 10490 50  0001 C CNN
F 3 "" H 7850 10490 50  0001 C CNN
	1    7850 10290
	0    1    1    0   
$EndComp
Text Label 8830 9970 2    60   ~ 0
BTN1
$Comp
L R R?
U 1 1 5A015157
P 8990 9680
F 0 "R?" V 8900 9680 50  0000 C CNN
F 1 "27K" V 9080 9680 50  0000 C CNN
F 2 "" V 8920 9680 50  0001 C CNN
F 3 "" H 8990 9680 50  0001 C CNN
	1    8990 9680
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A01515D
P 9340 9970
F 0 "R?" V 9250 9970 50  0000 C CNN
F 1 "100K" V 9430 9970 50  0000 C CNN
F 2 "" V 9270 9970 50  0001 C CNN
F 3 "" H 9340 9970 50  0001 C CNN
	1    9340 9970
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 5A015163
P 9650 10260
F 0 "C?" H 9675 10360 50  0000 L CNN
F 1 "4.7nF" H 9675 10160 50  0000 L CNN
F 2 "" H 9688 10110 50  0001 C CNN
F 3 "" H 9650 10260 50  0001 C CNN
	1    9650 10260
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR025
U 1 1 5A015169
P 8990 9330
F 0 "#PWR025" H 8990 9180 50  0001 C CNN
F 1 "+3.3V" H 8990 9470 50  0000 C CNN
F 2 "" H 8990 9330 50  0001 C CNN
F 3 "" H 8990 9330 50  0001 C CNN
	1    8990 9330
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 5A01516F
P 8990 10620
F 0 "#PWR026" H 8990 10370 50  0001 C CNN
F 1 "GND" H 8990 10470 50  0000 C CNN
F 2 "" H 8990 10620 50  0001 C CNN
F 3 "" H 8990 10620 50  0001 C CNN
	1    8990 10620
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW?
U 1 1 5A015175
P 8990 10290
F 0 "SW?" H 8910 10210 50  0000 L CNN
F 1 "Select" H 8990 10480 50  0000 C CNN
F 2 "" H 8990 10490 50  0001 C CNN
F 3 "" H 8990 10490 50  0001 C CNN
	1    8990 10290
	0    1    1    0   
$EndComp
Text Label 9970 9970 2    60   ~ 0
BTN2
$Comp
L R R?
U 1 1 5A015271
P 10150 9680
F 0 "R?" V 10060 9680 50  0000 C CNN
F 1 "27K" V 10240 9680 50  0000 C CNN
F 2 "" V 10080 9680 50  0001 C CNN
F 3 "" H 10150 9680 50  0001 C CNN
	1    10150 9680
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A015277
P 10500 9970
F 0 "R?" V 10410 9970 50  0000 C CNN
F 1 "100K" V 10590 9970 50  0000 C CNN
F 2 "" V 10430 9970 50  0001 C CNN
F 3 "" H 10500 9970 50  0001 C CNN
	1    10500 9970
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 5A01527D
P 10810 10260
F 0 "C?" H 10835 10360 50  0000 L CNN
F 1 "4.7nF" H 10835 10160 50  0000 L CNN
F 2 "" H 10848 10110 50  0001 C CNN
F 3 "" H 10810 10260 50  0001 C CNN
	1    10810 10260
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR027
U 1 1 5A015283
P 10150 9330
F 0 "#PWR027" H 10150 9180 50  0001 C CNN
F 1 "+3.3V" H 10150 9470 50  0000 C CNN
F 2 "" H 10150 9330 50  0001 C CNN
F 3 "" H 10150 9330 50  0001 C CNN
	1    10150 9330
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR028
U 1 1 5A015289
P 10150 10620
F 0 "#PWR028" H 10150 10370 50  0001 C CNN
F 1 "GND" H 10150 10470 50  0000 C CNN
F 2 "" H 10150 10620 50  0001 C CNN
F 3 "" H 10150 10620 50  0001 C CNN
	1    10150 10620
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW?
U 1 1 5A01528F
P 10150 10290
F 0 "SW?" H 10070 10210 50  0000 L CNN
F 1 "Down" H 10150 10470 50  0000 C CNN
F 2 "" H 10150 10490 50  0001 C CNN
F 3 "" H 10150 10490 50  0001 C CNN
	1    10150 10290
	0    1    1    0   
$EndComp
Text Label 11130 9970 2    60   ~ 0
BTN3
Text Label 6600 5470 2    60   ~ 0
BTN1
Text Label 6600 5570 2    60   ~ 0
BTN2
Text Label 6600 5670 2    60   ~ 0
BTN3
$Comp
L SW_Push SW?
U 1 1 5A0086A3
P 1980 5640
F 0 "SW?" H 1920 5530 50  0000 L CNN
F 1 "Reset" H 1980 5840 50  0000 C CNN
F 2 "" H 1980 5840 50  0001 C CNN
F 3 "" H 1980 5840 50  0001 C CNN
	1    1980 5640
	0    1    1    0   
$EndComp
Text Label 2210 8170 0    60   ~ 0
ISP_PDI_MOSI
Text Label 2210 8270 0    60   ~ 0
ISP_PDO_MISO
Text Label 5540 10280 2    60   ~ 0
ISP_PDI_MOSI
Text Label 3760 10180 0    60   ~ 0
ISP_PDO_MISO
$Comp
L LED D?
U 1 1 5A00EF5D
P 2610 8570
F 0 "D?" H 2610 8450 50  0000 C CNN
F 1 "green" H 2610 8660 50  0000 C CNN
F 2 "" H 2610 8570 50  0001 C CNN
F 3 "" H 2610 8570 50  0001 C CNN
	1    2610 8570
	-1   0    0    1   
$EndComp
$Comp
L LED D?
U 1 1 5A00F699
P 2610 8880
F 0 "D?" H 2610 8760 50  0000 C CNN
F 1 "yellow" H 2610 8980 50  0000 C CNN
F 2 "" H 2610 8880 50  0001 C CNN
F 3 "" H 2610 8880 50  0001 C CNN
	1    2610 8880
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 5A00FDA2
P 2180 8570
F 0 "R?" V 2100 8570 50  0000 C CNN
F 1 "1K" V 2270 8570 50  0000 C CNN
F 2 "" V 2110 8570 50  0001 C CNN
F 3 "" H 2180 8570 50  0001 C CNN
	1    2180 8570
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 5A00FE5E
P 2180 8880
F 0 "R?" V 2100 8880 50  0000 C CNN
F 1 "1K" V 2270 8880 50  0000 C CNN
F 2 "" V 2110 8880 50  0001 C CNN
F 3 "" H 2180 8880 50  0001 C CNN
	1    2180 8880
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR029
U 1 1 5A010B6E
P 1840 8460
F 0 "#PWR029" H 1840 8310 50  0001 C CNN
F 1 "+3.3V" H 1840 8600 50  0000 C CNN
F 2 "" H 1840 8460 50  0001 C CNN
F 3 "" H 1840 8460 50  0001 C CNN
	1    1840 8460
	1    0    0    -1  
$EndComp
$Comp
L SH1106G U?
U 1 1 5A01BB17
P 10520 6700
F 0 "U?" H 9730 8200 60  0000 C CNN
F 1 "SH1106G" H 11200 4990 60  0000 C CNN
F 2 "" H 10820 7000 60  0001 C CNN
F 3 "" H 10820 7000 60  0001 C CNN
	1    10520 6700
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP?
U 1 1 5A01DA25
P 9310 7250
F 0 "JP?" H 9270 7200 50  0000 C CNN
F 1 "D2D1" H 9310 7320 50  0000 C CNN
F 2 "" H 9310 7250 50  0001 C CNN
F 3 "" H 9310 7250 50  0001 C CNN
	1    9310 7250
	-1   0    0    1   
$EndComp
Wire Wire Line
	1370 1770 2370 1770
Wire Wire Line
	1720 2420 1720 2720
Wire Wire Line
	2070 1920 2070 1770
Connection ~ 2070 1770
Wire Wire Line
	2070 2370 2070 2220
Wire Wire Line
	2070 2670 2070 2920
Wire Wire Line
	1070 2370 1070 2720
Wire Wire Line
	9470 5350 9400 5350
Wire Wire Line
	9400 5350 9400 5060
Wire Wire Line
	8630 5550 9470 5550
Wire Wire Line
	9470 5450 9060 5450
Wire Wire Line
	9060 5450 9060 5400
Wire Wire Line
	9060 5400 9000 5400
Wire Wire Line
	8630 5550 8630 5400
Wire Wire Line
	8630 5400 8700 5400
Wire Wire Line
	9470 5650 8560 5650
Wire Wire Line
	8560 5650 8560 5570
Wire Wire Line
	8560 5570 8520 5570
Wire Wire Line
	9470 5750 8180 5750
Wire Wire Line
	8180 5750 8180 5570
Wire Wire Line
	8180 5570 8220 5570
Wire Wire Line
	8910 5850 9470 5850
Wire Wire Line
	8910 5850 8910 5880
Wire Wire Line
	8910 5880 8850 5880
Wire Wire Line
	8850 6150 9470 6150
Wire Wire Line
	9470 6050 8910 6050
Wire Wire Line
	8910 6050 8910 6020
Wire Wire Line
	8910 6020 8180 6020
Wire Wire Line
	8390 6150 8550 6150
Wire Wire Line
	8390 5880 8390 6150
Wire Wire Line
	8550 5880 8390 5880
Connection ~ 8390 6020
Wire Wire Line
	8180 6020 8180 6090
Wire Wire Line
	9270 5170 9270 5060
Wire Wire Line
	9270 5060 9400 5060
Wire Wire Line
	9120 4970 9120 6150
Connection ~ 9120 5850
Connection ~ 9120 6150
Wire Wire Line
	9470 6250 9000 6250
Wire Wire Line
	7940 6450 9470 6450
Wire Wire Line
	7660 6350 9470 6350
Wire Wire Line
	9000 6250 9000 6450
Wire Wire Line
	7940 6450 7940 6590
Connection ~ 9000 6450
Wire Wire Line
	7660 6240 7660 6410
Wire Wire Line
	7660 6820 7660 6710
Connection ~ 7660 6350
Wire Wire Line
	8750 7150 9470 7150
Wire Wire Line
	9470 7850 8760 7850
Wire Wire Line
	9470 7950 8780 7950
Wire Wire Line
	8780 7950 8780 7990
Wire Wire Line
	8780 7990 8250 7990
Wire Wire Line
	9470 8050 8850 8050
Wire Wire Line
	8850 8050 8850 8230
Wire Wire Line
	8850 8230 8250 8230
Wire Wire Line
	8460 7850 7620 7850
Wire Wire Line
	9470 8150 8940 8150
Wire Wire Line
	8940 8150 8940 8390
Wire Wire Line
	8940 8390 7620 8390
Wire Wire Line
	7620 7850 7620 8500
Connection ~ 7620 8390
Wire Wire Line
	7950 7990 7620 7990
Connection ~ 7620 7990
Wire Wire Line
	7950 8230 7620 8230
Connection ~ 7620 8230
Wire Wire Line
	9470 8250 9290 8250
Wire Wire Line
	9290 8250 9290 8480
Wire Wire Line
	970  2370 970  2510
Wire Wire Line
	970  2510 1070 2510
Connection ~ 1070 2510
Wire Wire Line
	2070 3220 2070 3400
Wire Wire Line
	2070 3700 2070 3860
Wire Wire Line
	2370 2070 2370 2800
Wire Wire Line
	2370 2800 2070 2800
Connection ~ 2070 2800
Wire Wire Line
	3270 2070 3270 2800
Connection ~ 1720 1770
Wire Wire Line
	3270 1920 3430 1920
Wire Wire Line
	3430 1920 3430 2270
Wire Wire Line
	3430 2570 3430 2870
Connection ~ 3430 2800
Wire Wire Line
	3730 2800 3730 2570
Connection ~ 3730 2800
Wire Wire Line
	4090 2800 4090 2570
Connection ~ 4090 2800
Wire Wire Line
	1720 1310 1720 2120
Wire Wire Line
	4090 2270 4090 1310
Wire Wire Line
	3270 2800 6090 2800
Wire Wire Line
	6420 1330 6040 1330
Wire Wire Line
	6040 1330 6040 1660
Wire Wire Line
	6040 1660 6090 1660
Wire Wire Line
	6720 1330 7160 1330
Wire Wire Line
	7160 1330 7160 1670
Wire Wire Line
	7160 1670 7110 1670
Wire Wire Line
	6090 2800 6090 2210
Wire Wire Line
	5560 2540 5560 2800
Connection ~ 5560 2800
Wire Wire Line
	5820 2540 5820 2800
Connection ~ 5820 2800
Wire Wire Line
	6090 1880 5820 1880
Wire Wire Line
	5820 1880 5820 2240
Wire Wire Line
	6090 1990 5820 1990
Connection ~ 5820 1990
Wire Wire Line
	6090 2100 5820 2100
Connection ~ 5820 2100
Wire Wire Line
	7110 1780 7870 1780
Wire Wire Line
	7870 1780 7870 1700
Wire Wire Line
	7110 1990 7230 1990
Wire Wire Line
	7230 1990 7230 1780
Connection ~ 7230 1780
Wire Wire Line
	7410 1970 7410 1780
Connection ~ 7410 1780
Wire Wire Line
	7640 1970 7640 1780
Connection ~ 7640 1780
Wire Wire Line
	7410 2270 7410 2850
Wire Wire Line
	7640 2270 7640 2800
Wire Wire Line
	7410 2800 8310 2800
Connection ~ 7410 2800
Connection ~ 7640 2800
Wire Wire Line
	5560 1770 6090 1770
Wire Wire Line
	5910 8270 7040 8270
Wire Wire Line
	6320 7550 6320 7460
Wire Wire Line
	6320 7460 6560 7460
Wire Wire Line
	6440 7460 6440 7400
Wire Wire Line
	6560 7460 6560 7550
Connection ~ 6440 7460
Wire Wire Line
	3730 1770 3730 2270
Wire Wire Line
	1720 1310 4390 1310
Connection ~ 4090 1310
Wire Wire Line
	4390 1430 4330 1430
Wire Wire Line
	4330 1310 4330 1670
Connection ~ 4330 1310
Wire Wire Line
	4330 1670 4390 1670
Connection ~ 4330 1430
Wire Wire Line
	4390 1550 4370 1550
Wire Wire Line
	4370 1550 4370 2110
Wire Wire Line
	3270 1770 4290 1770
Wire Wire Line
	4290 1770 4290 1950
Wire Wire Line
	4290 1950 5370 1950
Wire Wire Line
	5370 1550 5370 2630
Wire Wire Line
	5370 1550 5290 1550
Wire Wire Line
	5290 1670 5370 1670
Connection ~ 5370 1670
Connection ~ 3730 1770
Connection ~ 5370 1950
Wire Wire Line
	5370 2630 8310 2630
Wire Wire Line
	4370 2110 4690 2110
Wire Wire Line
	4990 2110 5560 2110
Connection ~ 5560 2110
Wire Wire Line
	5560 1770 5560 2240
Wire Wire Line
	5290 1430 5430 1430
Wire Wire Line
	5430 1310 5430 2020
Wire Wire Line
	5430 2020 4570 2020
Wire Wire Line
	4570 2020 4570 2110
Connection ~ 4570 2110
Wire Wire Line
	5290 1310 5430 1310
Connection ~ 5430 1430
Wire Wire Line
	2460 5920 3310 5920
Connection ~ 2840 5920
Wire Wire Line
	2460 6220 3310 6220
Connection ~ 2840 6220
Wire Wire Line
	4360 9170 4360 9260
Wire Wire Line
	4460 9170 4460 9220
Wire Wire Line
	4360 9220 4760 9220
Connection ~ 4360 9220
Wire Wire Line
	1980 5840 1980 6550
Wire Wire Line
	1980 6220 2160 6220
Wire Wire Line
	2160 5920 1980 5920
Wire Wire Line
	4760 9220 4760 9170
Connection ~ 4460 9220
Wire Wire Line
	1980 6470 3310 6470
Connection ~ 1980 6220
Connection ~ 1980 6470
Wire Wire Line
	1980 4980 1980 5040
Wire Wire Line
	5220 4450 5220 4570
Wire Wire Line
	4900 4510 4900 4570
Wire Wire Line
	4360 4510 5220 4510
Connection ~ 5220 4510
Wire Wire Line
	4360 4510 4360 5170
Connection ~ 4900 4510
Wire Wire Line
	4460 5170 4460 5070
Wire Wire Line
	4360 5070 4760 5070
Connection ~ 4360 5070
Wire Wire Line
	4760 5070 4760 5170
Connection ~ 4460 5070
Wire Wire Line
	5220 4870 5220 5010
Wire Wire Line
	4900 4870 4900 4940
Wire Wire Line
	4590 4940 5220 4940
Connection ~ 5220 4940
Wire Wire Line
	4910 10280 5540 10280
Wire Wire Line
	4910 10380 5030 10380
Wire Wire Line
	5030 10380 5030 10460
Wire Wire Line
	4910 10180 5030 10180
Wire Wire Line
	5030 10180 5030 9920
Wire Wire Line
	3760 10280 4410 10280
Wire Wire Line
	3760 10380 4410 10380
Wire Wire Line
	5910 6470 6600 6470
Wire Wire Line
	9470 7050 8750 7050
Wire Wire Line
	5910 8170 7040 8170
Wire Wire Line
	4590 4870 4590 4940
Connection ~ 4900 4940
Wire Wire Line
	4590 4570 4590 4510
Connection ~ 4590 4510
Wire Wire Line
	7850 9830 7850 10090
Wire Wire Line
	7850 9530 7850 9330
Wire Wire Line
	8050 9970 7850 9970
Connection ~ 7850 9970
Wire Wire Line
	8350 9970 8830 9970
Wire Wire Line
	8510 9970 8510 10110
Wire Wire Line
	7850 10490 7850 10620
Wire Wire Line
	8510 10410 8510 10570
Wire Wire Line
	8510 10570 7850 10570
Connection ~ 7850 10570
Connection ~ 8510 9970
Wire Wire Line
	8990 9830 8990 10090
Wire Wire Line
	8990 9530 8990 9330
Wire Wire Line
	9190 9970 8990 9970
Connection ~ 8990 9970
Wire Wire Line
	9490 9970 9970 9970
Wire Wire Line
	9650 9970 9650 10110
Wire Wire Line
	8990 10490 8990 10620
Wire Wire Line
	9650 10410 9650 10570
Wire Wire Line
	9650 10570 8990 10570
Connection ~ 8990 10570
Connection ~ 9650 9970
Wire Wire Line
	10150 9830 10150 10090
Wire Wire Line
	10150 9530 10150 9330
Wire Wire Line
	10350 9970 10150 9970
Connection ~ 10150 9970
Wire Wire Line
	10650 9970 11130 9970
Wire Wire Line
	10810 9970 10810 10110
Wire Wire Line
	10150 10490 10150 10620
Wire Wire Line
	10810 10410 10810 10570
Wire Wire Line
	10810 10570 10150 10570
Connection ~ 10150 10570
Connection ~ 10810 9970
Wire Wire Line
	5910 5470 6600 5470
Wire Wire Line
	5910 5570 6600 5570
Wire Wire Line
	5910 5670 6600 5670
Connection ~ 1980 5920
Wire Wire Line
	1980 5340 1980 5440
Wire Wire Line
	2380 5470 3310 5470
Wire Wire Line
	2380 5470 2380 5390
Wire Wire Line
	2380 5390 1980 5390
Connection ~ 1980 5390
Wire Wire Line
	3170 8170 3310 8170
Wire Wire Line
	2210 8270 3310 8270
Wire Wire Line
	2210 8170 2870 8170
Wire Wire Line
	3760 10180 4410 10180
Wire Wire Line
	6320 7850 6320 8170
Connection ~ 6320 8170
Wire Wire Line
	6560 7850 6560 8270
Connection ~ 6560 8270
Wire Wire Line
	2760 8570 3310 8570
Wire Wire Line
	3310 8670 2880 8670
Wire Wire Line
	2880 8670 2880 8880
Wire Wire Line
	2880 8880 2760 8880
Wire Wire Line
	2330 8570 2460 8570
Wire Wire Line
	2460 8880 2330 8880
Wire Wire Line
	1840 8460 1840 8880
Wire Wire Line
	1840 8570 2030 8570
Wire Wire Line
	1840 8880 2030 8880
Connection ~ 1840 8570
Wire Wire Line
	8600 6750 9470 6750
Wire Wire Line
	8600 6650 9470 6650
Wire Wire Line
	8600 6550 9470 6550
Wire Wire Line
	8600 6350 8600 6750
Connection ~ 8600 6350
Connection ~ 8600 6550
Connection ~ 8600 6650
Wire Wire Line
	9410 7250 9470 7250
Wire Wire Line
	9210 7250 9180 7250
Wire Wire Line
	9180 7250 9180 7150
Connection ~ 9180 7150
$EndSCHEMATC
