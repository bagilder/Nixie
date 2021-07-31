EESchema Schematic File Version 4
LIBS:nixie_legacy-demux-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Nixie legacy demultiplex & control"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L w_connectors:HEADER_3 J1
U 1 1 60FCBFD4
P 900 1050
F 0 "J1" H 892 1387 60  0000 C CNN
F 1 "HEADER_3" H 892 1281 60  0000 C CNN
F 2 "w_pin_strip:pin_socket_3" H 900 1050 60  0001 C CNN
F 3 "" H 900 1050 60  0000 C CNN
	1    900  1050
	-1   0    0    -1  
$EndComp
Text GLabel 1300 800  1    50   Input ~ 0
9v
Text GLabel 1300 1250 3    50   Input ~ 0
200v
$Comp
L power:GND #PWR0101
U 1 1 60FCCF70
P 1100 1300
F 0 "#PWR0101" H 1100 1050 50  0001 C CNN
F 1 "GND" H 1105 1127 50  0000 C CNN
F 2 "" H 1100 1300 50  0001 C CNN
F 3 "" H 1100 1300 50  0001 C CNN
	1    1100 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 1050 1100 1050
Wire Wire Line
	1100 1050 1100 1300
Wire Wire Line
	1000 950  1300 950 
Wire Wire Line
	1300 950  1300 800 
Wire Wire Line
	1000 1150 1300 1150
Wire Wire Line
	1300 1150 1300 1250
$Comp
L dk_PMIC-Voltage-Regulators-Linear:MC7805CT-BP U2
U 1 1 60FCE506
P 2200 950
F 0 "U2" H 2200 1237 60  0000 C CNN
F 1 "MC7805CT-BP" H 2200 1131 60  0000 C CNN
F 2 "digikey-footprints:TO-220-3" H 2400 1150 60  0001 L CNN
F 3 "http://www.mccsemi.com/up_pdf/MC7805CT(TO-220).pdf" H 2400 1250 60  0001 L CNN
F 4 "MC7805CT-BPMS-ND" H 2400 1350 60  0001 L CNN "Digi-Key_PN"
F 5 "MC7805CT-BP" H 2400 1450 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 2400 1550 60  0001 L CNN "Category"
F 7 "PMIC - Voltage Regulators - Linear" H 2400 1650 60  0001 L CNN "Family"
F 8 "http://www.mccsemi.com/up_pdf/MC7805CT(TO-220).pdf" H 2400 1750 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/micro-commercial-co/MC7805CT-BP/MC7805CT-BPMS-ND/804682" H 2400 1850 60  0001 L CNN "DK_Detail_Page"
F 10 "IC REG LINEAR 5V 1.5A TO220" H 2400 1950 60  0001 L CNN "Description"
F 11 "Micro Commercial Co" H 2400 2050 60  0001 L CNN "Manufacturer"
F 12 "Active" H 2400 2150 60  0001 L CNN "Status"
	1    2200 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C1
U 1 1 60FD03B9
P 1800 1250
F 0 "C1" H 1915 1296 50  0000 L CNN
F 1 "220nTANT" H 1915 1205 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.8mm_W2.6mm_P2.50mm" H 1800 1250 50  0001 C CNN
F 3 "~" H 1800 1250 50  0001 C CNN
	1    1800 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R1
U 1 1 60FD139D
P 2100 3150
F 0 "R1" H 2168 3196 50  0000 L CNN
F 1 "20k" H 2168 3105 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2140 3140 50  0001 C CNN
F 3 "~" H 2100 3150 50  0001 C CNN
	1    2100 3150
	1    0    0    -1  
$EndComp
$Comp
L dk_Embedded-Microcontrollers:ATMEGA328P-PU U1
U 1 1 60FD1F49
P 1550 4400
F 0 "U1" H 1828 4403 60  0000 L CNN
F 1 "ATMEGA328P-PU" H 1828 4297 60  0000 L CNN
F 2 "Package_DIP:DIP-28_W7.62mm_LongPads" H 1750 4600 60  0001 L CNN
F 3 "http://www.microchip.com/mymicrochip/filehandler.aspx?ddocname=en589968" H 1750 4700 60  0001 L CNN
F 4 "ATMEGA328P-PU-ND" H 1750 4800 60  0001 L CNN "Digi-Key_PN"
F 5 "ATMEGA328P-PU" H 1750 4900 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 1750 5000 60  0001 L CNN "Category"
F 7 "Embedded - Microcontrollers" H 1750 5100 60  0001 L CNN "Family"
F 8 "http://www.microchip.com/mymicrochip/filehandler.aspx?ddocname=en589968" H 1750 5200 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/microchip-technology/ATMEGA328P-PU/ATMEGA328P-PU-ND/1914589" H 1750 5300 60  0001 L CNN "DK_Detail_Page"
F 10 "IC MCU 8BIT 32KB FLASH 28DIP" H 1750 5400 60  0001 L CNN "Description"
F 11 "Microchip Technology" H 1750 5500 60  0001 L CNN "Manufacturer"
F 12 "Active" H 1750 5600 60  0001 L CNN "Status"
	1    1550 4400
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 60FD8E03
P 1800 2350
F 0 "C2" H 1915 2396 50  0000 L CNN
F 1 "22pF" H 1915 2305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 1838 2200 50  0001 C CNN
F 3 "~" H 1800 2350 50  0001 C CNN
	1    1800 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 60FDBA25
P 5750 7000
F 0 "R2" V 5545 7000 50  0000 C CNN
F 1 "470k" V 5636 7000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5790 6990 50  0001 C CNN
F 3 "~" H 5750 7000 50  0001 C CNN
	1    5750 7000
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R8
U 1 1 60FDC3ED
P 4750 4600
F 0 "R8" V 4545 4600 50  0000 C CNN
F 1 "470k" V 4636 4600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4790 4590 50  0001 C CNN
F 3 "~" H 4750 4600 50  0001 C CNN
	1    4750 4600
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R10
U 1 1 60FDCEDB
P 5000 5200
F 0 "R10" V 4795 5200 50  0000 C CNN
F 1 "470k" V 4886 5200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5040 5190 50  0001 C CNN
F 3 "~" H 5000 5200 50  0001 C CNN
	1    5000 5200
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R14
U 1 1 60FDCEEF
P 4400 5400
F 0 "R14" V 4195 5400 50  0000 C CNN
F 1 "33k" V 4286 5400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4440 5390 50  0001 C CNN
F 3 "~" H 4400 5400 50  0001 C CNN
	1    4400 5400
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R16
U 1 1 60FDCEF9
P 4150 4800
F 0 "R16" V 3945 4800 50  0000 C CNN
F 1 "33k" V 4036 4800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4190 4790 50  0001 C CNN
F 3 "~" H 4150 4800 50  0001 C CNN
	1    4150 4800
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R3
U 1 1 60FDEAA9
P 5900 6850
F 0 "R3" H 5968 6896 50  0000 L CNN
F 1 "100k" H 5968 6805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5940 6840 50  0001 C CNN
F 3 "~" H 5900 6850 50  0001 C CNN
	1    5900 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R5
U 1 1 60FDEAB3
P 5650 6250
F 0 "R5" H 5718 6296 50  0000 L CNN
F 1 "100k" H 5718 6205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5690 6240 50  0001 C CNN
F 3 "~" H 5650 6250 50  0001 C CNN
	1    5650 6250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R7
U 1 1 60FDEABD
P 5400 5650
F 0 "R7" H 5468 5696 50  0000 L CNN
F 1 "100k" H 5468 5605 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5440 5640 50  0001 C CNN
F 3 "~" H 5400 5650 50  0001 C CNN
	1    5400 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R9
U 1 1 60FDEAC7
P 5150 5050
F 0 "R9" H 5218 5096 50  0000 L CNN
F 1 "100k" H 5218 5005 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5190 5040 50  0001 C CNN
F 3 "~" H 5150 5050 50  0001 C CNN
	1    5150 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R11
U 1 1 60FDEAD1
P 4900 4450
F 0 "R11" H 4968 4496 50  0000 L CNN
F 1 "100k" H 4968 4405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4940 4440 50  0001 C CNN
F 3 "~" H 4900 4450 50  0001 C CNN
	1    4900 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R15
U 1 1 60FDEAE5
P 5150 7200
F 0 "R15" V 4945 7200 50  0000 C CNN
F 1 "33k" V 5036 7200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5190 7190 50  0001 C CNN
F 3 "~" H 5150 7200 50  0001 C CNN
	1    5150 7200
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R18
U 1 1 60FE0731
P 3900 4200
F 0 "R18" V 3695 4200 50  0000 C CNN
F 1 "33k" V 3786 4200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3940 4190 50  0001 C CNN
F 3 "~" H 3900 4200 50  0001 C CNN
	1    3900 4200
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT:MPSA42 Q2
U 1 1 60FE2DB5
P 5500 7200
F 0 "Q2" H 5691 7246 50  0000 L CNN
F 1 "MPSA42" H 5691 7155 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92L_Inline_Wide" H 5700 7125 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/MPSA42-D.PDF" H 5500 7200 50  0001 L CNN
	1    5500 7200
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MPSA42 Q4
U 1 1 60FE3598
P 4750 5400
F 0 "Q4" H 4941 5446 50  0000 L CNN
F 1 "MPSA42" H 4941 5355 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92L_Inline_Wide" H 4950 5325 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/MPSA42-D.PDF" H 4750 5400 50  0001 L CNN
	1    4750 5400
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MPSA42 Q5
U 1 1 60FE41A6
P 4250 4200
F 0 "Q5" H 4441 4246 50  0000 L CNN
F 1 "MPSA42" H 4441 4155 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92L_Inline_Wide" H 4450 4125 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/MPSA42-D.PDF" H 4250 4200 50  0001 L CNN
	1    4250 4200
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MPSA42 Q6
U 1 1 60FE41B0
P 4500 4800
F 0 "Q6" H 4691 4846 50  0000 L CNN
F 1 "MPSA42" H 4691 4755 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92L_Inline_Wide" H 4700 4725 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/MPSA42-D.PDF" H 4500 4800 50  0001 L CNN
	1    4500 4800
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS541 U3
U 1 1 60FE538F
P 4600 2100
F 0 "U3" H 4600 3081 50  0000 C CNN
F 1 "74LS541" H 4600 2990 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm_Socket_LongPads" H 4600 2100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS541" H 4600 2100 50  0001 C CNN
	1    4600 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 950  1800 950 
Wire Wire Line
	1900 950  1900 900 
Connection ~ 1300 950 
Connection ~ 1900 950 
Wire Wire Line
	1800 950  1800 1100
Connection ~ 1800 950 
Wire Wire Line
	1800 950  1900 950 
Wire Wire Line
	2200 1250 2200 1400
Wire Wire Line
	2200 1400 2000 1400
Wire Wire Line
	2050 3300 2100 3300
Wire Wire Line
	2100 3000 2100 2950
Wire Wire Line
	2100 2950 1550 2950
Wire Wire Line
	1550 2950 1550 3000
Wire Wire Line
	1450 3000 1450 2950
Wire Wire Line
	1450 2950 1550 2950
Connection ~ 1550 2950
Wire Wire Line
	1200 2950 1450 2950
Connection ~ 1450 2950
Wire Wire Line
	2500 950  2650 950 
Text GLabel 2650 800  1    50   Input ~ 0
5v
Wire Wire Line
	2650 950  2650 800 
$Comp
L power:GND #PWR0102
U 1 1 6100C0CE
P 2000 1500
F 0 "#PWR0102" H 2000 1250 50  0001 C CNN
F 1 "GND" H 2005 1327 50  0000 C CNN
F 2 "" H 2000 1500 50  0001 C CNN
F 3 "" H 2000 1500 50  0001 C CNN
	1    2000 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1500 2000 1400
Connection ~ 2000 1400
Wire Wire Line
	2000 1400 1800 1400
$Comp
L power:GND #PWR0103
U 1 1 6100CB94
P 1450 5950
F 0 "#PWR0103" H 1450 5700 50  0001 C CNN
F 1 "GND" H 1455 5777 50  0000 C CNN
F 2 "" H 1450 5950 50  0001 C CNN
F 3 "" H 1450 5950 50  0001 C CNN
	1    1450 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 5900 1550 5950
Wire Wire Line
	1550 5950 1450 5950
Wire Wire Line
	1450 5900 1450 5950
Connection ~ 1450 5950
Wire Wire Line
	1800 2200 2150 2200
$Comp
L power:GND #PWR0104
U 1 1 610156B3
P 1800 2500
F 0 "#PWR0104" H 1800 2250 50  0001 C CNN
F 1 "GND" H 1805 2327 50  0000 C CNN
F 2 "" H 1800 2500 50  0001 C CNN
F 3 "" H 1800 2500 50  0001 C CNN
	1    1800 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1800 2650 950 
Connection ~ 2650 950 
Wire Wire Line
	1550 1800 1550 2950
Wire Wire Line
	1550 1800 2650 1800
$Comp
L Device:Crystal Y1
U 1 1 60FD7D69
P 2300 2200
F 0 "Y1" H 2300 2468 50  0000 C CNN
F 1 "Crystal" H 2300 2377 50  0000 C CNN
F 2 "Crystal:Crystal_HC18-U_Vertical" H 2300 2200 50  0001 C CNN
F 3 "~" H 2300 2200 50  0001 C CNN
	1    2300 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 4000 2450 4000
Wire Wire Line
	2450 4000 2450 2200
Connection ~ 2450 2200
$Comp
L power:GND #PWR0105
U 1 1 61016407
P 2700 2500
F 0 "#PWR0105" H 2700 2250 50  0001 C CNN
F 1 "GND" H 2705 2327 50  0000 C CNN
F 2 "" H 2700 2500 50  0001 C CNN
F 3 "" H 2700 2500 50  0001 C CNN
	1    2700 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 2200 2700 2200
$Comp
L Device:C C3
U 1 1 60FD94D7
P 2700 2350
F 0 "C3" H 2815 2396 50  0000 L CNN
F 1 "22pF" H 2815 2305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 2738 2200 50  0001 C CNN
F 3 "~" H 2700 2350 50  0001 C CNN
	1    2700 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 4200 3700 3700
Wire Wire Line
	4000 4800 3600 4800
Wire Wire Line
	3750 4200 3700 4200
Wire Wire Line
	2050 3700 3700 3700
Wire Wire Line
	2050 4100 3600 4100
Wire Wire Line
	2050 4200 3500 4200
Wire Wire Line
	5000 7200 3200 7200
Wire Wire Line
	2050 4700 3200 4700
Wire Wire Line
	2050 4600 3300 4600
Wire Wire Line
	2050 4500 3400 4500
$Comp
L power:GND #PWR0106
U 1 1 610952B4
P 5600 7400
F 0 "#PWR0106" H 5600 7150 50  0001 C CNN
F 1 "GND" H 5605 7227 50  0000 C CNN
F 2 "" H 5600 7400 50  0001 C CNN
F 3 "" H 5600 7400 50  0001 C CNN
	1    5600 7400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 61095357
P 4850 5600
F 0 "#PWR0107" H 4850 5350 50  0001 C CNN
F 1 "GND" H 4855 5427 50  0000 C CNN
F 2 "" H 4850 5600 50  0001 C CNN
F 3 "" H 4850 5600 50  0001 C CNN
	1    4850 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 610957F1
P 4600 5000
F 0 "#PWR0108" H 4600 4750 50  0001 C CNN
F 1 "GND" H 4605 4827 50  0000 C CNN
F 2 "" H 4600 5000 50  0001 C CNN
F 3 "" H 4600 5000 50  0001 C CNN
	1    4600 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 61095C97
P 4350 4400
F 0 "#PWR0109" H 4350 4150 50  0001 C CNN
F 1 "GND" H 4355 4227 50  0000 C CNN
F 2 "" H 4350 4400 50  0001 C CNN
F 3 "" H 4350 4400 50  0001 C CNN
	1    4350 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 5400 4250 5400
Wire Wire Line
	3400 6000 4500 6000
$Comp
L Device:R_US R12
U 1 1 60FDCEE5
P 5250 5800
F 0 "R12" V 5045 5800 50  0000 C CNN
F 1 "470k" V 5136 5800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5290 5790 50  0001 C CNN
F 3 "~" H 5250 5800 50  0001 C CNN
	1    5250 5800
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R19
U 1 1 60FE073B
P 4650 6000
F 0 "R19" V 4445 6000 50  0000 C CNN
F 1 "33k" V 4536 6000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4690 5990 50  0001 C CNN
F 3 "~" H 4650 6000 50  0001 C CNN
	1    4650 6000
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 610941D1
P 5100 6200
F 0 "#PWR0110" H 5100 5950 50  0001 C CNN
F 1 "GND" H 5105 6027 50  0000 C CNN
F 2 "" H 5100 6200 50  0001 C CNN
F 3 "" H 5100 6200 50  0001 C CNN
	1    5100 6200
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MPSA42 Q3
U 1 1 60FE358E
P 5000 6000
F 0 "Q3" H 5191 6046 50  0000 L CNN
F 1 "MPSA42" H 5191 5955 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92L_Inline_Wide" H 5200 5925 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/MPSA42-D.PDF" H 5000 6000 50  0001 L CNN
	1    5000 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 6600 4750 6600
$Comp
L Device:R_US R4
U 1 1 60FDC1BE
P 5500 6400
F 0 "R4" V 5295 6400 50  0000 C CNN
F 1 "470k" V 5386 6400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5540 6390 50  0001 C CNN
F 3 "~" H 5500 6400 50  0001 C CNN
	1    5500 6400
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R17
U 1 1 60FDEAEF
P 4900 6600
F 0 "R17" V 4695 6600 50  0000 C CNN
F 1 "33k" V 4786 6600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4940 6590 50  0001 C CNN
F 3 "~" H 4900 6600 50  0001 C CNN
	1    4900 6600
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 61094A3B
P 5350 6800
F 0 "#PWR0111" H 5350 6550 50  0001 C CNN
F 1 "GND" H 5355 6627 50  0000 C CNN
F 2 "" H 5350 6800 50  0001 C CNN
F 3 "" H 5350 6800 50  0001 C CNN
	1    5350 6800
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MPSA42 Q1
U 1 1 60FE1B78
P 5250 6600
F 0 "Q1" H 5441 6646 50  0000 L CNN
F 1 "MPSA42" H 5441 6555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92L_Inline_Wide" H 5450 6525 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/MPSA42-D.PDF" H 5250 6600 50  0001 L CNN
	1    5250 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4700 3200 7200
Wire Wire Line
	3300 4600 3300 6600
Wire Wire Line
	3400 4500 3400 6000
Wire Wire Line
	3500 4200 3500 5400
Wire Wire Line
	3600 4100 3600 4800
Wire Wire Line
	5900 6700 5900 6100
Wire Wire Line
	5900 6100 5650 6100
Wire Wire Line
	5650 6100 5650 5500
Wire Wire Line
	5650 5500 5400 5500
Connection ~ 5650 6100
Wire Wire Line
	5400 5500 5400 4900
Wire Wire Line
	5400 4900 5150 4900
Connection ~ 5400 5500
Wire Wire Line
	5150 4900 5150 4300
Wire Wire Line
	5150 4300 4900 4300
Connection ~ 5150 4900
Wire Wire Line
	4900 4300 4900 3700
Wire Wire Line
	4900 3700 4650 3700
Connection ~ 4900 4300
Wire Wire Line
	1450 1150 1300 1150
Connection ~ 1300 1150
Text GLabel 6700 6700 0    50   Input ~ 0
digit6lsb
Text GLabel 6450 6000 0    50   Input ~ 0
digit5
Text GLabel 6200 5500 0    50   Input ~ 0
digit4
Text GLabel 5950 4850 0    50   Input ~ 0
digit3
Connection ~ 5150 5200
Connection ~ 5400 5800
Connection ~ 5650 6400
Connection ~ 5900 7000
Connection ~ 4900 4600
Text GLabel 5700 4250 0    50   Input ~ 0
digit2
Text GLabel 5500 3800 0    50   Input ~ 0
digit1msb
Wire Wire Line
	5650 6400 6450 6400
Wire Wire Line
	5900 6700 6200 6700
Wire Wire Line
	6200 6700 6200 7650
Wire Wire Line
	6200 7650 1100 7650
Wire Wire Line
	1100 7650 1100 1700
Wire Wire Line
	1100 1700 1450 1700
Wire Wire Line
	1450 1700 1450 1150
Connection ~ 5900 6700
Wire Wire Line
	2050 3600 3500 3600
Wire Wire Line
	2050 3800 3400 3800
Wire Wire Line
	2050 4300 3300 4300
Wire Wire Line
	2050 4400 3200 4400
Wire Wire Line
	2350 2400 2150 2400
Wire Wire Line
	2150 2400 2150 2200
Connection ~ 2150 2200
Wire Wire Line
	2350 2400 2350 3900
Wire Wire Line
	2350 3900 2050 3900
Wire Wire Line
	4600 2900 4100 2900
Wire Wire Line
	4100 2900 4100 2600
Connection ~ 4100 2600
Wire Wire Line
	4100 2600 4100 2500
$Comp
L power:GND #PWR0112
U 1 1 6113F45A
P 4600 2900
F 0 "#PWR0112" H 4600 2650 50  0001 C CNN
F 1 "GND" H 4605 2727 50  0000 C CNN
F 2 "" H 4600 2900 50  0001 C CNN
F 3 "" H 4600 2900 50  0001 C CNN
	1    4600 2900
	1    0    0    -1  
$EndComp
NoConn ~ 4100 2300
NoConn ~ 4100 2200
NoConn ~ 4100 2100
NoConn ~ 4100 1600
NoConn ~ 5100 1600
NoConn ~ 5100 2300
NoConn ~ 5100 2200
NoConn ~ 5100 2100
$Comp
L w_connectors:HEADER_16 J2
U 1 1 60FE6DA9
P 6600 1650
F 0 "J2" H 6728 1703 60  0000 L CNN
F 1 "K155ID1" H 6728 1597 60  0000 L CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket_LongPads" H 6600 1650 60  0001 C CNN
F 3 "" H 6600 1650 60  0000 C CNN
	1    6600 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1100 5200 1100
Wire Wire Line
	5200 1100 5200 1700
Wire Wire Line
	5300 1200 5300 2000
Wire Wire Line
	5400 1300 5400 950 
Text GLabel 5950 900  0    50   Input ~ 0
8
Text GLabel 5950 1000 0    50   Input ~ 0
9
Text GLabel 5950 1600 0    50   Input ~ 0
2
Text GLabel 5950 1700 0    50   Input ~ 0
3
Text GLabel 5950 1800 0    50   Input ~ 0
7
Text GLabel 5950 1900 0    50   Input ~ 0
6
Wire Wire Line
	5700 2000 5700 2100
$Comp
L power:GND #PWR0113
U 1 1 611C6700
P 5700 2100
F 0 "#PWR0113" H 5700 1850 50  0001 C CNN
F 1 "GND" H 5705 1927 50  0000 C CNN
F 2 "" H 5700 2100 50  0001 C CNN
F 3 "" H 5700 2100 50  0001 C CNN
	1    5700 2100
	1    0    0    -1  
$EndComp
Text GLabel 5950 2100 0    50   Input ~ 0
4
Text GLabel 5950 2200 0    50   Input ~ 0
5
Text GLabel 5950 2300 0    50   Input ~ 0
1
Text GLabel 5950 2400 0    50   Input ~ 0
0
$Comp
L Switch:SW_Push SW1
U 1 1 611C7E93
P 2550 6350
F 0 "SW1" H 2550 6635 50  0000 C CNN
F 1 "Select_butt" H 2550 6544 50  0000 C CNN
F 2 "" H 2550 6550 50  0001 C CNN
F 3 "~" H 2550 6550 50  0001 C CNN
	1    2550 6350
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 611CCA27
P 2650 5900
F 0 "SW2" H 2650 6185 50  0000 C CNN
F 1 "Up_butt" H 2650 6094 50  0000 C CNN
F 2 "" H 2650 6100 50  0001 C CNN
F 3 "~" H 2650 6100 50  0001 C CNN
	1    2650 5900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 611CFFF0
P 2750 5450
F 0 "SW3" H 2750 5735 50  0000 C CNN
F 1 "Down_butt" H 2750 5644 50  0000 C CNN
F 2 "" H 2750 5650 50  0001 C CNN
F 3 "~" H 2750 5650 50  0001 C CNN
	1    2750 5450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0114
U 1 1 611F23BD
P 2950 5450
F 0 "#PWR0114" H 2950 5200 50  0001 C CNN
F 1 "GND" H 2955 5277 50  0000 C CNN
F 2 "" H 2950 5450 50  0001 C CNN
F 3 "" H 2950 5450 50  0001 C CNN
	1    2950 5450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 611F2B1A
P 2850 5900
F 0 "#PWR0115" H 2850 5650 50  0001 C CNN
F 1 "GND" H 2855 5727 50  0000 C CNN
F 2 "" H 2850 5900 50  0001 C CNN
F 3 "" H 2850 5900 50  0001 C CNN
	1    2850 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 611F302C
P 2750 6350
F 0 "#PWR0116" H 2750 6100 50  0001 C CNN
F 1 "GND" H 2755 6177 50  0000 C CNN
F 2 "" H 2750 6350 50  0001 C CNN
F 3 "" H 2750 6350 50  0001 C CNN
	1    2750 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 5450 2550 5100
Wire Wire Line
	2550 5100 2050 5100
Wire Wire Line
	2450 5900 2450 5200
Wire Wire Line
	2450 5200 2050 5200
Wire Wire Line
	2350 6350 2350 5300
Wire Wire Line
	2350 5300 2050 5300
Wire Wire Line
	3100 5000 3100 6700
Wire Wire Line
	3100 6700 1200 6700
Wire Wire Line
	2050 5000 3100 5000
Wire Wire Line
	1200 2950 1200 6700
NoConn ~ 2050 4800
NoConn ~ 2050 4900
NoConn ~ 2050 5400
Wire Wire Line
	3500 2000 4100 2000
Wire Wire Line
	3500 2000 3500 3600
Wire Wire Line
	3400 1900 3400 3800
Wire Wire Line
	3400 1900 4100 1900
Wire Wire Line
	3300 1800 3300 4300
Wire Wire Line
	3300 1800 4100 1800
Wire Wire Line
	3200 1700 3200 4400
Wire Wire Line
	3200 1700 4100 1700
Connection ~ 4600 2900
Wire Wire Line
	5100 2000 5300 2000
Wire Wire Line
	5100 1900 5500 1900
Wire Wire Line
	5500 1900 5500 1500
Wire Wire Line
	5100 1800 5400 1800
Wire Wire Line
	5400 1800 5400 1400
Wire Wire Line
	5100 1700 5200 1700
$Comp
L w_connectors:HEADER_18 J3
U 1 1 612A594A
P 6950 3450
F 0 "J3" H 7078 3503 60  0000 L CNN
F 1 "HEADER_18" H 7078 3397 60  0000 L CNN
F 2 "" H 6950 3450 60  0001 C CNN
F 3 "" H 6950 3450 60  0000 C CNN
	1    6950 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 4300 6850 4300
Wire Wire Line
	6850 4200 6450 4200
Wire Wire Line
	6850 4100 6200 4100
Wire Wire Line
	6200 4100 6200 5800
Wire Wire Line
	5950 4000 5950 5200
Wire Wire Line
	6850 3900 5700 3900
Wire Wire Line
	5700 3900 5700 4600
Wire Wire Line
	5500 4000 5500 3800
Wire Wire Line
	5500 3800 6850 3800
Wire Wire Line
	4900 4600 5700 4600
Wire Wire Line
	5150 5200 5950 5200
Wire Wire Line
	5400 5800 6200 5800
Wire Wire Line
	5900 7000 6700 7000
$Comp
L power:GND #PWR0117
U 1 1 612FC262
P 4250 3600
F 0 "#PWR0117" H 4250 3350 50  0001 C CNN
F 1 "GND" H 4255 3427 50  0000 C CNN
F 2 "" H 4250 3600 50  0001 C CNN
F 3 "" H 4250 3600 50  0001 C CNN
	1    4250 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 3700 4900 3700
Connection ~ 4900 3700
Wire Wire Line
	6850 3600 4250 3600
Wire Wire Line
	6500 2600 6500 2400
Wire Wire Line
	6500 2300 6050 2300
Wire Wire Line
	6050 2300 6050 3500
Wire Wire Line
	6050 3500 6850 3500
Wire Wire Line
	6100 3400 6100 1600
Wire Wire Line
	6100 1600 6500 1600
Wire Wire Line
	6500 1700 6150 1700
Wire Wire Line
	6150 1700 6150 3300
Wire Wire Line
	6150 3300 6850 3300
Wire Wire Line
	6200 3200 6200 2100
Wire Wire Line
	6500 2200 6250 2200
Wire Wire Line
	6250 2200 6250 3100
Wire Wire Line
	6250 3100 6850 3100
Wire Wire Line
	6500 1900 6300 1900
Wire Wire Line
	6300 1900 6300 3000
Wire Wire Line
	6300 3000 6850 3000
Wire Wire Line
	6350 2900 6350 1800
Wire Wire Line
	6350 1800 6500 1800
Wire Wire Line
	6450 2700 6450 1000
Wire Wire Line
	6500 900  6400 900 
Wire Wire Line
	5950 1600 6100 1600
Connection ~ 6100 1600
Wire Wire Line
	5950 1700 6150 1700
Connection ~ 6150 1700
Wire Wire Line
	5950 1800 6350 1800
Connection ~ 6350 1800
Wire Wire Line
	5950 1900 6300 1900
Connection ~ 6300 1900
Wire Wire Line
	5950 2100 6200 2100
Connection ~ 6200 2100
Wire Wire Line
	6200 2100 6500 2100
Wire Wire Line
	5950 2200 6250 2200
Connection ~ 6250 2200
Wire Wire Line
	5950 2300 6050 2300
Connection ~ 6050 2300
Wire Wire Line
	5950 2400 6500 2400
Connection ~ 6500 2400
Connection ~ 6450 1000
Wire Wire Line
	6450 1000 5950 1000
Wire Wire Line
	6450 1000 6500 1000
Connection ~ 6400 900 
Wire Wire Line
	6400 900  5950 900 
Wire Wire Line
	6400 900  6400 2800
Wire Wire Line
	5700 2000 6500 2000
Wire Wire Line
	5300 1200 6500 1200
Wire Wire Line
	5400 1300 6500 1300
Wire Wire Line
	5400 1400 6500 1400
Wire Wire Line
	5500 1500 6500 1500
Wire Wire Line
	6400 2800 6850 2800
Wire Wire Line
	4600 1300 4600 950 
Connection ~ 4600 950 
Wire Wire Line
	2650 950  4600 950 
Wire Wire Line
	4600 950  5400 950 
Wire Wire Line
	6850 2600 6500 2600
Wire Wire Line
	6850 2700 6450 2700
Wire Wire Line
	6850 2900 6350 2900
Wire Wire Line
	6850 3200 6200 3200
Wire Wire Line
	6850 3400 6100 3400
Wire Wire Line
	5950 4000 6850 4000
Wire Wire Line
	6450 4200 6450 6400
Wire Wire Line
	6700 4300 6700 7000
Wire Wire Line
	7250 1950 7250 750 
Wire Wire Line
	5400 750  5400 950 
Connection ~ 5400 950 
Wire Wire Line
	3100 550  3100 4900
Wire Wire Line
	3100 4900 2250 4900
Wire Wire Line
	2250 4900 2250 5600
Wire Wire Line
	2250 5600 2050 5600
Wire Wire Line
	3000 650  3000 4800
Wire Wire Line
	3000 4800 2150 4800
Wire Wire Line
	2150 4800 2150 5500
Wire Wire Line
	2150 5500 2050 5500
Wire Wire Line
	4650 4000 5500 4000
Connection ~ 4650 4000
$Comp
L Device:R_US R13
U 1 1 60FDEADB
P 4650 3850
F 0 "R13" H 4718 3896 50  0000 L CNN
F 1 "100k" H 4718 3805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4690 3840 50  0001 C CNN
F 3 "~" H 4650 3850 50  0001 C CNN
	1    4650 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R6
U 1 1 60FDC3E3
P 4500 4000
F 0 "R6" V 4295 4000 50  0000 C CNN
F 1 "470k" V 4386 4000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4540 3990 50  0001 C CNN
F 3 "~" H 4500 4000 50  0001 C CNN
	1    4500 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	7250 750  5400 750 
Wire Wire Line
	7350 650  3000 650 
Wire Wire Line
	7450 550  3100 550 
Wire Wire Line
	7450 550  7450 1550
Wire Wire Line
	7350 2650 7350 650 
$Comp
L w_connectors:HEADER_14 J4
U 1 1 60FEF940
P 8150 2100
F 0 "J4" H 8278 2153 60  0000 L CNN
F 1 "RX-8025SA" H 8278 2047 60  0000 L CNN
F 2 "Package_SO:SOP-16_4.4x10.4mm_P1.27mm" H 8150 2100 60  0001 C CNN
F 3 "" H 8150 2100 60  0000 C CNN
	1    8150 2100
	1    0    0    -1  
$EndComp
NoConn ~ 8050 1450
NoConn ~ 8050 1650
NoConn ~ 8050 1750
NoConn ~ 8050 1850
NoConn ~ 8050 2150
NoConn ~ 8050 2250
NoConn ~ 8050 2350
NoConn ~ 8050 2550
NoConn ~ 8050 2750
$Comp
L power:GND #PWR0118
U 1 1 61031C23
P 7950 2450
F 0 "#PWR0118" H 7950 2200 50  0001 C CNN
F 1 "GND" H 7955 2277 50  0000 C CNN
F 2 "" H 7950 2450 50  0001 C CNN
F 3 "" H 7950 2450 50  0001 C CNN
	1    7950 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 2450 7950 2450
Wire Wire Line
	8050 2050 7950 2050
Wire Wire Line
	7950 2050 7950 2450
Connection ~ 7950 2450
Wire Wire Line
	8050 1950 7600 1950
Wire Wire Line
	8050 1550 7600 1550
Wire Wire Line
	8050 2650 7600 2650
$Comp
L Device:R_US R21
U 1 1 61159182
P 7600 2500
F 0 "R21" H 7668 2546 50  0000 L CNN
F 1 "3.3k" H 7668 2455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7640 2490 50  0001 C CNN
F 3 "~" H 7600 2500 50  0001 C CNN
	1    7600 2500
	1    0    0    -1  
$EndComp
Connection ~ 7600 2650
Wire Wire Line
	7600 2650 7350 2650
Connection ~ 7600 1550
Wire Wire Line
	7600 1550 7450 1550
Wire Wire Line
	7600 1850 7600 1950
Connection ~ 7600 1950
Wire Wire Line
	7600 1950 7250 1950
Wire Wire Line
	7600 2350 7600 1950
$Comp
L Device:R_US R20
U 1 1 6115918C
P 7600 1700
F 0 "R20" V 7395 1700 50  0000 C CNN
F 1 "3.3k" V 7486 1700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7640 1690 50  0001 C CNN
F 3 "~" H 7600 1700 50  0001 C CNN
	1    7600 1700
	-1   0    0    1   
$EndComp
NoConn ~ 2050 3400
NoConn ~ 2050 3500
Text Notes 8650 1500 0    50   ~ 0
should I add a backup battery???
$EndSCHEMATC
