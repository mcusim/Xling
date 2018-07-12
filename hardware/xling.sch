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
Title "Xling Schematic"
Date "2018-07-12"
Rev "0.1.22"
Comp "MCUSim Laboratory"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCP73831 J3
U 1 1 59FC873F
P 2780 6660
F 0 "J3" H 2580 7050 60  0000 C CNN
F 1 "MCP73831" H 2760 6960 60  0000 C CNN
F 2 "Xling Footprints:MCP73831_SOT_23" H 2780 6660 60  0001 C CNN
F 3 "" H 2780 6660 60  0001 C CNN
F 4 "MCP73831T-2AT" H 2780 6660 60  0001 C CNN "Model"
	1    2780 6660
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 59FC8778
P 3390 7160
F 0 "R3" V 3290 7160 50  0000 C CNN
F 1 "2K7" V 3490 7160 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 3320 7160 50  0001 C CNN
F 3 "" H 3390 7160 50  0001 C CNN
F 4 "1206S4J0272T5E" V 3390 7160 60  0001 C CNN "Model"
	1    3390 7160
	1    0    0    -1  
$EndComp
$Comp
L USB_OTG J4
U 1 1 59FC88F3
P 1030 6710
F 0 "J4" H 830 7160 50  0000 L CNN
F 1 "USB_OTG" H 830 7060 50  0000 L CNN
F 2 "Connectors:USB_Mini-B" H 1180 6660 50  0001 C CNN
F 3 "" H 1180 6660 50  0001 C CNN
F 4 "675031020" H 1030 6710 60  0001 C CNN "Model"
	1    1030 6710
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 59FC89D1
P 3390 7540
F 0 "#PWR01" H 3390 7290 50  0001 C CNN
F 1 "GND" H 3390 7390 50  0000 C CNN
F 2 "" H 3390 7540 50  0001 C CNN
F 3 "" H 3390 7540 50  0001 C CNN
	1    3390 7540
	1    0    0    -1  
$EndComp
Text Notes 5080 5210 0    98   ~ 20
3.3V power supply and battery charger
Text Notes 6620 1670 0    51   ~ 0
Controller:\nVDD:\nVCC:\nMode:\nAddress:
Text Notes 7070 1670 0    51   ~ 0
SSD1309ZC\n2.8-3.3V\n12V\nI2C\n0x7A
$Comp
L R R4
U 1 1 59FE800F
P 4760 6820
F 0 "R4" V 4670 6820 50  0000 C CNN
F 1 "100K" V 4850 6820 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 4690 6820 50  0001 C CNN
F 3 "" H 4760 6820 50  0001 C CNN
F 4 "CRCW1206100KFKTABC" V 4760 6820 60  0001 C CNN "Model"
	1    4760 6820
	1    0    0    -1  
$EndComp
$Comp
L L L1
U 1 1 59FF5879
P 6400 5980
F 0 "L1" V 6480 5980 50  0000 C CNN
F 1 "4.7uH" V 6340 5980 50  0000 C CNN
F 2 "Inductors_SMD:L_1210" H 6400 5980 50  0001 C CNN
F 3 "" H 6400 5980 50  0001 C CNN
F 4 "NLV10MTC4R7" V 6400 5980 60  0001 C CNN "Model"
	1    6400 5980
	0    -1   -1   0   
$EndComp
$Comp
L C C5
U 1 1 59FF5F5A
P 5530 6890
F 0 "C5" H 5555 6990 50  0000 L CNN
F 1 "4.7uF" H 5555 6790 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 5568 6740 50  0001 C CNN
F 3 "" H 5530 6890 50  0001 C CNN
F 4 "CL31B475KAHNNNE" H 5530 6890 60  0001 C CNN "Model"
	1    5530 6890
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 59FF6FED
P 7250 6220
F 0 "#PWR02" H 7250 6070 50  0001 C CNN
F 1 "+3.3V" H 7250 6360 50  0000 C CNN
F 2 "" H 7250 6220 50  0001 C CNN
F 3 "" H 7250 6220 50  0001 C CNN
	1    7250 6220
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 59FF71A2
P 7590 6720
F 0 "C1" H 7470 6830 50  0000 L CNN
F 1 "10uF" H 7400 6620 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7628 6570 50  0001 C CNN
F 3 "" H 7590 6720 50  0001 C CNN
F 4 "AVX 08056D106KAT2A" H 7590 6720 60  0001 C CNN "Model"
	1    7590 6720
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 59FF6B80
P 4050 3460
F 0 "R8" V 3960 3460 50  0000 C CNN
F 1 "1K" V 4130 3460 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3980 3460 50  0001 C CNN
F 3 "" H 4050 3460 50  0001 C CNN
F 4 "CRCW06031K00JNTABC" V 4050 3460 60  0001 C CNN "Model"
	1    4050 3460
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 59FF6C05
P 4290 3460
F 0 "R9" V 4200 3460 50  0000 C CNN
F 1 "1K" V 4370 3460 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4220 3460 50  0001 C CNN
F 3 "" H 4290 3460 50  0001 C CNN
F 4 "CRCW06031K00JNTABC" V 4290 3460 60  0001 C CNN "Model"
	1    4290 3460
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 5A008587
P 4370 2670
F 0 "C13" V 4290 2740 50  0000 L CNN
F 1 "22pF" V 4440 2740 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4408 2520 50  0001 C CNN
F 3 "" H 4370 2670 50  0001 C CNN
F 4 "CL21C220JBANNNC" V 4370 2670 60  0001 C CNN "Model"
	1    4370 2670
	0    -1   1    0   
$EndComp
$Comp
L C C15
U 1 1 5A00862C
P 4370 2970
F 0 "C15" V 4280 3040 50  0000 L CNN
F 1 "22pF" V 4440 3040 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4408 2820 50  0001 C CNN
F 3 "" H 4370 2970 50  0001 C CNN
F 4 "CL21C220JBANNNC" V 4370 2970 60  0001 C CNN "Model"
	1    4370 2970
	0    -1   1    0   
$EndComp
$Comp
L +3.3V #PWR03
U 1 1 5A0091D9
P 1810 1340
F 0 "#PWR03" H 1810 1190 50  0001 C CNN
F 1 "+3.3V" H 1810 1480 50  0000 C CNN
F 2 "" H 1810 1340 50  0001 C CNN
F 3 "" H 1810 1340 50  0001 C CNN
	1    1810 1340
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5A00C900
P 1810 1610
F 0 "C8" H 1835 1710 50  0000 L CNN
F 1 "100nF" H 1835 1510 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1848 1460 50  0001 C CNN
F 3 "" H 1810 1610 50  0001 C CNN
F 4 "CL21B104JBCNNNC" H 1810 1610 60  0001 C CNN "Model"
	1    1810 1610
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5A00CB3F
P 1810 1900
F 0 "#PWR04" H 1810 1650 50  0001 C CNN
F 1 "GND" H 1810 1750 50  0000 C CNN
F 2 "" H 1810 1900 50  0001 C CNN
F 3 "" H 1810 1900 50  0001 C CNN
	1    1810 1900
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x03_Odd_Even J5
U 1 1 5A00DAAC
P 3250 1610
F 0 "J5" H 3300 1810 50  0000 C CNN
F 1 "AVR ISP" H 3300 1410 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_2x03_Pitch2.54mm" H 3250 1610 50  0001 C CNN
F 3 "" H 3250 1610 50  0001 C CNN
F 4 "empty" H 3250 1610 60  0001 C CNN "Model"
	1    3250 1610
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 5A00E067
P 3580 1930
F 0 "#PWR05" H 3580 1780 50  0001 C CNN
F 1 "+3.3V" H 3580 2070 50  0000 C CNN
F 2 "" H 3580 1930 50  0001 C CNN
F 3 "" H 3580 1930 50  0001 C CNN
	1    3580 1930
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 5A00E2F0
P 3580 1330
F 0 "#PWR06" H 3580 1080 50  0001 C CNN
F 1 "GND" H 3580 1180 50  0000 C CNN
F 2 "" H 3580 1330 50  0001 C CNN
F 3 "" H 3580 1330 50  0001 C CNN
	1    3580 1330
	-1   0    0    1   
$EndComp
Text Label 2400 1610 0    60   ~ 0
ISP_SCK
Text Label 2400 1510 0    60   ~ 0
ISP_RST
Text Label 4810 3870 2    60   ~ 0
ISP_RST
Text Label 3720 2770 2    60   ~ 0
ISP_SCK
Text Label 5370 2820 0    60   ~ 0
OLED_SCL
Text Label 5370 2910 0    60   ~ 0
OLED_SDA
Text Label 4810 3770 2    60   ~ 0
OLED_SCL
Text Label 4810 3670 2    60   ~ 0
OLED_SDA
$Comp
L C C7
U 1 1 5A01213D
P 1500 1610
F 0 "C7" H 1525 1710 50  0000 L CNN
F 1 "4.7nF" H 1525 1510 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1538 1460 50  0001 C CNN
F 3 "" H 1500 1610 50  0001 C CNN
F 4 "CC0805KRX7R9BB472" H 1500 1610 60  0001 C CNN "Model"
	1    1500 1610
	1    0    0    -1  
$EndComp
$Comp
L R R14
U 1 1 5A0129A2
P 8630 3720
F 0 "R14" V 8540 3720 50  0000 C CNN
F 1 "27K" V 8720 3720 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8560 3720 50  0001 C CNN
F 3 "" H 8630 3720 50  0001 C CNN
F 4 "CRCW060327K0FKTABC" V 8630 3720 60  0001 C CNN "Model"
	1    8630 3720
	1    0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 5A012A39
P 8980 4010
F 0 "R17" V 8890 4010 50  0000 C CNN
F 1 "100K" V 9070 4010 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 8910 4010 50  0001 C CNN
F 3 "" H 8980 4010 50  0001 C CNN
F 4 "CRCW1206100KFKTABC" V 8980 4010 60  0001 C CNN "Model"
	1    8980 4010
	0    1    1    0   
$EndComp
$Comp
L C C18
U 1 1 5A012B2D
P 9290 4300
F 0 "C18" H 9315 4400 50  0000 L CNN
F 1 "4.7nF" H 9315 4200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 9328 4150 50  0001 C CNN
F 3 "" H 9290 4300 50  0001 C CNN
F 4 "CC0805KRX7R9BB472" H 9290 4300 60  0001 C CNN "Model"
	1    9290 4300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 5A012C23
P 8630 3370
F 0 "#PWR07" H 8630 3220 50  0001 C CNN
F 1 "+3.3V" H 8630 3510 50  0000 C CNN
F 2 "" H 8630 3370 50  0001 C CNN
F 3 "" H 8630 3370 50  0001 C CNN
	1    8630 3370
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 5A012CB5
P 8630 4660
F 0 "#PWR08" H 8630 4410 50  0001 C CNN
F 1 "GND" H 8630 4510 50  0000 C CNN
F 2 "" H 8630 4660 50  0001 C CNN
F 3 "" H 8630 4660 50  0001 C CNN
	1    8630 4660
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW2
U 1 1 5A012D9B
P 8630 4330
F 0 "SW2" H 8550 4250 50  0000 L CNN
F 1 "Up" H 8630 4520 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_PTS645" H 8630 4530 50  0001 C CNN
F 3 "" H 8630 4530 50  0001 C CNN
F 4 "KSC241JLFS" H 8630 4330 60  0001 C CNN "Model"
	1    8630 4330
	0    1    1    0   
$EndComp
Text Label 9610 4010 2    60   ~ 0
BTN1
$Comp
L R R15
U 1 1 5A015157
P 9900 3710
F 0 "R15" V 9810 3710 50  0000 C CNN
F 1 "27K" V 9990 3710 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9830 3710 50  0001 C CNN
F 3 "" H 9900 3710 50  0001 C CNN
F 4 "CRCW060327K0FKTABC" V 9900 3710 60  0001 C CNN "Model"
	1    9900 3710
	1    0    0    -1  
$EndComp
$Comp
L R R18
U 1 1 5A01515D
P 10250 4000
F 0 "R18" V 10160 4000 50  0000 C CNN
F 1 "100K" V 10340 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 10180 4000 50  0001 C CNN
F 3 "" H 10250 4000 50  0001 C CNN
F 4 "CRCW1206100KFKTABC" V 10250 4000 60  0001 C CNN "Model"
	1    10250 4000
	0    1    1    0   
$EndComp
$Comp
L C C19
U 1 1 5A015163
P 10560 4290
F 0 "C19" H 10585 4390 50  0000 L CNN
F 1 "4.7nF" H 10585 4190 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 10598 4140 50  0001 C CNN
F 3 "" H 10560 4290 50  0001 C CNN
F 4 "CC0805KRX7R9BB472" H 10560 4290 60  0001 C CNN "Model"
	1    10560 4290
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR09
U 1 1 5A015169
P 9900 3360
F 0 "#PWR09" H 9900 3210 50  0001 C CNN
F 1 "+3.3V" H 9900 3500 50  0000 C CNN
F 2 "" H 9900 3360 50  0001 C CNN
F 3 "" H 9900 3360 50  0001 C CNN
	1    9900 3360
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 5A01516F
P 9900 4650
F 0 "#PWR010" H 9900 4400 50  0001 C CNN
F 1 "GND" H 9900 4500 50  0000 C CNN
F 2 "" H 9900 4650 50  0001 C CNN
F 3 "" H 9900 4650 50  0001 C CNN
	1    9900 4650
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW3
U 1 1 5A015175
P 9900 4320
F 0 "SW3" H 9820 4240 50  0000 L CNN
F 1 "Select" H 9900 4510 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_PTS645" H 9900 4520 50  0001 C CNN
F 3 "" H 9900 4520 50  0001 C CNN
F 4 "KSC241JLFS" H 9900 4320 60  0001 C CNN "Model"
	1    9900 4320
	0    1    1    0   
$EndComp
Text Label 10880 4000 2    60   ~ 0
BTN2
$Comp
L R R16
U 1 1 5A015271
P 11110 3700
F 0 "R16" V 11020 3700 50  0000 C CNN
F 1 "27K" V 11200 3700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 11040 3700 50  0001 C CNN
F 3 "" H 11110 3700 50  0001 C CNN
F 4 "CRCW060327K0FKTABC" V 11110 3700 60  0001 C CNN "Model"
	1    11110 3700
	1    0    0    -1  
$EndComp
$Comp
L R R19
U 1 1 5A015277
P 11460 3990
F 0 "R19" V 11370 3990 50  0000 C CNN
F 1 "100K" V 11550 3990 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 11390 3990 50  0001 C CNN
F 3 "" H 11460 3990 50  0001 C CNN
F 4 "CRCW1206100KFKTABC" V 11460 3990 60  0001 C CNN "Model"
	1    11460 3990
	0    1    1    0   
$EndComp
$Comp
L C C20
U 1 1 5A01527D
P 11770 4280
F 0 "C20" H 11795 4380 50  0000 L CNN
F 1 "4.7nF" H 11795 4180 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 11808 4130 50  0001 C CNN
F 3 "" H 11770 4280 50  0001 C CNN
F 4 "CC0805KRX7R9BB472" H 11770 4280 60  0001 C CNN "Model"
	1    11770 4280
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR011
U 1 1 5A015283
P 11110 3350
F 0 "#PWR011" H 11110 3200 50  0001 C CNN
F 1 "+3.3V" H 11110 3490 50  0000 C CNN
F 2 "" H 11110 3350 50  0001 C CNN
F 3 "" H 11110 3350 50  0001 C CNN
	1    11110 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 5A015289
P 11110 4640
F 0 "#PWR012" H 11110 4390 50  0001 C CNN
F 1 "GND" H 11110 4490 50  0000 C CNN
F 2 "" H 11110 4640 50  0001 C CNN
F 3 "" H 11110 4640 50  0001 C CNN
	1    11110 4640
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW4
U 1 1 5A01528F
P 11110 4310
F 0 "SW4" H 11030 4230 50  0000 L CNN
F 1 "Down" H 11110 4490 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_PTS645" H 11110 4510 50  0001 C CNN
F 3 "" H 11110 4510 50  0001 C CNN
F 4 "KSC241JLFS" H 11110 4310 60  0001 C CNN "Model"
	1    11110 4310
	0    1    1    0   
$EndComp
Text Label 12090 3990 2    60   ~ 0
BTN3
Text Label 3560 3870 2    60   ~ 0
BTN1
Text Label 3560 3970 2    60   ~ 0
BTN2
Text Label 3560 4070 2    60   ~ 0
BTN3
Text Label 3720 2570 2    60   ~ 0
ISP_PDI_MOSI
Text Label 3720 2670 2    60   ~ 0
ISP_PDO_MISO
Text Label 4180 1610 2    60   ~ 0
ISP_PDI_MOSI
Text Label 2400 1710 0    60   ~ 0
ISP_PDO_MISO
$Comp
L Conn_01x02 J6
U 1 1 5A020B3D
P 4190 7130
F 0 "J6" H 4190 7250 50  0000 C CNN
F 1 "Battery" H 4160 6900 50  0000 C CNN
F 2 "Xling Footprints:Pin_Header_Straight_1x02" H 4190 7130 50  0001 C CNN
F 3 "" H 4190 7130 50  0001 C CNN
F 4 "empty" H 4190 7130 60  0001 C CNN "Model"
	1    4190 7130
	1    0    0    -1  
$EndComp
$Comp
L B320 D5
U 1 1 5A05A5FD
P 5530 6320
F 0 "D5" H 5530 6450 50  0000 C CNN
F 1 "1N5818" H 5530 6190 50  0000 C CNN
F 2 "Diodes_THT:D_DO-41_SOD81_P7.62mm_Horizontal" H 5530 6145 50  0001 C CNN
F 3 "" H 5530 6320 50  0001 C CNN
F 4 "1N5819" H 5530 6320 60  0001 C CNN "Model"
	1    5530 6320
	0    -1   -1   0   
$EndComp
$Comp
L ATMEGA328P-AU U2
U 1 1 5A096E7F
P 2040 3370
F 0 "U2" H 1300 1980 50  0000 L BNN
F 1 "ATMEGA328P-AU" H 2240 1980 50  0000 L BNN
F 2 "Housings_QFP:TQFP-32_7x7mm_Pitch0.8mm" H 2040 3370 50  0001 C CIN
F 3 "" H 2040 3370 50  0001 C CNN
F 4 "ATMEGA328P-AU" H 2040 3370 60  0001 C CNN "Model"
	1    2040 3370
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 5A09844B
P 1000 2930
F 0 "#PWR013" H 1000 2680 50  0001 C CNN
F 1 "GND" H 1000 2780 50  0000 C CNN
F 2 "" H 1000 2930 50  0001 C CNN
F 3 "" H 1000 2930 50  0001 C CNN
	1    1000 2930
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5A0995CE
P 4700 3050
F 0 "#PWR014" H 4700 2800 50  0001 C CNN
F 1 "GND" H 4700 2900 50  0000 C CNN
F 2 "" H 4700 3050 50  0001 C CNN
F 3 "" H 4700 3050 50  0001 C CNN
	1    4700 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 5A09BBD8
P 1010 4690
F 0 "#PWR015" H 1010 4440 50  0001 C CNN
F 1 "GND" H 1010 4540 50  0000 C CNN
F 2 "" H 1010 4690 50  0001 C CNN
F 3 "" H 1010 4690 50  0001 C CNN
	1    1010 4690
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR016
U 1 1 59FF6F02
P 4170 3310
F 0 "#PWR016" H 4170 3160 50  0001 C CNN
F 1 "+3.3V" H 4170 3450 50  0000 C CNN
F 2 "" H 4170 3310 50  0001 C CNN
F 3 "" H 4170 3310 50  0001 C CNN
	1    4170 3310
	1    0    0    -1  
$EndComp
$Comp
L DEP_128064I U3
U 1 1 5A09DE1E
P 7160 2690
F 0 "U3" H 6680 1460 60  0000 C CNN
F 1 "DEP_128064I" H 7380 1460 60  0000 C CNN
F 2 "Xling Footprints:XF2M_24151AR100_SMD_1x24_Pitch_0.5mm" H 6800 3320 60  0001 C CNN
F 3 "" H 6800 3320 60  0001 C CNN
F 4 "XF2M24151AR100,DEP128064I-Y" H 7160 2690 60  0001 C CNN "Model"
	1    7160 2690
	1    0    0    -1  
$EndComp
$Comp
L LT1109CZ-12 U1
U 1 1 5A0ABBC5
P 9620 6190
F 0 "U1" H 9400 6560 60  0000 C CNN
F 1 "LT1109CZ-12" H 9950 5960 60  0000 C CNN
F 2 "Xling Footprints:TO-92_LT1109CZ" H 9620 6190 60  0001 C CNN
F 3 "" H 9620 6190 60  0001 C CNN
F 4 "LT1109CZ-12PBF" H 9620 6190 60  0001 C CNN "Model"
	1    9620 6190
	1    0    0    -1  
$EndComp
$Comp
L 1N5818 D3
U 1 1 5A0ABC52
P 9930 5590
F 0 "D3" H 9930 5470 50  0000 C CNN
F 1 "1N5818" H 9930 5720 50  0000 C CNN
F 2 "Diodes_THT:D_DO-41_SOD81_P7.62mm_Horizontal" H 9930 5415 50  0001 C CNN
F 3 "" H 9930 5590 50  0001 C CNN
F 4 "1N5819" H 9930 5590 60  0001 C CNN "Model"
	1    9930 5590
	-1   0    0    1   
$EndComp
$Comp
L L_Core_Ferrite L2
U 1 1 5A0ABF6C
P 9290 5590
F 0 "L2" V 9410 5590 50  0000 C CNN
F 1 "22uH" V 9220 5590 50  0000 C CNN
F 2 "Inductors_SMD:L_7.3x7.3_H3.5" H 9290 5590 50  0001 C CNN
F 3 "" H 9290 5590 50  0001 C CNN
F 4 "DE0703-22" V 9290 5590 60  0001 C CNN "Model"
	1    9290 5590
	0    -1   -1   0   
$EndComp
$Comp
L CP C10
U 1 1 5A0AC21A
P 10400 6370
F 0 "C10" H 10425 6470 50  0000 L CNN
F 1 "33uF" H 10425 6270 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:CP_Tantalum_Case-C_EIA-6032-28_Reflow" H 10438 6220 50  0001 C CNN
F 3 "" H 10400 6370 50  0001 C CNN
F 4 "TPSC336K016R0150" H 10400 6370 60  0001 C CNN "Model"
	1    10400 6370
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 5A0AC3B9
P 9580 6690
F 0 "#PWR017" H 9580 6440 50  0001 C CNN
F 1 "GND" H 9580 6540 50  0000 C CNN
F 2 "" H 9580 6690 50  0001 C CNN
F 3 "" H 9580 6690 50  0001 C CNN
	1    9580 6690
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR018
U 1 1 5A0ACF51
P 10600 5530
F 0 "#PWR018" H 10600 5380 50  0001 C CNN
F 1 "+12V" H 10600 5670 50  0000 C CNN
F 2 "" H 10600 5530 50  0001 C CNN
F 3 "" H 10600 5530 50  0001 C CNN
	1    10600 5530
	1    0    0    -1  
$EndComp
Text Notes 10660 6470 0    51   ~ 0
tantalum\nlow ESR 0.1Ω\nAVX TPSC336K016R0150
Text Notes 8770 5970 0    51   ~ 0
22uH, 0.19Ω\nFERROCORE\nDE0703-22
Text Notes 10680 5630 0    51   ~ 0
max.\n45mA
Text Notes 8550 5200 0    98   ~ 20
3.3V to 12V, 45mA DC-DC for OLED panel
Text Notes 8700 8020 0    51   ~ 0
Power required from the inductor:\nPl = (Vout+Vd-Vin)*Iout = (12V+0.5V-3.3V)(45mA) = 414mW\n\nEnergy required by the inductor per cycle must be >=:\nPl/Fosc = 414mW/120kHz = 3.45 uJ\n\nPicking an inductor value of 22μH with 0.19R DCR\nresults in a peak switch current of:\nIpeak = (3.3V/(0.8R+0.19R))*(1-e^((-1.0*4.2us)/(22uH))) = 579mA\n\nOnce Ipeak is known, energy in the inductor at the end\nof the switch-ON time can be calculated as:\nEl = 0.5*(L*Ipeak^2) = 0.5*(22uH)*(0.579)^2 = 3.69uJ > 3.45uJ\n
$Comp
L GND #PWR019
U 1 1 5A0B7FD6
P 6260 4240
F 0 "#PWR019" H 6260 3990 50  0001 C CNN
F 1 "GND" H 6260 4090 50  0000 C CNN
F 2 "" H 6260 4240 50  0001 C CNN
F 3 "" H 6260 4240 50  0001 C CNN
	1    6260 4240
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR020
U 1 1 5A0B88C5
P 6190 1550
F 0 "#PWR020" H 6190 1400 50  0001 C CNN
F 1 "+3.3V" H 6190 1690 50  0000 C CNN
F 2 "" H 6190 1550 50  0001 C CNN
F 3 "" H 6190 1550 50  0001 C CNN
	1    6190 1550
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5A0BDAED
P 5710 3970
F 0 "R2" V 5790 3970 50  0000 C CNN
F 1 "910K" V 5610 3970 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 5640 3970 50  0001 C CNN
F 3 "" H 5710 3970 50  0001 C CNN
F 4 "1206S4F9103T5E" V 5710 3970 60  0001 C CNN "Model"
	1    5710 3970
	-1   0    0    1   
$EndComp
$Comp
L C C11
U 1 1 5A0C00CE
P 5970 3980
F 0 "C11" H 5995 4080 50  0000 L CNN
F 1 "2.2uF" H 5995 3880 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6008 3830 50  0001 C CNN
F 3 "" H 5970 3980 50  0001 C CNN
F 4 "0603ZD225KAT2A" H 5970 3980 60  0001 C CNN "Model"
	1    5970 3980
	1    0    0    -1  
$EndComp
Text Notes 5520 4700 0    51   ~ 0
It is recommended to set Iref to 10±2uA:\nR2 = (Vcc-3)/Iref = (12V-3V)/10uA = 900kΩ
$Comp
L MCP1640B-CH U4
U 1 1 5A0D6820
P 6410 6610
F 0 "U4" H 6110 7035 50  0000 L CNN
F 1 "MCP1640B-CH" H 6110 6960 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6" H 6410 6110 50  0001 C CIN
F 3 "" H 6160 7060 50  0001 C CNN
F 4 "MCP1640BT-I/CHY" H 6410 6610 60  0001 C CNN "Model"
	1    6410 6610
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5A0D8CE3
P 7250 6510
F 0 "R6" V 7150 6510 50  0000 C CNN
F 1 "191K" V 7340 6510 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7180 6510 50  0001 C CNN
F 3 "" H 7250 6510 50  0001 C CNN
F 4 "RC0805FR-07196KL" V 7250 6510 60  0001 C CNN "Model"
	1    7250 6510
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 5A0D8DA0
P 7250 6890
F 0 "R7" V 7150 6890 50  0000 C CNN
F 1 "110K" V 7340 6890 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7180 6890 50  0001 C CNN
F 3 "" H 7250 6890 50  0001 C CNN
F 4 "CRCW0805110KJNTABC" V 7250 6890 60  0001 C CNN "Model"
	1    7250 6890
	1    0    0    -1  
$EndComp
Text Notes 6060 7690 0    51   ~ 0
Resistor devider:\nR6 = R7*(Vout/1.21-1) =\n(110*10^3)*(3.3/1.21-1) = 190kOhm
$Comp
L Crystal Y1
U 1 1 5A0083D0
P 3960 2820
F 0 "Y1" V 4180 2820 50  0000 C CNN
F 1 "8MHz" V 3720 2820 50  0000 C CNN
F 2 "Xling Footprints:Crystal_HC49-SD_SMD" H 3960 2820 50  0001 C CNN
F 3 "" H 3960 2820 50  0001 C CNN
F 4 "LF XTAL003151" V 3960 2820 60  0001 C CNN "Model"
	1    3960 2820
	0    1    -1   0   
$EndComp
$Comp
L C C3
U 1 1 5A657A90
P 1680 7010
F 0 "C3" H 1705 7110 50  0000 L CNN
F 1 "4.7uF" H 1705 6910 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 1718 6860 50  0001 C CNN
F 3 "" H 1680 7010 50  0001 C CNN
F 4 "CL31B475KAHNNNE" H 1680 7010 60  0001 C CNN "Model"
	1    1680 7010
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5A657DFD
P 3690 7160
F 0 "C6" H 3715 7260 50  0000 L CNN
F 1 "4.7uF" H 3715 7060 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 3728 7010 50  0001 C CNN
F 3 "" H 3690 7160 50  0001 C CNN
F 4 "CL31B475KAHNNNE" H 3690 7160 60  0001 C CNN "Model"
	1    3690 7160
	1    0    0    -1  
$EndComp
Text Label 5370 2460 0    60   ~ 0
OLED_RES
$Comp
L R R1
U 1 1 5AABB267
P 6050 1970
F 0 "R1" V 5960 1970 50  0000 C CNN
F 1 "100K" V 6130 1970 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 5980 1970 50  0001 C CNN
F 3 "" H 6050 1970 50  0001 C CNN
F 4 "CRCW1206100KFKTABC" V 6050 1970 60  0001 C CNN "Model"
	1    6050 1970
	1    0    0    -1  
$EndComp
Text Label 3560 4170 2    60   ~ 0
OLED_RES
Text Label 3560 4370 2    60   ~ 0
BAT_STAT
Text Label 1910 6810 0    60   ~ 0
BAT_STAT
$Comp
L NTR2101P Q2
U 1 1 5AAEB4E3
P 10020 1830
F 0 "Q2" H 10220 1790 50  0000 L CNN
F 1 "NTR0202PL" H 10220 1860 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 10220 1755 50  0001 L CIN
F 3 "" H 10020 1830 50  0001 L CNN
F 4 "NTR0202PLT1G" H 10020 1830 60  0001 C CNN "Model"
	1    10020 1830
	1    0    0    1   
$EndComp
$Comp
L R R10
U 1 1 5AAEF25A
P 9030 2330
F 0 "R10" V 8930 2330 50  0000 C CNN
F 1 "100K" V 9130 2330 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 8960 2330 50  0001 C CNN
F 3 "" H 9030 2330 50  0001 C CNN
F 4 "CRCW1206100KFKTABC" V 9030 2330 60  0001 C CNN "Model"
	1    9030 2330
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5AAEF30D
P 9520 1570
F 0 "R5" V 9440 1570 50  0000 C CNN
F 1 "100K" V 9600 1570 50  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 9450 1570 50  0001 C CNN
F 3 "" H 9520 1570 50  0001 C CNN
F 4 "CRCW1206100KFKTABC" V 9520 1570 60  0001 C CNN "Model"
	1    9520 1570
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 5AAF0F77
P 9520 2550
F 0 "#PWR021" H 9520 2300 50  0001 C CNN
F 1 "GND" H 9520 2400 50  0000 C CNN
F 2 "" H 9520 2550 50  0001 C CNN
F 3 "" H 9520 2550 50  0001 C CNN
	1    9520 2550
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5AAFBAC6
P 10120 2310
F 0 "C9" H 10145 2410 50  0000 L CNN
F 1 "1pF" H 10145 2210 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 10158 2160 50  0001 C CNN
F 3 "" H 10120 2310 50  0001 C CNN
F 4 "CL21C010BBANNNC" H 10120 2310 60  0001 C CNN "Model"
	1    10120 2310
	1    0    0    -1  
$EndComp
Text Label 3560 4270 2    60   ~ 0
OLED_PWR
Text Label 8500 2130 0    60   ~ 0
OLED_PWR
Text Label 10890 2100 2    60   ~ 0
OLED_VCC
Text Notes 5070 5800 0    51   ~ 0
The main idea of a scheme is to produce 3.3V and charge a battery\nif it needs using load-sharing. This can be done by providing a 5V USB \nconnection to power the whole circuit via LM1117 and\ncharge a 3.7V Li-Ion or Li-Polymer battery using MCP73831 on one hand.\nOn the other hand, it can work autonomously using the battery and\nchanging its voltage down to 3.3V via MCP1640B.
Text Notes 2630 1000 0    98   ~ 20
Microcontroller ATMEGA328P
Text Notes 6430 980  0    98   ~ 20
OLED display 128x64
Text Notes 10530 990  0    98   ~ 20
12V OLED Vcc switch
Text Notes 10120 3090 0    98   ~ 20
Buttons for microcontroller
Text Notes 10500 1590 0    51   ~ 0
This switch is based on N-Channel and\nP-Channel MOSFETs to power on and off\nOLED display. This is necessary to use\na logic level 3.3V to switch 12V and\nleave Vcc contact floating when\nNTR0202PL is closed.
Text Notes 12340 3970 0    51   ~ 0
*\n* Including model files\n*\n*.include NTA4001NT1.LIB\n.include NTA4153N.REV0.LIB\n.include NTR0202PL.REV0.LIB\n*\n* Components of the scheme\n*\nR10 0 1 100k\nR5 2 3 100k\n* R3 - pull-down resistor to prove an output is floating contact when\n* XQ2 is closed\n*R3 4 0 100k\nC9 4 0 1p\nXQ3 2 1 0 nta4153n\nXQ2 4 2 3 ntr0202plt1\nVIN 1 0 pulse (0 3.15 2ms 0ns 0ms 5ms 10ms)\nVDD 3 0 pwl (0 0 0ms 0 0ms 12v 20ms 12v)\n*\n* Transient analysis for 20ms, step size 0.02ms\n*\n.tran 0.02ms 20ms\n*\n* Defining the run-time control functions\n*\n.control\nrun\n*\n* Plotting input and output voltages\n*\nplot v(1) v(4)\n.endc\n.end
Text Notes 12520 990  0    98   ~ 20
12V OLED Vcc switch (transient analysis)
$Comp
L NTA4153N Q3
U 1 1 5B3E89E3
P 9420 2130
F 0 "Q3" H 9620 2205 50  0000 L CNN
F 1 "NTA4153N" H 9620 2130 50  0000 L CNN
F 2 "Xling Footprints:SC-75_SOT-416" H 9620 2055 50  0001 L CIN
F 3 "" H 9420 2130 50  0001 L CNN
F 4 "NTA4153NT1G" H 9420 2130 60  0001 C CNN "Model"
	1    9420 2130
	1    0    0    -1  
$EndComp
$Comp
L NTR2101P Q1
U 1 1 5B447641
P 5040 6410
F 0 "Q1" V 4980 6190 50  0000 L CNN
F 1 "NTR0202PL" V 5300 6100 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5240 6335 50  0001 L CIN
F 3 "" H 5040 6410 50  0001 L CNN
F 4 "NTR0202PLT1G" H 5040 6410 60  0001 C CNN "Model"
	1    5040 6410
	0    -1   1    0   
$EndComp
$Comp
L GND #PWR022
U 1 1 5B4536C6
P 5530 7410
F 0 "#PWR022" H 5530 7160 50  0001 C CNN
F 1 "GND" H 5530 7260 50  0000 C CNN
F 2 "" H 5530 7410 50  0001 C CNN
F 3 "" H 5530 7410 50  0001 C CNN
	1    5530 7410
	1    0    0    -1  
$EndComp
Text Notes 12330 7260 0    51   ~ 0
Current Direction Control Circuit based on a single P-Channel MOSFET\n\n* Including model files\n*.include NTA4001NT1.LIB\n.include NTA4153N.REV0.LIB\n.include NTR0202PL.REV0.LIB\n.include 1N5819.LIB\n\n* Components of the scheme\nR1 0 1 100k\nR2 0 2 12\nC1 0 3 4.7u\nC2 0 2 4.7u\nXQ1 3 1 2 ntr0202plt1\nD1 1 2 D1n5819\nVIN 1 0 pulse (0 4.95 2ms 0ns 0ms 5ms 10ms)\nVDD 3 0 pwl (0 0 0ms 0 0ms 3.2v 20ms 3.2v)\n\n* Transient analysis for 20ms, step size 0.02ms\n.tran 0.02ms 20ms\n\n* Defining the run-time control functions\n.control\nrun\n\n* Plotting input and output voltages\nplot v(1) v(3) v(2)\n.endc\n.end
Text Notes 12370 4670 0    98   ~ 20
Current Direction Control Circuit\nbased on a single P-channel MOSFET\n(part of a battery charger)
Text Notes 3570 8180 0    51   ~ 0
Charge current regulation:\nIreg = 1000V/Rprog, where\nIreg - charge current, in mA\nRprog - resistor value (R3), in kOhm.\n\nRprog = 1000/380 ~~ 2.6316kOhm,\nStandard value is 2.7kOhm, Ireg ~~ 370mA
Text Label 8660 5590 0    60   ~ 0
OLED_VCC
Connection ~ 7250 7360
Wire Wire Line
	7250 7360 7250 7040
Wire Wire Line
	3230 6510 4840 6510
Wire Wire Line
	3990 6510 3990 7130
Wire Wire Line
	3990 7540 3990 7230
Wire Wire Line
	5970 3630 6420 3630
Wire Wire Line
	5970 3830 5970 3630
Wire Wire Line
	5710 3540 6420 3540
Wire Wire Line
	5710 3540 5710 3820
Connection ~ 6260 3450
Wire Notes Line
	15680 780  12230 780 
Wire Notes Line
	15680 4080 15680 780 
Wire Notes Line
	12230 4080 15680 4080
Wire Notes Line
	12230 780  12230 4080
Wire Notes Line
	8150 4950 8150 2900
Wire Notes Line
	12200 2900 12200 4950
Wire Notes Line
	8150 2900 12200 2900
Wire Notes Line
	8150 2870 8150 780 
Wire Notes Line
	12200 2870 8150 2870
Wire Notes Line
	12200 780  12200 2870
Wire Notes Line
	8150 780  12200 780 
Wire Notes Line
	12200 4950 8150 4950
Wire Notes Line
	8150 4980 12200 4980
Wire Notes Line
	8120 780  4900 780 
Wire Notes Line
	8120 4950 8120 780 
Wire Notes Line
	4900 4950 8120 4950
Wire Notes Line
	4900 780  4900 4950
Wire Notes Line
	710  4950 710  780 
Wire Notes Line
	4860 4950 710  4950
Wire Notes Line
	4860 780  4860 4950
Wire Notes Line
	710  780  4860 780 
Connection ~ 1680 7540
Connection ~ 3230 7540
Wire Notes Line
	710  8250 710  4980
Wire Notes Line
	710  8250 8120 8250
Wire Notes Line
	710  4980 8120 4980
Connection ~ 10120 2100
Wire Wire Line
	10890 2100 10120 2100
Wire Wire Line
	10120 2520 10120 2460
Connection ~ 9520 2520
Wire Wire Line
	9030 2520 10120 2520
Wire Wire Line
	3580 1710 3580 1930
Wire Wire Line
	3550 1710 3580 1710
Wire Wire Line
	3580 1510 3550 1510
Wire Wire Line
	3580 1330 3580 1510
Wire Wire Line
	3040 4270 3560 4270
Wire Wire Line
	10120 2030 10120 2160
Wire Wire Line
	10120 1390 10120 1630
Wire Wire Line
	9520 1390 10120 1390
Connection ~ 9030 2130
Wire Wire Line
	9030 2480 9030 2520
Wire Wire Line
	9520 2330 9520 2550
Wire Wire Line
	8500 2130 9220 2130
Wire Wire Line
	9030 2180 9030 2130
Connection ~ 9520 1830
Wire Wire Line
	9820 1830 9520 1830
Wire Wire Line
	9520 1720 9520 1930
Connection ~ 6190 1620
Wire Wire Line
	6050 1620 6190 1620
Wire Wire Line
	6050 1820 6050 1620
Wire Wire Line
	1910 6810 2330 6810
Connection ~ 6050 2460
Wire Wire Line
	6050 2120 6050 2460
Wire Wire Line
	5370 2460 6420 2460
Wire Wire Line
	3040 4370 3560 4370
Wire Wire Line
	5370 2820 6420 2820
Connection ~ 6340 2910
Wire Wire Line
	6340 3000 6340 2910
Wire Wire Line
	5370 2910 6420 2910
Wire Wire Line
	6260 4210 5710 4210
Connection ~ 5970 4210
Wire Wire Line
	5970 4210 5970 4130
Connection ~ 6260 4210
Wire Wire Line
	5710 4210 5710 4120
Wire Wire Line
	6420 3000 6340 3000
Wire Wire Line
	3040 4070 3560 4070
Wire Wire Line
	3040 3970 3560 3970
Wire Wire Line
	3040 3870 3560 3870
Wire Wire Line
	3040 4170 3560 4170
Connection ~ 1680 6510
Wire Wire Line
	1680 6160 1680 6860
Connection ~ 7250 6300
Wire Wire Line
	7000 6300 7590 6300
Wire Wire Line
	7000 6610 7000 6300
Wire Wire Line
	7250 6220 7250 6360
Connection ~ 7250 6710
Wire Wire Line
	6810 6710 7250 6710
Wire Wire Line
	6870 6410 6810 6410
Wire Wire Line
	6870 5980 6870 6410
Wire Wire Line
	6550 5980 6870 5980
Wire Wire Line
	6250 5980 5850 5980
Wire Wire Line
	5850 5980 5850 6710
Wire Wire Line
	1000 1380 1000 2570
Connection ~ 1500 1380
Connection ~ 1810 1380
Wire Wire Line
	1000 1380 1810 1380
Connection ~ 1810 1860
Wire Wire Line
	1500 1860 1810 1860
Wire Wire Line
	6810 6610 7000 6610
Wire Wire Line
	7250 6660 7250 6740
Wire Wire Line
	5850 6710 6010 6710
Connection ~ 6260 2370
Wire Wire Line
	6260 2370 6420 2370
Wire Wire Line
	6260 3450 6420 3450
Connection ~ 6260 3360
Wire Wire Line
	6260 3360 6420 3360
Connection ~ 6260 3270
Wire Wire Line
	6260 3270 6420 3270
Connection ~ 6260 3180
Wire Wire Line
	6260 3180 6420 3180
Connection ~ 6260 3090
Wire Wire Line
	6260 3090 6420 3090
Connection ~ 6260 2730
Wire Wire Line
	6260 2730 6420 2730
Connection ~ 6260 2640
Wire Wire Line
	6260 2640 6420 2640
Connection ~ 6260 1920
Wire Wire Line
	6420 1830 6260 1830
Connection ~ 6260 2280
Wire Wire Line
	6260 1920 6420 1920
Wire Wire Line
	6190 2550 6420 2550
Connection ~ 6190 2190
Wire Wire Line
	6260 1830 6260 4240
Wire Wire Line
	6260 2280 6420 2280
Connection ~ 6190 2100
Wire Wire Line
	6190 2190 6420 2190
Wire Wire Line
	6190 2100 6420 2100
Wire Wire Line
	6190 1550 6190 2550
Wire Wire Line
	3720 3870 4810 3870
Wire Wire Line
	10600 5590 10600 5530
Connection ~ 10400 5590
Wire Wire Line
	8660 5590 9140 5590
Connection ~ 9580 5590
Wire Wire Line
	9580 5590 9580 5670
Wire Wire Line
	9440 5590 9780 5590
Connection ~ 10400 6110
Wire Wire Line
	10080 5590 10600 5590
Wire Wire Line
	10400 5590 10400 6220
Wire Wire Line
	10040 6110 10400 6110
Connection ~ 9580 6630
Wire Wire Line
	10400 6630 9580 6630
Wire Wire Line
	10400 6520 10400 6630
Wire Wire Line
	9580 6560 9580 6690
Connection ~ 4290 3770
Wire Wire Line
	4290 3610 4290 3770
Connection ~ 4050 3670
Wire Wire Line
	4050 3610 4050 3670
Connection ~ 4170 3310
Wire Wire Line
	4050 3310 4290 3310
Wire Wire Line
	3040 2570 3720 2570
Wire Wire Line
	3040 2670 3720 2670
Wire Wire Line
	3040 2770 3720 2770
Connection ~ 1010 4470
Wire Wire Line
	1140 4370 1010 4370
Connection ~ 1010 4570
Wire Wire Line
	1140 4470 1010 4470
Wire Wire Line
	1010 4570 1140 4570
Wire Wire Line
	1010 4370 1010 4690
Wire Wire Line
	3760 3770 4810 3770
Wire Wire Line
	3800 3670 4810 3670
Connection ~ 4700 2970
Wire Wire Line
	4700 2670 4700 3050
Wire Wire Line
	1000 2870 1000 2930
Wire Wire Line
	1140 2870 1000 2870
Connection ~ 1000 2370
Wire Wire Line
	1000 2570 1140 2570
Connection ~ 1000 2270
Wire Wire Line
	1000 2370 1140 2370
Wire Wire Line
	1000 2270 1140 2270
Wire Wire Line
	8630 3370 8630 3570
Wire Wire Line
	2400 1510 3050 1510
Connection ~ 11770 3990
Connection ~ 11110 4590
Wire Wire Line
	11770 4590 11110 4590
Wire Wire Line
	11770 4430 11770 4590
Wire Wire Line
	11110 4510 11110 4640
Wire Wire Line
	11770 3990 11770 4130
Wire Wire Line
	11610 3990 12090 3990
Connection ~ 11110 3990
Wire Wire Line
	11310 3990 11110 3990
Wire Wire Line
	11110 3550 11110 3350
Wire Wire Line
	11110 3850 11110 4110
Connection ~ 10560 4000
Connection ~ 9900 4600
Wire Wire Line
	10560 4600 9900 4600
Wire Wire Line
	10560 4440 10560 4600
Wire Wire Line
	9900 4520 9900 4650
Wire Wire Line
	10560 4000 10560 4140
Wire Wire Line
	10400 4000 10880 4000
Connection ~ 9900 4000
Wire Wire Line
	10100 4000 9900 4000
Wire Wire Line
	9900 3560 9900 3360
Wire Wire Line
	9900 3860 9900 4120
Connection ~ 9290 4010
Connection ~ 8630 4610
Wire Wire Line
	9290 4610 8630 4610
Wire Wire Line
	9290 4450 9290 4610
Wire Wire Line
	8630 4530 8630 4660
Wire Wire Line
	9290 4010 9290 4150
Wire Wire Line
	9130 4010 9610 4010
Connection ~ 8630 4010
Wire Wire Line
	8830 4010 8630 4010
Wire Wire Line
	8630 3870 8630 4130
Wire Wire Line
	1500 1380 1500 1460
Wire Wire Line
	1500 1760 1500 1860
Wire Wire Line
	2400 1710 3050 1710
Wire Wire Line
	2400 1610 3050 1610
Wire Wire Line
	3550 1610 4180 1610
Wire Wire Line
	1810 1760 1810 1900
Wire Wire Line
	1810 1340 1810 1460
Wire Wire Line
	4520 2670 4700 2670
Wire Wire Line
	4700 2970 4520 2970
Connection ~ 3690 6510
Wire Wire Line
	3690 6510 3690 7010
Wire Wire Line
	1030 7540 4760 7540
Connection ~ 3690 7540
Wire Wire Line
	3690 7540 3690 7310
Connection ~ 3390 7540
Wire Wire Line
	3390 7310 3390 7540
Wire Wire Line
	3390 6660 3390 7010
Wire Wire Line
	3230 6660 3390 6660
Wire Wire Line
	3230 7540 3230 6810
Connection ~ 1030 7250
Wire Wire Line
	930  7250 1030 7250
Wire Wire Line
	930  7110 930  7250
Wire Wire Line
	1030 7110 1030 7540
Wire Wire Line
	1680 7160 1680 7540
Wire Wire Line
	1330 6510 2330 6510
Wire Notes Line
	8120 4980 8120 8250
Wire Wire Line
	7590 6300 7590 6570
Wire Wire Line
	7590 7360 7590 6870
Wire Notes Line
	8150 4980 8150 8250
Wire Notes Line
	8150 8250 12200 8250
Wire Notes Line
	12200 8250 12200 4980
Wire Wire Line
	5530 7360 7590 7360
Wire Wire Line
	5530 7040 5530 7410
Connection ~ 5530 7360
Wire Wire Line
	5530 6470 5530 6740
Wire Wire Line
	6410 7010 6410 7360
Connection ~ 6410 7360
Connection ~ 3990 6510
Wire Wire Line
	5530 6160 5530 6170
Connection ~ 3990 7540
Wire Notes Line
	15680 4110 12230 4110
Wire Notes Line
	15680 7410 15680 4110
Wire Notes Line
	12230 7410 15680 7410
Wire Notes Line
	12230 4110 12230 7410
Connection ~ 9520 1390
Wire Wire Line
	9520 1350 9520 1420
$Comp
L +3.3V #PWR023
U 1 1 5A0AC2F4
P 9520 1350
F 0 "#PWR023" H 9520 1200 50  0001 C CNN
F 1 "+3.3V" H 9520 1490 50  0000 C CNN
F 2 "" H 9520 1350 50  0001 C CNN
F 3 "" H 9520 1350 50  0001 C CNN
	1    9520 1350
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR024
U 1 1 5A0BC6A5
P 6060 3470
F 0 "#PWR024" H 6060 3320 50  0001 C CNN
F 1 "+12V" H 6060 3610 50  0000 C CNN
F 2 "" H 6060 3470 50  0001 C CNN
F 3 "" H 6060 3470 50  0001 C CNN
	1    6060 3470
	1    0    0    -1  
$EndComp
Wire Wire Line
	1680 6160 5530 6160
Wire Wire Line
	5240 6510 6010 6510
Connection ~ 5850 6510
Connection ~ 5530 6510
Wire Wire Line
	5040 6210 5040 6160
Connection ~ 5040 6160
$Comp
L R R11
U 1 1 5B48ABDB
P 4470 6820
F 0 "R11" V 4370 6820 50  0000 C CNN
F 1 "191K" V 4560 6820 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4400 6820 50  0001 C CNN
F 3 "" H 4470 6820 50  0001 C CNN
F 4 "RC0805FR-07196KL" V 4470 6820 60  0001 C CNN "Model"
	1    4470 6820
	1    0    0    -1  
$EndComp
$Comp
L R R12
U 1 1 5B48AD6F
P 4470 7270
F 0 "R12" V 4370 7270 50  0000 C CNN
F 1 "110K" V 4560 7270 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4400 7270 50  0001 C CNN
F 3 "" H 4470 7270 50  0001 C CNN
F 4 "CRCW0805110KJNTABC" V 4470 7270 60  0001 C CNN "Model"
	1    4470 7270
	1    0    0    -1  
$EndComp
Wire Wire Line
	4470 6970 4470 7120
Wire Wire Line
	4470 7540 4470 7420
Wire Wire Line
	4760 7540 4760 6970
Connection ~ 4470 7540
Wire Wire Line
	4760 6670 4760 6160
Connection ~ 4760 6160
Wire Wire Line
	4470 7080 5270 7080
Connection ~ 4470 7080
Wire Wire Line
	4470 6670 4470 6510
Connection ~ 4470 6510
Wire Wire Line
	3040 2870 3800 2870
Wire Wire Line
	3800 2870 3800 2670
Wire Wire Line
	3800 2670 4220 2670
Connection ~ 3960 2670
Wire Wire Line
	3040 2970 4220 2970
Connection ~ 3960 2970
Wire Wire Line
	3040 3720 3720 3720
Wire Wire Line
	3720 3720 3720 3870
Wire Wire Line
	3040 3620 3760 3620
Wire Wire Line
	3760 3620 3760 3770
Wire Wire Line
	3040 3520 3800 3520
Wire Wire Line
	3800 3520 3800 3670
Wire Wire Line
	3040 3120 3560 3120
Text Label 5270 7080 2    60   ~ 0
BAT_LVL
Text Label 3560 3120 2    60   ~ 0
BAT_LVL
Wire Wire Line
	6060 3470 6060 3720
Wire Wire Line
	6060 3720 6420 3720
$EndSCHEMATC
