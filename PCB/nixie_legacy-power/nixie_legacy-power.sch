EESchema Schematic File Version 4
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "nixie power board 9v"
Date ""
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR0101
U 1 1 60FB4F49
P 2100 5600
F 0 "#PWR0101" H 2100 5350 50  0001 C CNN
F 1 "GND" H 2105 5427 50  0000 C CNN
F 2 "" H 2100 5600 50  0001 C CNN
F 3 "" H 2100 5600 50  0001 C CNN
	1    2100 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT_US RV1
U 1 1 60FBA6CA
P 9400 4600
F 0 "RV1" H 9332 4646 50  0000 R CNN
F 1 "1k" H 9332 4555 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_3386P_Vertical" H 9400 4600 50  0001 C CNN
F 3 "~" H 9400 4600 50  0001 C CNN
	1    9400 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C1
U 1 1 60FBB5AF
P 2850 3500
F 0 "C1" H 2965 3546 50  0000 L CNN
F 1 "470u" H 2965 3455 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 2850 3500 50  0001 C CNN
F 3 "~" H 2850 3500 50  0001 C CNN
	1    2850 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C4
U 1 1 60FBBE3F
P 8550 4550
F 0 "C4" H 8665 4596 50  0000 L CNN
F 1 "1u" H 8665 4505 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 8550 4550 50  0001 C CNN
F 3 "~" H 8550 4550 50  0001 C CNN
	1    8550 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 60FBC665
P 4350 3350
F 0 "C2" H 4465 3396 50  0000 L CNN
F 1 "2.2n" H 4465 3305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D12.5mm_W5.0mm_P10.00mm" H 4388 3200 50  0001 C CNN
F 3 "~" H 4350 3350 50  0001 C CNN
	1    4350 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 60FBEAA3
P 7850 3100
F 0 "L1" H 7902 3146 50  0000 L CNN
F 1 "100u" H 7902 3055 50  0000 L CNN
F 2 "Inductor_THT:L_Radial_D8.7mm_P5.00mm_Fastron_07HCP" H 7850 3100 50  0001 C CNN
F 3 "~" H 7850 3100 50  0001 C CNN
	1    7850 3100
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-Bipolar-BJT-Single:MPSA56-AP Q2
U 1 1 60FC0AB1
P 6300 4500
F 0 "Q2" H 6488 4553 60  0000 L CNN
F 1 "MPSA56-AP" H 6488 4447 60  0000 L CNN
F 2 "digikey-footprints:TO-92-3_Formed_Leads" H 6500 4700 60  0001 L CNN
F 3 "http://www.mccsemi.com/up_pdf/MPSA55-MPSA56(TO-92).pdf" H 6500 4800 60  0001 L CNN
F 4 "MPSA56-APMSCT-ND" H 6500 4900 60  0001 L CNN "Digi-Key_PN"
F 5 "MPSA56-AP" H 6500 5000 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 6500 5100 60  0001 L CNN "Category"
F 7 "Transistors - Bipolar (BJT) - Single" H 6500 5200 60  0001 L CNN "Family"
F 8 "http://www.mccsemi.com/up_pdf/MPSA55-MPSA56(TO-92).pdf" H 6500 5300 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/micro-commercial-co/MPSA56-AP/MPSA56-APMSCT-ND/2233955" H 6500 5400 60  0001 L CNN "DK_Detail_Page"
F 10 "TRANS PNP 80V 0.5A TO-92" H 6500 5500 60  0001 L CNN "Description"
F 11 "Micro Commercial Co" H 6500 5600 60  0001 L CNN "Manufacturer"
F 12 "Active" H 6500 5700 60  0001 L CNN "Status"
	1    6300 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:D D2
U 1 1 60FC1717
P 8100 3850
F 0 "D2" H 8100 3634 50  0000 C CNN
F 1 "D" H 8100 3725 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P7.62mm_Horizontal" H 8100 3850 50  0001 C CNN
F 3 "~" H 8100 3850 50  0001 C CNN
	1    8100 3850
	-1   0    0    1   
$EndComp
$Comp
L Device:D_ALT D1
U 1 1 60FC2A66
P 6400 3500
F 0 "D1" V 6446 3421 50  0000 R CNN
F 1 "D_ALT" V 6355 3421 50  0000 R CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 6400 3500 50  0001 C CNN
F 3 "~" H 6400 3500 50  0001 C CNN
	1    6400 3500
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R3
U 1 1 60FC2F6C
P 5500 2100
F 0 "R3" H 5432 2054 50  0000 R CNN
F 1 "1k" H 5432 2145 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5540 2090 50  0001 C CNN
F 3 "~" H 5500 2100 50  0001 C CNN
	1    5500 2100
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R8
U 1 1 60FC3A8B
P 9400 5100
F 0 "R8" H 9468 5146 50  0000 L CNN
F 1 "470" H 9468 5055 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 9440 5090 50  0001 C CNN
F 3 "~" H 9400 5100 50  0001 C CNN
	1    9400 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R7
U 1 1 60FC4A7C
P 9400 4100
F 0 "R7" H 9332 4054 50  0000 R CNN
F 1 "220k" H 9332 4145 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 9440 4090 50  0001 C CNN
F 3 "~" H 9400 4100 50  0001 C CNN
	1    9400 4100
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R2
U 1 1 60FC5014
P 4850 2250
F 0 "R2" V 5055 2250 50  0000 C CNN
F 1 "10k" V 4964 2250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4890 2240 50  0001 C CNN
F 3 "~" H 4850 2250 50  0001 C CNN
	1    4850 2250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R6
U 1 1 60FC5028
P 9400 3600
F 0 "R6" H 9468 3646 50  0000 L CNN
F 1 "4.7k" H 9468 3555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 9440 3590 50  0001 C CNN
F 3 "~" H 9400 3600 50  0001 C CNN
	1    9400 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R5
U 1 1 60FC5F0A
P 7850 4150
F 0 "R5" H 7918 4196 50  0000 L CNN
F 1 "1k" H 7918 4105 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7890 4140 50  0001 C CNN
F 3 "~" H 7850 4150 50  0001 C CNN
	1    7850 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R4
U 1 1 60FC5F14
P 5500 3350
F 0 "R4" H 5568 3396 50  0000 L CNN
F 1 "1k" H 5568 3305 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5540 3340 50  0001 C CNN
F 3 "~" H 5500 3350 50  0001 C CNN
	1    5500 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 2900 4350 3000
Wire Wire Line
	4350 3000 4400 3000
Wire Wire Line
	4400 2800 4400 2600
Wire Wire Line
	4400 2600 4900 2600
Connection ~ 4400 2600
Wire Wire Line
	5400 2900 5500 2900
Wire Wire Line
	4250 2250 4250 2900
Wire Wire Line
	4250 2900 4350 2900
Connection ~ 4350 2900
Wire Wire Line
	4350 2900 4400 2900
Wire Wire Line
	4400 2600 3950 2600
$Comp
L Device:C C3
U 1 1 60FCF88A
P 7850 4550
F 0 "C3" H 7965 4596 50  0000 L CNN
F 1 "47p" H 7965 4505 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.4mm_W2.1mm_P2.50mm" H 7888 4400 50  0001 C CNN
F 3 "~" H 7850 4550 50  0001 C CNN
	1    7850 4550
	1    0    0    -1  
$EndComp
Connection ~ 4350 3000
Connection ~ 4900 3600
Wire Wire Line
	4900 3600 4900 3300
Wire Wire Line
	4350 3500 4350 3600
$Comp
L Device:R_US R1
U 1 1 60FC501E
P 3950 2850
F 0 "R1" H 4018 2896 50  0000 L CNN
F 1 "33k" H 4018 2805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3990 2840 50  0001 C CNN
F 3 "~" H 3950 2850 50  0001 C CNN
	1    3950 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3600 3950 3600
Wire Wire Line
	3950 3600 3950 3550
Connection ~ 4350 3600
Wire Wire Line
	4250 2250 4700 2250
Wire Wire Line
	9550 4600 9950 4600
Wire Wire Line
	5000 2250 5500 2250
Wire Wire Line
	5500 2250 5500 2900
Connection ~ 5500 2250
Wire Wire Line
	3950 2700 3950 2600
Wire Wire Line
	7950 3850 7850 3850
Wire Wire Line
	7850 3850 7850 4000
Wire Wire Line
	7850 4700 7850 5500
Connection ~ 7850 5500
Connection ~ 7850 3850
Wire Wire Line
	6400 3350 6400 3000
Wire Wire Line
	4350 3600 4900 3600
Wire Wire Line
	4900 3600 5500 3600
Wire Wire Line
	5500 3600 5500 3500
Wire Wire Line
	9400 5250 9400 5500
Wire Wire Line
	9400 3850 9400 3950
Wire Wire Line
	8550 4400 8550 3850
Wire Wire Line
	8250 3850 8550 3850
Wire Wire Line
	8550 3850 9400 3850
Wire Wire Line
	8550 4700 8550 5500
Wire Wire Line
	7850 5500 8550 5500
Connection ~ 8550 5500
Connection ~ 9400 3850
Text GLabel 9550 3300 2    50   Input ~ 0
output
Wire Wire Line
	9400 3300 9550 3300
Wire Wire Line
	9400 3450 9400 3300
Wire Wire Line
	9400 3750 9400 3850
Connection ~ 5500 3000
Wire Wire Line
	5500 3000 5500 3200
Wire Wire Line
	2850 5500 2850 3650
Wire Wire Line
	4900 3600 4900 5500
Connection ~ 4900 5500
Wire Wire Line
	9950 5950 3550 5950
Wire Wire Line
	3550 5950 3550 3350
Wire Wire Line
	5400 3000 5500 3000
$Comp
L dk_Clock-Timing-Programmable-Timers-and-Oscillators:NE555P U1
U 1 1 60FB8E4F
P 4900 2900
F 0 "U1" H 4900 3403 60  0000 C CNN
F 1 "NE555P" H 4900 3297 60  0000 C CNN
F 2 "digikey-footprints:DIP-8_W7.62mm" H 5100 3100 60  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/ne555.pdf" H 5100 3200 60  0001 L CNN
F 4 "296-1411-5-ND" H 5100 3300 60  0001 L CNN "Digi-Key_PN"
F 5 "NE555P" H 5100 3400 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 5100 3500 60  0001 L CNN "Category"
F 7 "Clock/Timing - Programmable Timers and Oscillators" H 5100 3600 60  0001 L CNN "Family"
F 8 "http://www.ti.com/lit/ds/symlink/ne555.pdf" H 5100 3700 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/texas-instruments/NE555P/296-1411-5-ND/277057" H 5100 3800 60  0001 L CNN "DK_Detail_Page"
F 10 "IC OSC SINGLE TIMER 100KHZ 8-DIP" H 5100 3900 60  0001 L CNN "Description"
F 11 "Texas Instruments" H 5100 4000 60  0001 L CNN "Manufacturer"
F 12 "Active" H 5100 4100 60  0001 L CNN "Status"
	1    4900 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4500 6100 4500
Wire Wire Line
	7850 4300 7850 4400
Connection ~ 8550 3850
Wire Wire Line
	9400 4750 9400 4950
Wire Wire Line
	9950 4600 9950 5950
Wire Wire Line
	9400 4250 9400 4450
Wire Wire Line
	2100 5500 2850 5500
Connection ~ 2850 5500
Wire Wire Line
	2850 5500 4900 5500
Wire Wire Line
	4900 5500 6400 5500
Connection ~ 6400 5500
Connection ~ 7200 5500
Wire Wire Line
	7200 5500 7850 5500
Wire Wire Line
	6400 5500 7200 5500
Wire Wire Line
	2100 5500 2100 5600
Connection ~ 2100 5500
Wire Wire Line
	8550 5500 9400 5500
Wire Wire Line
	7200 4350 7200 5500
Wire Wire Line
	6400 4150 6400 4300
Connection ~ 6400 4150
Wire Wire Line
	6900 4150 6400 4150
Wire Wire Line
	7200 3850 7850 3850
$Comp
L Transistor_FET:IRF740 Q3
U 1 1 60FBEFF3
P 7100 4150
F 0 "Q3" H 7306 4196 50  0000 L CNN
F 1 "IRF740" H 7306 4105 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 7350 4075 50  0001 L CIN
F 3 "http://www.vishay.com/docs/91054/91054.pdf" H 7100 4150 50  0001 L CNN
	1    7100 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3000 4350 3200
Wire Wire Line
	3550 3350 3650 3350
Wire Wire Line
	3950 3000 3950 3100
Connection ~ 3950 3100
Wire Wire Line
	3950 3150 3950 3100
$Comp
L Transistor_BJT:BC547 Q1
U 1 1 60FC0026
P 3850 3350
F 0 "Q1" H 4041 3396 50  0000 L CNN
F 1 "BC547" H 4041 3305 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 4050 3275 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 3850 3350 50  0001 L CNN
	1    3850 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3100 3950 3100
Wire Wire Line
	6400 4700 6400 5500
Wire Wire Line
	5500 3000 6000 3000
Wire Wire Line
	7850 3250 7850 3850
Wire Wire Line
	7200 3950 7200 3850
Wire Wire Line
	6400 3650 6400 4150
Wire Wire Line
	6000 3000 6000 4500
Connection ~ 6000 3000
Wire Wire Line
	6000 3000 6400 3000
$Comp
L Device:Polyfuse F1
U 1 1 60FB616D
P 2450 1700
F 0 "F1" V 2675 1700 50  0000 C CNN
F 1 "Polyfuse" V 2584 1700 50  0000 C CNN
F 2 "Fuse:Fuse_Bourns_MF-RG300" H 2500 1500 50  0001 L CNN
F 3 "~" H 2450 1700 50  0001 C CNN
	1    2450 1700
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Barrel_Jack J1
U 1 1 610FC31E
P 1800 1800
F 0 "J1" H 1857 2125 50  0000 C CNN
F 1 "Barrel_Jack" H 1857 2034 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1850 1760 50  0001 C CNN
F 3 "~" H 1850 1760 50  0001 C CNN
	1    1800 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1700 2300 1700
$Comp
L w_connectors:HEADER_3 J2
U 1 1 6111F8B8
P 9550 1800
F 0 "J2" H 9678 1853 60  0000 L CNN
F 1 "HEADER_3" H 9678 1747 60  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 9550 1800 60  0001 C CNN
F 3 "" H 9550 1800 60  0000 C CNN
	1    9550 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 6112FFC6
P 9200 1900
F 0 "#PWR0102" H 9200 1650 50  0001 C CNN
F 1 "GND" H 9205 1727 50  0000 C CNN
F 2 "" H 9200 1900 50  0001 C CNN
F 3 "" H 9200 1900 50  0001 C CNN
	1    9200 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 1900 9400 1900
Wire Wire Line
	9200 1900 9200 1800
Wire Wire Line
	9200 1800 9450 1800
Connection ~ 9450 1700
Wire Wire Line
	9450 1450 9450 1700
Text GLabel 9450 1450 1    50   Input ~ 0
9v
Wire Wire Line
	2600 1700 2850 1700
Wire Wire Line
	2100 1900 2100 5500
Wire Wire Line
	3950 2600 3950 1700
Connection ~ 3950 2600
Connection ~ 3950 1700
Wire Wire Line
	3950 1700 5500 1700
Wire Wire Line
	2850 3350 2850 1700
Connection ~ 2850 1700
Wire Wire Line
	2850 1700 3950 1700
Wire Wire Line
	9400 3300 9400 1900
Connection ~ 9400 3300
Wire Wire Line
	7850 2950 7850 1700
Connection ~ 7850 1700
Wire Wire Line
	7850 1700 9450 1700
Wire Wire Line
	5500 1950 5500 1700
Connection ~ 5500 1700
Wire Wire Line
	5500 1700 7850 1700
$EndSCHEMATC
