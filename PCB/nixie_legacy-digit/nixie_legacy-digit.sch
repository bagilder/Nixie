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
L Device:R_US R1
U 1 1 610F2AD5
P 4650 1600
F 0 "R1" H 4718 1646 50  0000 L CNN
F 1 "20k" H 4718 1555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4690 1590 50  0001 C CNN
F 3 "~" H 4650 1600 50  0001 C CNN
	1    4650 1600
	0    1    1    0   
$EndComp
$Comp
L nixies-us:IN-12BORIGINAL NX2
U 1 1 610F3D1A
P 4100 3250
F 0 "NX2" H 4050 4095 45  0000 C CNN
F 1 "IN-12BORIGINAL" H 4100 3250 45  0001 L BNN
F 2 "nixies-us:nixies-us-IN-12-DSUB" H 4130 3400 20  0001 C CNN
F 3 "" H 4100 3250 50  0001 C CNN
	1    4100 3250
	1    0    0    -1  
$EndComp
Text GLabel 1600 1150 2    50   Input ~ 0
0
Text GLabel 1600 1050 2    50   Input ~ 0
9
Text GLabel 1600 1350 2    50   Input ~ 0
8
Text GLabel 1600 1250 2    50   Input ~ 0
7
Text GLabel 1600 1550 2    50   Input ~ 0
6
Text GLabel 1600 1450 2    50   Input ~ 0
5
Text GLabel 1600 1750 2    50   Input ~ 0
4
Text GLabel 1600 1650 2    50   Input ~ 0
3
Text GLabel 1600 1950 2    50   Input ~ 0
2
Text GLabel 1600 1850 2    50   Input ~ 0
1
Text GLabel 1600 2150 2    50   Input ~ 0
digit1msb
Text GLabel 1600 2050 2    50   Input ~ 0
digit2
Text GLabel 1600 2350 2    50   Input ~ 0
digit3
Text GLabel 1600 2250 2    50   Input ~ 0
digit4
Text GLabel 1600 2550 2    50   Input ~ 0
digit5
Text GLabel 1600 2450 2    50   Input ~ 0
digit6lsb
$Comp
L Device:R_US R2
U 1 1 61113E51
P 4650 3250
F 0 "R2" H 4718 3296 50  0000 L CNN
F 1 "20k" H 4718 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4690 3240 50  0001 C CNN
F 3 "~" H 4650 3250 50  0001 C CNN
	1    4650 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 2750 3550 2750
Wire Wire Line
	3550 1100 3600 1100
Wire Wire Line
	3600 1200 3500 1200
Wire Wire Line
	3500 2850 3600 2850
Wire Wire Line
	3600 2950 3450 2950
Wire Wire Line
	3450 1300 3600 1300
Wire Wire Line
	3600 1400 3400 1400
Wire Wire Line
	3400 3050 3600 3050
Wire Wire Line
	3600 3150 3350 3150
Wire Wire Line
	3350 1500 3600 1500
Wire Wire Line
	3600 1600 3300 1600
Wire Wire Line
	3300 3250 3600 3250
Wire Wire Line
	3600 3350 3250 3350
Wire Wire Line
	3250 1700 3600 1700
Wire Wire Line
	3600 1800 3200 1800
Wire Wire Line
	3200 3450 3600 3450
Wire Wire Line
	3600 3550 3150 3550
Wire Wire Line
	3150 1900 3600 1900
Wire Wire Line
	3100 3650 3600 3650
Wire Wire Line
	1600 1850 3550 1850
Wire Wire Line
	1600 1950 3500 1950
Wire Wire Line
	1600 1650 3450 1650
Wire Wire Line
	1600 1750 3400 1750
Wire Wire Line
	1600 1550 3300 1550
Wire Wire Line
	1600 1350 3200 1350
Wire Wire Line
	3100 2000 3100 3650
Connection ~ 3100 2000
Wire Wire Line
	3600 2000 3100 2000
Connection ~ 3150 1900
Wire Wire Line
	3150 3550 3150 1900
Connection ~ 3200 1800
Wire Wire Line
	3200 1800 3200 3450
Connection ~ 3250 1700
Wire Wire Line
	3250 3350 3250 1700
Connection ~ 3300 1600
Wire Wire Line
	3300 1600 3300 3250
$Comp
L nixies-us:IN-12BORIGINAL NX1
U 1 1 610F3540
P 4100 1600
F 0 "NX1" H 4050 2445 45  0000 C CNN
F 1 "IN-12BORIGINAL" H 4100 1600 45  0001 L BNN
F 2 "nixies-us:nixies-us-IN-12-DSUB" H 4130 1750 20  0001 C CNN
F 3 "" H 4100 1600 50  0001 C CNN
	1    4100 1600
	1    0    0    -1  
$EndComp
$Comp
L w_connectors:HEADER_16 J1
U 1 1 6166124A
P 1500 1800
F 0 "J1" H 1492 2787 60  0000 C CNN
F 1 "HEADER_16" H 1492 2681 60  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x08_P2.54mm_Vertical" H 1500 1800 60  0001 C CNN
F 3 "" H 1500 1800 60  0000 C CNN
	1    1500 1800
	-1   0    0    -1  
$EndComp
$Comp
L w_connectors:HEADER_1 min10
U 1 1 616638CE
P 2300 2350
F 0 "min10" H 2428 2398 60  0000 L CNN
F 1 "HEADER_1" H 2650 2350 60  0000 L CNN
F 2 "customPartsLibrary:Single_pad-1x1" H 2300 2350 60  0001 C CNN
F 3 "" H 2300 2350 60  0000 C CNN
	1    2300 2350
	1    0    0    -1  
$EndComp
$Comp
L w_connectors:HEADER_1 min0
U 1 1 61666A78
P 2300 2250
F 0 "min0" H 2428 2298 60  0000 L CNN
F 1 "HEADER_1" H 2650 2250 60  0000 L CNN
F 2 "customPartsLibrary:Single_pad-1x1" H 2300 2250 60  0001 C CNN
F 3 "" H 2300 2250 60  0000 C CNN
	1    2300 2250
	1    0    0    -1  
$EndComp
$Comp
L w_connectors:HEADER_1 sec10
U 1 1 61667A7E
P 2300 2550
F 0 "sec10" H 2428 2598 60  0000 L CNN
F 1 "HEADER_1" H 2650 2550 60  0000 L CNN
F 2 "customPartsLibrary:Single_pad-1x1" H 2300 2550 60  0001 C CNN
F 3 "" H 2300 2550 60  0000 C CNN
	1    2300 2550
	1    0    0    -1  
$EndComp
$Comp
L w_connectors:HEADER_1 sec0
U 1 1 61667A88
P 2300 2450
F 0 "sec0" H 2428 2498 60  0000 L CNN
F 1 "HEADER_1" H 2650 2450 60  0000 L CNN
F 2 "customPartsLibrary:Single_pad-1x1" H 2300 2450 60  0001 C CNN
F 3 "" H 2300 2450 60  0000 C CNN
	1    2300 2450
	1    0    0    -1  
$EndComp
$Comp
L w_connectors:HEADER_1 hr10
U 1 1 6166A207
P 2300 2150
F 0 "hr10" H 2428 2198 60  0000 L CNN
F 1 "HEADER_1" H 2650 2150 60  0000 L CNN
F 2 "customPartsLibrary:Single_pad-1x1" H 2300 2150 60  0001 C CNN
F 3 "" H 2300 2150 60  0000 C CNN
	1    2300 2150
	1    0    0    -1  
$EndComp
$Comp
L w_connectors:HEADER_1 hr0
U 1 1 6166A211
P 2300 2050
F 0 "hr0" H 2428 2098 60  0000 L CNN
F 1 "HEADER_1" H 2650 2050 60  0000 L CNN
F 2 "customPartsLibrary:Single_pad-1x1" H 2300 2050 60  0001 C CNN
F 3 "" H 2300 2050 60  0000 C CNN
	1    2300 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 2150 1600 2150
Wire Wire Line
	2200 2050 1600 2050
Wire Wire Line
	2200 2350 1600 2350
Wire Wire Line
	2200 2250 1600 2250
Wire Wire Line
	2200 2550 1600 2550
Wire Wire Line
	2200 2450 1600 2450
$Comp
L w_connectors:HEADER_1 dp1
U 1 1 61696992
P 4600 1100
F 0 "dp1" H 4728 1148 60  0000 L CNN
F 1 "HEADER_1" H 4728 1042 60  0000 L CNN
F 2 "customPartsLibrary:Single_pad-1x1" H 4600 1100 60  0001 C CNN
F 3 "" H 4600 1100 60  0000 C CNN
	1    4600 1100
	1    0    0    -1  
$EndComp
NoConn ~ 4500 2750
Wire Wire Line
	3100 2000 3100 1150
Wire Wire Line
	1600 1050 3150 1050
Wire Wire Line
	3100 1150 1600 1150
Wire Wire Line
	3150 1050 3150 1900
Wire Wire Line
	3350 1500 3350 3150
Wire Wire Line
	1600 1450 3350 1450
Wire Wire Line
	3350 1450 3350 1500
Connection ~ 3350 1500
Wire Wire Line
	3400 1400 3400 1750
Connection ~ 3450 1650
Wire Wire Line
	3450 1650 3450 1300
Wire Wire Line
	3450 1650 3450 2950
Connection ~ 3400 1750
Wire Wire Line
	3400 1750 3400 3050
Wire Wire Line
	3500 1200 3500 1950
Connection ~ 3550 1850
Wire Wire Line
	3550 1850 3550 1100
Wire Wire Line
	3550 1850 3550 2750
Connection ~ 3500 1950
Wire Wire Line
	3500 1950 3500 2850
Wire Wire Line
	1600 1250 3250 1250
Wire Wire Line
	3250 1250 3250 1700
Wire Wire Line
	3200 1350 3200 1800
Wire Wire Line
	3300 1550 3300 1600
$EndSCHEMATC
