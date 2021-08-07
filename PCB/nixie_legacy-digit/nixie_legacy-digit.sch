EESchema Schematic File Version 4
LIBS:nixie_legacy-digit-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Nixie Legacy - digit"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L w_connectors:HEADER_18 J?
U 1 1 610F0530
P 1500 1900
F 0 "J?" H 1492 2987 60  0000 C CNN
F 1 "HEADER_18" H 1492 2881 60  0000 C CNN
F 2 "" H 1500 1900 60  0000 C CNN
F 3 "" H 1500 1900 60  0000 C CNN
	1    1500 1900
	-1   0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MPSA92 Q?
U 1 1 610F21C8
P 4700 3050
F 0 "Q?" H 4891 3096 50  0000 L CNN
F 1 "20k" H 4891 3005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 4900 2975 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/MPSA92-D.PDF" H 4700 3050 50  0001 L CNN
	1    4700 3050
	1    0    0    1   
$EndComp
$Comp
L Device:R_US R?
U 1 1 610F2AD5
P 4650 1600
F 0 "R?" H 4718 1646 50  0000 L CNN
F 1 "R_US" H 4718 1555 50  0000 L CNN
F 2 "" V 4690 1590 50  0001 C CNN
F 3 "~" H 4650 1600 50  0001 C CNN
	1    4650 1600
	0    1    1    0   
$EndComp
$Comp
L nixies-us:IN-12BORIGINAL NX?
U 1 1 610F3540
P 4100 1600
F 0 "NX?" H 4050 2445 45  0000 C CNN
F 1 "IN-12BORIGINAL" H 4100 1600 45  0001 L BNN
F 2 "nixies-us_IN-12" H 4130 1750 20  0001 C CNN
F 3 "" H 4100 1600 50  0001 C CNN
	1    4100 1600
	1    0    0    -1  
$EndComp
$Comp
L nixies-us:IN-12BORIGINAL NX?
U 1 1 610F3D1A
P 4100 3250
F 0 "NX?" H 4050 4095 45  0000 C CNN
F 1 "IN-12BORIGINAL" H 4100 3250 45  0001 L BNN
F 2 "nixies-us_IN-12" H 4130 3400 20  0001 C CNN
F 3 "" H 4100 3250 50  0001 C CNN
	1    4100 3250
	1    0    0    -1  
$EndComp
Text GLabel 1600 1050 2    50   Input ~ 0
0
Text GLabel 1600 1150 2    50   Input ~ 0
9
Text GLabel 1600 1250 2    50   Input ~ 0
8
Text GLabel 1600 1350 2    50   Input ~ 0
7
Text GLabel 1600 1450 2    50   Input ~ 0
6
Text GLabel 1600 1550 2    50   Input ~ 0
5
Text GLabel 1600 1650 2    50   Input ~ 0
4
Text GLabel 1600 1750 2    50   Input ~ 0
3
Text GLabel 1600 1850 2    50   Input ~ 0
2
Text GLabel 1600 1950 2    50   Input ~ 0
1
Text GLabel 1600 2150 2    50   Input ~ 0
200v
Text GLabel 1600 2250 2    50   Input ~ 0
digit1msb
Text GLabel 1600 2350 2    50   Input ~ 0
digit2
Text GLabel 1600 2450 2    50   Input ~ 0
digit3
Text GLabel 1600 2550 2    50   Input ~ 0
digit4
Text GLabel 1600 2650 2    50   Input ~ 0
digit5
Text GLabel 1600 2750 2    50   Input ~ 0
digit6lsb
$Comp
L power:GND #PWR?
U 1 1 61111493
P 1900 2050
F 0 "#PWR?" H 1900 1800 50  0001 C CNN
F 1 "GND" H 1905 1877 50  0000 C CNN
F 2 "" H 1900 2050 50  0001 C CNN
F 3 "" H 1900 2050 50  0001 C CNN
	1    1900 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 2050 1600 2050
$Comp
L Transistor_BJT:MPSA92 Q?
U 1 1 61113E47
P 4700 1400
F 0 "Q?" H 4891 1446 50  0000 L CNN
F 1 "20k" H 4891 1355 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 4900 1325 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/MPSA92-D.PDF" H 4700 1400 50  0001 L CNN
	1    4700 1400
	1    0    0    1   
$EndComp
$Comp
L Device:R_US R?
U 1 1 61113E51
P 4650 3250
F 0 "R?" H 4718 3296 50  0000 L CNN
F 1 "R_US" H 4718 3205 50  0000 L CNN
F 2 "" V 4690 3240 50  0001 C CNN
F 3 "~" H 4650 3250 50  0001 C CNN
	1    4650 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 2750 3550 2750
Wire Wire Line
	3550 2750 3550 1950
Wire Wire Line
	3550 1100 3600 1100
Wire Wire Line
	3600 1200 3500 1200
Wire Wire Line
	3500 1200 3500 1850
Wire Wire Line
	3500 2850 3600 2850
Wire Wire Line
	3600 2950 3450 2950
Wire Wire Line
	3450 2950 3450 1750
Wire Wire Line
	3450 1300 3600 1300
Wire Wire Line
	3600 1400 3400 1400
Wire Wire Line
	3400 1400 3400 1650
Wire Wire Line
	3400 3050 3600 3050
Wire Wire Line
	3600 3150 3350 3150
Wire Wire Line
	3350 3150 3350 1550
Wire Wire Line
	3350 1500 3600 1500
Wire Wire Line
	3600 1600 3300 1600
Wire Wire Line
	3300 1600 3300 3250
Wire Wire Line
	3300 3250 3600 3250
Wire Wire Line
	3600 3350 3250 3350
Wire Wire Line
	3250 3350 3250 1700
Wire Wire Line
	3250 1700 3600 1700
Wire Wire Line
	3600 1800 3200 1800
Wire Wire Line
	3200 1800 3200 3450
Wire Wire Line
	3200 3450 3600 3450
Wire Wire Line
	3600 3550 3150 3550
Wire Wire Line
	3150 3550 3150 1900
Wire Wire Line
	3150 1900 3600 1900
Wire Wire Line
	3600 2000 3100 2000
Wire Wire Line
	3100 2000 3100 3650
Wire Wire Line
	3100 3650 3600 3650
Wire Wire Line
	1600 1950 3550 1950
Connection ~ 3550 1950
Wire Wire Line
	3550 1950 3550 1100
Wire Wire Line
	1600 1850 3500 1850
Connection ~ 3500 1850
Wire Wire Line
	3500 1850 3500 2850
Wire Wire Line
	1600 1750 3450 1750
Connection ~ 3450 1750
Wire Wire Line
	3450 1750 3450 1300
Wire Wire Line
	1600 1650 3400 1650
Connection ~ 3400 1650
Wire Wire Line
	3400 1650 3400 3050
Wire Wire Line
	1600 1550 3350 1550
Connection ~ 3350 1550
Wire Wire Line
	3350 1550 3350 1500
Wire Wire Line
	3300 1450 3300 1600
Wire Wire Line
	1600 1450 3300 1450
Connection ~ 3300 1600
Wire Wire Line
	3250 1350 3250 1700
Wire Wire Line
	1600 1350 3250 1350
Connection ~ 3250 1700
Wire Wire Line
	3200 1250 3200 1800
Wire Wire Line
	1600 1250 3200 1250
Connection ~ 3200 1800
Wire Wire Line
	3150 1150 3150 1900
Wire Wire Line
	1600 1150 3150 1150
Connection ~ 3150 1900
Wire Wire Line
	3100 2000 3100 1050
Wire Wire Line
	3100 1050 1600 1050
Connection ~ 3100 2000
$EndSCHEMATC
