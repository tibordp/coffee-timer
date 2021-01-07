EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Coffee Timer"
Date "2021-01-07"
Rev "1"
Comp "Tibor Djurica Potpara"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATtiny:ATtiny85-20PU U1
U 1 1 5FF5E76A
P 1800 1900
F 0 "U1" H 1271 1946 50  0000 R CNN
F 1 "ATtiny85-20PU" H 1271 1855 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 1800 1900 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf" H 1800 1900 50  0001 C CNN
	1    1800 1900
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC595 U3
U 1 1 5FF5F5CB
P 4050 1900
F 0 "U3" H 4300 2600 50  0000 C CNN
F 1 "74HC595" H 4300 2500 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 4050 1900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 4050 1900 50  0001 C CNN
	1    4050 1900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 5FF6532A
P 8500 3700
F 0 "SW1" H 8500 3985 50  0000 C CNN
F 1 "SW_Push" H 8500 3894 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 8500 3900 50  0001 C CNN
F 3 "~" H 8500 3900 50  0001 C CNN
	1    8500 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:Buzzer BZ1
U 1 1 5FF666AA
P 9150 5250
F 0 "BZ1" H 9302 5279 50  0000 L CNN
F 1 "Buzzer" H 9302 5188 50  0000 L CNN
F 2 "Buzzer_Beeper:Buzzer_12x9.5RM7.6" V 9125 5350 50  0001 C CNN
F 3 "~" V 9125 5350 50  0001 C CNN
	1    9150 5250
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:PN2222A Q1
U 1 1 5FF672DC
P 8950 5550
F 0 "Q1" H 9140 5596 50  0000 L CNN
F 1 "PN2222A" H 9140 5505 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 9150 5475 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/PN2222-D.PDF" H 8950 5550 50  0001 L CNN
	1    8950 5550
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR09
U 1 1 5FF6978F
P 9050 4900
F 0 "#PWR09" H 9050 4750 50  0001 C CNN
F 1 "VCC" H 9065 5073 50  0000 C CNN
F 2 "" H 9050 4900 50  0001 C CNN
F 3 "" H 9050 4900 50  0001 C CNN
	1    9050 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 4900 9050 5150
$Comp
L power:GND #PWR010
U 1 1 5FF6C1CF
P 9050 5950
F 0 "#PWR010" H 9050 5700 50  0001 C CNN
F 1 "GND" H 9055 5777 50  0000 C CNN
F 2 "" H 9050 5950 50  0001 C CNN
F 3 "" H 9050 5950 50  0001 C CNN
	1    9050 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 5950 9050 5750
$Comp
L Device:R R1
U 1 1 5FF6D29C
P 8600 5550
F 0 "R1" V 8393 5550 50  0000 C CNN
F 1 "100R" V 8484 5550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8530 5550 50  0001 C CNN
F 3 "~" H 8600 5550 50  0001 C CNN
	1    8600 5550
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR06
U 1 1 5FF72BE7
P 4700 750
F 0 "#PWR06" H 4700 600 50  0001 C CNN
F 1 "VCC" H 4715 923 50  0000 C CNN
F 2 "" H 4700 750 50  0001 C CNN
F 3 "" H 4700 750 50  0001 C CNN
	1    4700 750 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5FF73E29
P 4700 2800
F 0 "#PWR07" H 4700 2550 50  0001 C CNN
F 1 "GND" H 4705 2627 50  0000 C CNN
F 2 "" H 4700 2800 50  0001 C CNN
F 3 "" H 4700 2800 50  0001 C CNN
	1    4700 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5FF7581C
P 3900 1150
F 0 "C2" V 3648 1150 50  0000 C CNN
F 1 "100n" V 3739 1150 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm" H 3938 1000 50  0001 C CNN
F 3 "~" H 3900 1150 50  0001 C CNN
	1    3900 1150
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5FF77801
P 3750 1150
F 0 "#PWR04" H 3750 900 50  0001 C CNN
F 1 "GND" H 3755 977 50  0000 C CNN
F 2 "" H 3750 1150 50  0001 C CNN
F 3 "" H 3750 1150 50  0001 C CNN
	1    3750 1150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5FF7EF6F
P 5950 1150
F 0 "C3" V 5698 1150 50  0000 C CNN
F 1 "100n" V 5789 1150 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm" H 5988 1000 50  0001 C CNN
F 3 "~" H 5950 1150 50  0001 C CNN
	1    5950 1150
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5FF7EF87
P 5800 1150
F 0 "#PWR08" H 5800 900 50  0001 C CNN
F 1 "GND" H 5805 977 50  0000 C CNN
F 2 "" H 5800 1150 50  0001 C CNN
F 3 "" H 5800 1150 50  0001 C CNN
	1    5800 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5FF83B50
P 8700 3850
F 0 "#PWR05" H 8700 3600 50  0001 C CNN
F 1 "GND" H 8705 3677 50  0000 C CNN
F 2 "" H 8700 3850 50  0001 C CNN
F 3 "" H 8700 3850 50  0001 C CNN
	1    8700 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 3850 8700 3700
Text GLabel 3650 1500 0    50   Input ~ 0
SR_DATA
Wire Wire Line
	6100 2600 6100 2800
Wire Wire Line
	6100 2800 5700 2800
Wire Wire Line
	3650 2100 3650 2800
$Comp
L power:VCC #PWR02
U 1 1 5FF87D73
P 1800 1000
F 0 "#PWR02" H 1800 850 50  0001 C CNN
F 1 "VCC" H 1815 1173 50  0000 C CNN
F 2 "" H 1800 1000 50  0001 C CNN
F 3 "" H 1800 1000 50  0001 C CNN
	1    1800 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5FF87D89
P 1500 1100
F 0 "C1" V 1248 1100 50  0000 C CNN
F 1 "100n" V 1339 1100 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm" H 1538 950 50  0001 C CNN
F 3 "~" H 1500 1100 50  0001 C CNN
	1    1500 1100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5FF87DA1
P 1350 1100
F 0 "#PWR01" H 1350 850 50  0001 C CNN
F 1 "GND" H 1355 927 50  0000 C CNN
F 2 "" H 1350 1100 50  0001 C CNN
F 3 "" H 1350 1100 50  0001 C CNN
	1    1350 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1000 1800 1100
Wire Wire Line
	1800 1100 1650 1100
Connection ~ 1800 1100
Wire Wire Line
	1800 1100 1800 1300
$Comp
L power:GND #PWR03
U 1 1 5FF899A2
P 1800 2500
F 0 "#PWR03" H 1800 2250 50  0001 C CNN
F 1 "GND" H 1805 2327 50  0000 C CNN
F 2 "" H 1800 2500 50  0001 C CNN
F 3 "" H 1800 2500 50  0001 C CNN
	1    1800 2500
	1    0    0    -1  
$EndComp
Text GLabel 3650 1700 0    50   Input ~ 0
SR_CLK
Wire Wire Line
	3200 1800 3650 1800
Text GLabel 5700 1700 0    50   Input ~ 0
SR_CLK
Wire Wire Line
	6100 850  6100 1150
Wire Wire Line
	5700 1500 5100 1500
Wire Wire Line
	5700 1800 5200 1800
Wire Wire Line
	5200 1800 5200 850 
Connection ~ 5200 850 
Wire Wire Line
	3200 1800 3200 850 
Wire Wire Line
	3200 850  4050 850 
Connection ~ 4050 1150
Wire Wire Line
	4050 1150 4050 1300
Wire Wire Line
	4050 850  4050 1150
Text GLabel 5700 2000 0    50   Input ~ 0
LATCH_CLK
Text GLabel 3650 2000 0    50   Input ~ 0
LATCH_CLK
Wire Wire Line
	4050 850  4700 850 
Connection ~ 4050 850 
Wire Wire Line
	5100 1500 5100 2400
Wire Wire Line
	5100 2400 4450 2400
Wire Wire Line
	4700 2800 5700 2800
Connection ~ 4700 2800
Connection ~ 5700 2800
Wire Wire Line
	5200 850  6100 850 
Wire Wire Line
	4700 750  4700 850 
Connection ~ 4700 850 
Wire Wire Line
	4700 850  5200 850 
Text GLabel 4450 1600 2    50   Input ~ 0
LED_CAT_1
Text GLabel 4450 1700 2    50   Input ~ 0
LED_CAT_2
Text GLabel 6500 1900 2    50   Input ~ 0
LED_CAT_3
Text GLabel 6500 2000 2    50   Input ~ 0
LED_CAT_4
Text GLabel 4450 1800 2    50   Input ~ 0
LED_A
Text GLabel 4450 1900 2    50   Input ~ 0
LED_B
Text GLabel 4450 2000 2    50   Input ~ 0
LED_C
Text GLabel 4450 2100 2    50   Input ~ 0
LED_D
Text GLabel 4450 2200 2    50   Input ~ 0
LED_E
Text GLabel 6500 1600 2    50   Input ~ 0
LED_F
Text GLabel 6500 1700 2    50   Input ~ 0
LED_G
Text GLabel 6500 1800 2    50   Input ~ 0
LED_DP
Wire Wire Line
	5700 2100 5700 2800
$Comp
L 74xx:74HC595 U4
U 1 1 5FF60D62
P 6100 1900
F 0 "U4" H 6300 2600 50  0000 C CNN
F 1 "74HC595" H 6300 2500 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 6100 1900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 6100 1900 50  0001 C CNN
	1    6100 1900
	1    0    0    -1  
$EndComp
NoConn ~ 6500 2100
NoConn ~ 6500 2200
NoConn ~ 6500 2400
$Comp
L Device:R R8
U 1 1 5FFA5BBB
P 1250 5000
F 0 "R8" V 1043 5000 50  0000 C CNN
F 1 "220R" V 1134 5000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1180 5000 50  0001 C CNN
F 3 "~" H 1250 5000 50  0001 C CNN
	1    1250 5000
	0    1    1    0   
$EndComp
Text GLabel 1100 5000 0    50   Input ~ 0
LED_A
Text GLabel 2400 1600 2    50   Input ~ 0
SR_DATA
Text GLabel 2400 1700 2    50   Input ~ 0
SR_CLK
Text GLabel 2400 1800 2    50   Input ~ 0
LATCH_CLK
Text GLabel 8450 5550 0    50   Input ~ 0
BUZZER_OUT
Text GLabel 4450 1500 2    50   Input ~ 0
BUZZER_OUT
Text GLabel 8300 3700 0    50   Input ~ 0
BUTTON1_IN
Text GLabel 2400 2000 2    50   Input ~ 0
BUTTON1_IN
NoConn ~ 2400 2100
NoConn ~ 2550 -100
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 5FFDBCBB
P 8000 1000
F 0 "J1" H 8080 992 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 8080 901 50  0000 L CNN
F 2 "TerminalBlock_TE-Connectivity:TerminalBlock_TE_282834-2_1x02_P2.54mm_Horizontal" H 8000 1000 50  0001 C CNN
F 3 "~" H 8000 1000 50  0001 C CNN
	1    8000 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5FFDD0E8
P 7800 1100
F 0 "#PWR012" H 7800 850 50  0001 C CNN
F 1 "GND" H 7805 927 50  0000 C CNN
F 2 "" H 7800 1100 50  0001 C CNN
F 3 "" H 7800 1100 50  0001 C CNN
	1    7800 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 1300 6100 1150
Connection ~ 6100 1150
$Comp
L power:VCC #PWR011
U 1 1 5FFDCAA2
P 7800 1000
F 0 "#PWR011" H 7800 850 50  0001 C CNN
F 1 "VCC" H 7815 1173 50  0000 C CNN
F 2 "" H 7800 1000 50  0001 C CNN
F 3 "" H 7800 1000 50  0001 C CNN
	1    7800 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2800 4050 2800
Wire Wire Line
	4050 2600 4050 2800
Connection ~ 4050 2800
Wire Wire Line
	4050 2800 4700 2800
NoConn ~ 6500 1500
$Comp
L Switch:SW_Push SW2
U 1 1 6002B372
P 9700 3700
F 0 "SW2" H 9700 3985 50  0000 C CNN
F 1 "SW_Push" H 9700 3894 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 9700 3900 50  0001 C CNN
F 3 "~" H 9700 3900 50  0001 C CNN
	1    9700 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 6002B38E
P 9900 3850
F 0 "#PWR0101" H 9900 3600 50  0001 C CNN
F 1 "GND" H 9905 3677 50  0000 C CNN
F 2 "" H 9900 3850 50  0001 C CNN
F 3 "" H 9900 3850 50  0001 C CNN
	1    9900 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3850 9900 3700
Text GLabel 9500 3700 0    50   Input ~ 0
BUTTON2_IN
Text GLabel 2400 1900 2    50   Input ~ 0
BUTTON2_IN
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5FF667DA
P 7800 2350
F 0 "H2" H 7900 2399 50  0000 L CNN
F 1 "MountingHole_Pad" H 7900 2308 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_TopBottom" H 7800 2350 50  0001 C CNN
F 3 "~" H 7800 2350 50  0001 C CNN
	1    7800 2350
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5FF66B33
P 8900 1800
F 0 "H3" H 9000 1849 50  0000 L CNN
F 1 "MountingHole_Pad" H 9000 1758 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_TopBottom" H 8900 1800 50  0001 C CNN
F 3 "~" H 8900 1800 50  0001 C CNN
	1    8900 1800
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5FF66F37
P 8900 2350
F 0 "H4" H 9000 2399 50  0000 L CNN
F 1 "MountingHole_Pad" H 9000 2308 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_TopBottom" H 8900 2350 50  0001 C CNN
F 3 "~" H 8900 2350 50  0001 C CNN
	1    8900 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5FF6726A
P 7800 1900
F 0 "#PWR013" H 7800 1650 50  0001 C CNN
F 1 "GND" H 7805 1727 50  0000 C CNN
F 2 "" H 7800 1900 50  0001 C CNN
F 3 "" H 7800 1900 50  0001 C CNN
	1    7800 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5FF675D6
P 7800 2450
F 0 "#PWR014" H 7800 2200 50  0001 C CNN
F 1 "GND" H 7805 2277 50  0000 C CNN
F 2 "" H 7800 2450 50  0001 C CNN
F 3 "" H 7800 2450 50  0001 C CNN
	1    7800 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5FF67DED
P 8900 1900
F 0 "#PWR015" H 8900 1650 50  0001 C CNN
F 1 "GND" H 8905 1727 50  0000 C CNN
F 2 "" H 8900 1900 50  0001 C CNN
F 3 "" H 8900 1900 50  0001 C CNN
	1    8900 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5FF681BF
P 8900 2450
F 0 "#PWR016" H 8900 2200 50  0001 C CNN
F 1 "GND" H 8905 2277 50  0000 C CNN
F 2 "" H 8900 2450 50  0001 C CNN
F 3 "" H 8900 2450 50  0001 C CNN
	1    8900 2450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5FF663B7
P 7800 1800
F 0 "H1" H 7900 1849 50  0000 L CNN
F 1 "MountingHole_Pad" H 7900 1758 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_TopBottom" H 7800 1800 50  0001 C CNN
F 3 "~" H 7800 1800 50  0001 C CNN
	1    7800 1800
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:LTS-6980HR AFF1
U 1 1 5FF72A37
P 3400 5400
F 0 "AFF1" H 3400 6067 50  0000 C CNN
F 1 "LTS-6980HR" H 3400 5976 50  0000 C CNN
F 2 "Display_7Segment:7SegmentLED_LTS6760_LTS6780" H 3400 4800 50  0001 C CNN
F 3 "http://datasheet.octopart.com/LTS-6960HR-Lite-On-datasheet-11803242.pdf" H 3400 5400 50  0001 C CNN
	1    3400 5400
	1    0    0    -1  
$EndComp
Text GLabel 1400 5000 2    50   Input ~ 0
LED_A_OUT
$Comp
L Device:R R2
U 1 1 5FF756F1
P 1250 5350
F 0 "R2" V 1043 5350 50  0000 C CNN
F 1 "220R" V 1134 5350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1180 5350 50  0001 C CNN
F 3 "~" H 1250 5350 50  0001 C CNN
	1    1250 5350
	0    1    1    0   
$EndComp
Text GLabel 1100 5350 0    50   Input ~ 0
LED_B
Text GLabel 1400 5350 2    50   Input ~ 0
LED_B_OUT
$Comp
L Device:R R3
U 1 1 5FF75E9B
P 1250 5700
F 0 "R3" V 1043 5700 50  0000 C CNN
F 1 "220R" V 1134 5700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1180 5700 50  0001 C CNN
F 3 "~" H 1250 5700 50  0001 C CNN
	1    1250 5700
	0    1    1    0   
$EndComp
Text GLabel 1100 5700 0    50   Input ~ 0
LED_C
Text GLabel 1400 5700 2    50   Input ~ 0
LED_C_OUT
$Comp
L Device:R R4
U 1 1 5FF766B8
P 1250 6050
F 0 "R4" V 1043 6050 50  0000 C CNN
F 1 "220R" V 1134 6050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1180 6050 50  0001 C CNN
F 3 "~" H 1250 6050 50  0001 C CNN
	1    1250 6050
	0    1    1    0   
$EndComp
Text GLabel 1100 6050 0    50   Input ~ 0
LED_D
Text GLabel 1400 6050 2    50   Input ~ 0
LED_D_OUT
$Comp
L Device:R R5
U 1 1 5FF766D0
P 1250 6400
F 0 "R5" V 1043 6400 50  0000 C CNN
F 1 "220R" V 1134 6400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1180 6400 50  0001 C CNN
F 3 "~" H 1250 6400 50  0001 C CNN
	1    1250 6400
	0    1    1    0   
$EndComp
Text GLabel 1100 6400 0    50   Input ~ 0
LED_E
Text GLabel 1400 6400 2    50   Input ~ 0
LED_E_OUT
$Comp
L Device:R R6
U 1 1 5FF766E8
P 1250 6750
F 0 "R6" V 1043 6750 50  0000 C CNN
F 1 "220R" V 1134 6750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1180 6750 50  0001 C CNN
F 3 "~" H 1250 6750 50  0001 C CNN
	1    1250 6750
	0    1    1    0   
$EndComp
Text GLabel 1100 6750 0    50   Input ~ 0
LED_F
Text GLabel 1400 6750 2    50   Input ~ 0
LED_F_OUT
$Comp
L Device:R R7
U 1 1 5FF77DD4
P 1250 7100
F 0 "R7" V 1043 7100 50  0000 C CNN
F 1 "220R" V 1134 7100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1180 7100 50  0001 C CNN
F 3 "~" H 1250 7100 50  0001 C CNN
	1    1250 7100
	0    1    1    0   
$EndComp
Text GLabel 1100 7100 0    50   Input ~ 0
LED_G
Text GLabel 1400 7100 2    50   Input ~ 0
LED_G_OUT
$Comp
L Device:R R9
U 1 1 5FF77DEC
P 1250 7450
F 0 "R9" V 1043 7450 50  0000 C CNN
F 1 "220R" V 1134 7450 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1180 7450 50  0001 C CNN
F 3 "~" H 1250 7450 50  0001 C CNN
	1    1250 7450
	0    1    1    0   
$EndComp
Text GLabel 1100 7450 0    50   Input ~ 0
LED_DP
Text GLabel 1400 7450 2    50   Input ~ 0
LED_DP_OUT
Text GLabel 2900 5000 0    50   Input ~ 0
LED_A_OUT
Text GLabel 2900 5100 0    50   Input ~ 0
LED_B_OUT
Text GLabel 2900 5200 0    50   Input ~ 0
LED_C_OUT
Text GLabel 2900 5300 0    50   Input ~ 0
LED_D_OUT
Text GLabel 2900 5400 0    50   Input ~ 0
LED_E_OUT
Text GLabel 2900 5500 0    50   Input ~ 0
LED_F_OUT
Text GLabel 2900 5600 0    50   Input ~ 0
LED_G_OUT
Text GLabel 3900 5700 2    50   Input ~ 0
LED_DP_OUT
Wire Wire Line
	3900 5000 3900 5100
Text GLabel 3900 5100 2    50   Input ~ 0
LED_CAT_1
$Comp
L Display_Character:LTS-6980HR AFF2
U 1 1 5FF81FFE
P 5700 5400
F 0 "AFF2" H 5700 6067 50  0000 C CNN
F 1 "LTS-6980HR" H 5700 5976 50  0000 C CNN
F 2 "Display_7Segment:7SegmentLED_LTS6760_LTS6780" H 5700 4800 50  0001 C CNN
F 3 "http://datasheet.octopart.com/LTS-6960HR-Lite-On-datasheet-11803242.pdf" H 5700 5400 50  0001 C CNN
	1    5700 5400
	1    0    0    -1  
$EndComp
Text GLabel 5200 5000 0    50   Input ~ 0
LED_A_OUT
Text GLabel 5200 5100 0    50   Input ~ 0
LED_B_OUT
Text GLabel 5200 5200 0    50   Input ~ 0
LED_C_OUT
Text GLabel 5200 5300 0    50   Input ~ 0
LED_D_OUT
Text GLabel 5200 5400 0    50   Input ~ 0
LED_E_OUT
Text GLabel 5200 5500 0    50   Input ~ 0
LED_F_OUT
Text GLabel 5200 5600 0    50   Input ~ 0
LED_G_OUT
Text GLabel 6200 5700 2    50   Input ~ 0
LED_DP_OUT
Wire Wire Line
	6200 5000 6200 5100
Text GLabel 6200 5100 2    50   Input ~ 0
LED_CAT_2
$Comp
L Display_Character:LTS-6980HR AFF3
U 1 1 5FF83C2C
P 3400 6950
F 0 "AFF3" H 3400 7617 50  0000 C CNN
F 1 "LTS-6980HR" H 3400 7526 50  0000 C CNN
F 2 "Display_7Segment:7SegmentLED_LTS6760_LTS6780" H 3400 6350 50  0001 C CNN
F 3 "http://datasheet.octopart.com/LTS-6960HR-Lite-On-datasheet-11803242.pdf" H 3400 6950 50  0001 C CNN
	1    3400 6950
	1    0    0    -1  
$EndComp
Text GLabel 2900 6550 0    50   Input ~ 0
LED_A_OUT
Text GLabel 2900 6650 0    50   Input ~ 0
LED_B_OUT
Text GLabel 2900 6750 0    50   Input ~ 0
LED_C_OUT
Text GLabel 2900 6850 0    50   Input ~ 0
LED_D_OUT
Text GLabel 2900 6950 0    50   Input ~ 0
LED_E_OUT
Text GLabel 2900 7050 0    50   Input ~ 0
LED_F_OUT
Text GLabel 2900 7150 0    50   Input ~ 0
LED_G_OUT
Text GLabel 3900 7250 2    50   Input ~ 0
LED_DP_OUT
Wire Wire Line
	3900 6550 3900 6650
Text GLabel 3900 6650 2    50   Input ~ 0
LED_CAT_3
$Comp
L Display_Character:LTS-6980HR AFF4
U 1 1 5FF83C62
P 5700 6950
F 0 "AFF4" H 5700 7617 50  0000 C CNN
F 1 "LTS-6980HR" H 5700 7526 50  0000 C CNN
F 2 "Display_7Segment:7SegmentLED_LTS6760_LTS6780" H 5700 6350 50  0001 C CNN
F 3 "http://datasheet.octopart.com/LTS-6960HR-Lite-On-datasheet-11803242.pdf" H 5700 6950 50  0001 C CNN
	1    5700 6950
	1    0    0    -1  
$EndComp
Text GLabel 5200 6550 0    50   Input ~ 0
LED_A_OUT
Text GLabel 5200 6650 0    50   Input ~ 0
LED_B_OUT
Text GLabel 5200 6750 0    50   Input ~ 0
LED_C_OUT
Text GLabel 5200 6850 0    50   Input ~ 0
LED_D_OUT
Text GLabel 5200 6950 0    50   Input ~ 0
LED_E_OUT
Text GLabel 5200 7050 0    50   Input ~ 0
LED_F_OUT
Text GLabel 5200 7150 0    50   Input ~ 0
LED_G_OUT
Text GLabel 6200 7250 2    50   Input ~ 0
LED_DP_OUT
Wire Wire Line
	6200 6550 6200 6650
Text GLabel 6200 6650 2    50   Input ~ 0
LED_CAT_4
$EndSCHEMATC
