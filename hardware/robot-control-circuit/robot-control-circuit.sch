EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Navigation Board"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Interface_Expansion:MCP23017_SP U3
U 1 1 607C640F
P 8000 4000
F 0 "U3" H 8000 5281 50  0000 C CNN
F 1 "MCP23017_SP" H 8000 5190 50  0000 C CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 8200 3000 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf" H 8200 2900 50  0001 L CNN
	1    8000 4000
	1    0    0    -1  
$EndComp
Text Label 1550 850  0    50   ~ 0
VL53L1-1_INT
Text Label 1550 950  0    50   ~ 0
VL53L1-1_XSHUT
$Comp
L Diode:1N4001 D1
U 1 1 607FC4F8
P 4350 5750
F 0 "D1" H 4350 5534 50  0000 C CNN
F 1 "1N4001" H 4350 5625 50  0000 C CNN
F 2 "Diode_THT:D_DO-34_SOD68_P2.54mm_Vertical_KathodeUp" H 4350 5575 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 4350 5750 50  0001 C CNN
	1    4350 5750
	-1   0    0    1   
$EndComp
Text Label 4200 5750 0    50   ~ 0
VCC
$Comp
L Device:LED D2
U 1 1 608065A6
P 4950 6650
F 0 "D2" V 4989 6533 50  0000 R CNN
F 1 "Power LED green" V 4898 6533 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 4950 6650 50  0001 C CNN
F 3 "~" H 4950 6650 50  0001 C CNN
	1    4950 6650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R9
U 1 1 608071AB
P 4950 6950
F 0 "R9" H 5020 6996 50  0000 L CNN
F 1 "330" H 5020 6905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4880 6950 50  0001 C CNN
F 3 "~" H 4950 6950 50  0001 C CNN
	1    4950 6950
	1    0    0    -1  
$EndComp
Text Label 4950 6500 0    50   ~ 0
VDD
Text Label 4950 7100 0    50   ~ 0
GND
Text Label 5350 2100 0    50   ~ 0
I2C-1_SDA
Text Label 8000 2900 0    50   ~ 0
VDD
Text Label 8000 5100 0    50   ~ 0
GND
Wire Wire Line
	8000 5100 7300 5100
Wire Wire Line
	7300 5100 7300 4800
Connection ~ 7300 4700
Wire Wire Line
	7300 4700 7300 4600
Connection ~ 7300 4800
Wire Wire Line
	7300 4800 7300 4700
Text Label 6850 3200 0    50   ~ 0
I2C-1_SDA
Wire Wire Line
	6850 3200 7300 3200
Wire Wire Line
	7300 3300 6850 3300
Text Label 6850 3300 0    50   ~ 0
I2C-1_SCL
Text Label 7150 4100 0    50   ~ 0
VDD
Wire Wire Line
	7300 850  6900 850 
Text Label 6900 850  0    50   ~ 0
ODO_LEFT_HV
Text Label 7600 650  0    50   ~ 0
VDD
$Comp
L Transistor_BJT:BD235 Q2
U 1 1 60837503
P 7500 1750
F 0 "Q2" H 7692 1796 50  0000 L CNN
F 1 "BD235" H 7692 1705 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-126-3_Vertical" H 7700 1675 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BD/BD233.pdf" H 7500 1750 50  0001 L CNN
	1    7500 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 1750 6900 1750
Text Label 6900 1750 0    50   ~ 0
ODO_RIGHT_HV
Text Label 7600 1550 0    50   ~ 0
VDD
Text Label 7600 1950 0    50   ~ 0
ODO_RIGHT_LV
$Comp
L Connector:Conn_01x20_Female J1
U 1 1 60841A62
P 900 6800
F 0 "J1" H 792 5575 50  0000 C CNN
F 1 "CCRP5-EC2" H 792 5666 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Vertical" H 900 6800 50  0001 C CNN
F 3 "http://suddendocs.samtec.com/catalog_english/ssa.pdf" H 900 6800 50  0001 C CNN
F 4 "SSA-120-W-T " H 900 6800 50  0001 C CNN "Part"
	1    900  6800
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x20_Female J4
U 1 1 60847E5F
P 2000 6800
F 0 "J4" H 1892 5575 50  0000 C CNN
F 1 "CCRP5-EC1" H 1892 5666 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Vertical" H 2000 6800 50  0001 C CNN
F 3 "http://suddendocs.samtec.com/catalog_english/ssa.pdf" H 2000 6800 50  0001 C CNN
F 4 "SSA-120-W-T " H 2000 6800 50  0001 C CNN "Part"
	1    2000 6800
	-1   0    0    1   
$EndComp
Text Label 1100 6000 0    50   ~ 0
COMM-NAV_POWER+
Text Label 1100 7700 0    50   ~ 0
GND
Text Label 1100 5800 0    50   ~ 0
VCC
$Comp
L power:GND #PWR01
U 1 1 6084F717
P 1400 6000
F 0 "#PWR01" H 1400 5750 50  0001 C CNN
F 1 "GND" H 1405 5827 50  0000 C CNN
F 2 "" H 1400 6000 50  0001 C CNN
F 3 "" H 1400 6000 50  0001 C CNN
	1    1400 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 6000 1400 6000
$Comp
L Connector:Conn_01x02_Male J10
U 1 1 608550AB
P 9350 1650
F 0 "J10" H 9458 1831 50  0000 C CNN
F 1 "Testpoint GND" H 9458 1740 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9350 1650 50  0001 C CNN
F 3 "~" H 9350 1650 50  0001 C CNN
	1    9350 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J11
U 1 1 60859505
P 9350 2000
F 0 "J11" H 9458 2181 50  0000 C CNN
F 1 "Testpoint VDD" H 9458 2090 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9350 2000 50  0001 C CNN
F 3 "~" H 9350 2000 50  0001 C CNN
	1    9350 2000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J12
U 1 1 608597A7
P 9350 2350
F 0 "J12" H 9458 2531 50  0000 C CNN
F 1 "Testpoint VCC" H 9458 2440 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9350 2350 50  0001 C CNN
F 3 "~" H 9350 2350 50  0001 C CNN
	1    9350 2350
	1    0    0    -1  
$EndComp
Text Label 9550 1650 0    50   ~ 0
GND
Text Label 9550 1750 0    50   ~ 0
GND
Text Label 9550 2000 0    50   ~ 0
VDD
Text Label 9550 2100 0    50   ~ 0
VDD
Text Label 9550 2350 0    50   ~ 0
VCC
Text Label 9550 2450 0    50   ~ 0
VCC
Text Label 7600 1350 0    50   ~ 0
GND
$Comp
L Device:R R12
U 1 1 60873497
P 7600 1200
F 0 "R12" H 7670 1246 50  0000 L CNN
F 1 "1 k" H 7670 1155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 7530 1200 50  0001 C CNN
F 3 "~" H 7600 1200 50  0001 C CNN
	1    7600 1200
	1    0    0    -1  
$EndComp
Text Label 7600 1050 0    50   ~ 0
ODO_LEFT_LV
$Comp
L Transistor_BJT:BD235 Q1
U 1 1 6082C578
P 7500 850
F 0 "Q1" H 7692 896 50  0000 L CNN
F 1 "BD235" H 7692 805 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-126-3_Vertical" H 7700 775 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BD/BD233.pdf" H 7500 850 50  0001 L CNN
	1    7500 850 
	1    0    0    -1  
$EndComp
Text Label 7600 2250 0    50   ~ 0
GND
$Comp
L Device:R R13
U 1 1 608780F0
P 7600 2100
F 0 "R13" H 7670 2146 50  0000 L CNN
F 1 "1 k" H 7670 2055 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 7530 2100 50  0001 C CNN
F 3 "~" H 7600 2100 50  0001 C CNN
	1    7600 2100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x15_Female J7
U 1 1 6087CED0
P 2750 6600
F 0 "J7" H 2642 5575 50  0000 C CNN
F 1 "CCRP5-EC3" H 2642 5666 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x15_P2.54mm_Vertical" H 2750 6600 50  0001 C CNN
F 3 "~" H 2750 6600 50  0001 C CNN
	1    2750 6600
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x10_Male J13
U 1 1 60881A8A
P 9650 3200
F 0 "J13" H 9622 3174 50  0000 R CNN
F 1 "CCRP5_Injections" H 9622 3083 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 9650 3200 50  0001 C CNN
F 3 "~" H 9650 3200 50  0001 C CNN
	1    9650 3200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8700 3200 9450 3200
Wire Wire Line
	9450 3300 8700 3300
Wire Wire Line
	8700 3400 9450 3400
Wire Wire Line
	9450 3500 8700 3500
Wire Wire Line
	8700 3600 9450 3600
Wire Wire Line
	9450 3700 8700 3700
Wire Wire Line
	8700 3800 9450 3800
Wire Wire Line
	9450 3900 8700 3900
Text Label 5350 2400 0    50   ~ 0
ODO_RIGHT_LV
Text Label 5350 2500 0    50   ~ 0
ODO_LEFT_LV
NoConn ~ 5350 2800
NoConn ~ 5350 2900
$Comp
L Device:R R1
U 1 1 6089BE2D
P 2350 850
F 0 "R1" V 2250 850 50  0000 C CNN
F 1 "10 k" V 2250 1050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2280 850 50  0001 C CNN
F 3 "~" H 2350 850 50  0001 C CNN
	1    2350 850 
	0    1    1    0   
$EndComp
Wire Wire Line
	2200 850  1550 850 
$Comp
L Device:R R2
U 1 1 608BCB44
P 2350 950
F 0 "R2" V 2450 950 50  0000 C CNN
F 1 "10 k" V 2450 1150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2280 950 50  0001 C CNN
F 3 "~" H 2350 950 50  0001 C CNN
	1    2350 950 
	0    1    1    0   
$EndComp
Text Label 2500 950  0    50   ~ 0
VDD
$Comp
L Device:R R10
U 1 1 608C0D8C
P 6350 2000
F 0 "R10" V 6300 1800 50  0000 C CNN
F 1 "4.7 k" V 6350 2000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 6280 2000 50  0001 C CNN
F 3 "~" H 6350 2000 50  0001 C CNN
	1    6350 2000
	0    1    1    0   
$EndComp
Text Label 6500 2000 0    50   ~ 0
VDD
$Comp
L Device:R R11
U 1 1 608C16C6
P 6350 2100
F 0 "R11" V 6300 1900 50  0000 C CNN
F 1 "4.7 k" V 6350 2100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 6280 2100 50  0001 C CNN
F 3 "~" H 6350 2100 50  0001 C CNN
	1    6350 2100
	0    1    1    0   
$EndComp
Text Label 6500 2100 0    50   ~ 0
VDD
Wire Wire Line
	2200 950  1550 950 
Text Label 8700 4100 0    50   ~ 0
VL53L1-1_INT
Text Label 8700 4200 0    50   ~ 0
VL53L1-1_XSHUT
Text Label 8700 4300 0    50   ~ 0
VL53L1-2_INT
Text Label 8700 4400 0    50   ~ 0
VL53L1-2_XSHUT
Text Label 8700 4500 0    50   ~ 0
VL53L1-3_INT
Text Label 8700 4600 0    50   ~ 0
VL53L1-3_XSHUT
Text Label 8700 4700 0    50   ~ 0
VL53L1-4_INT
Text Label 8700 4800 0    50   ~ 0
VL53L1-4_XSHUT
$Comp
L power:GND #PWR0101
U 1 1 60924D3F
P 4550 4450
F 0 "#PWR0101" H 4550 4200 50  0001 C CNN
F 1 "GND" H 4555 4277 50  0000 C CNN
F 2 "" H 4550 4450 50  0001 C CNN
F 3 "" H 4550 4450 50  0001 C CNN
	1    4550 4450
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 60926D0E
P 4550 4350
F 0 "#FLG0101" H 4550 4425 50  0001 C CNN
F 1 "PWR_FLAG" H 4550 4523 50  0000 C CNN
F 2 "" H 4550 4350 50  0001 C CNN
F 3 "~" H 4550 4350 50  0001 C CNN
	1    4550 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 4350 4550 4450
Text Label 4550 4400 0    50   ~ 0
GND
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 6092C344
P 5350 4350
F 0 "#FLG0103" H 5350 4425 50  0001 C CNN
F 1 "PWR_FLAG" H 5350 4523 50  0000 C CNN
F 2 "" H 5350 4350 50  0001 C CNN
F 3 "~" H 5350 4350 50  0001 C CNN
	1    5350 4350
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0103
U 1 1 6092C8A6
P 5350 4450
F 0 "#PWR0103" H 5350 4300 50  0001 C CNN
F 1 "VCC" H 5368 4623 50  0000 C CNN
F 2 "" H 5350 4450 50  0001 C CNN
F 3 "" H 5350 4450 50  0001 C CNN
	1    5350 4450
	-1   0    0    1   
$EndComp
Wire Wire Line
	5350 4450 5350 4350
Text Label 5350 4400 0    50   ~ 0
VCC
Text Label 5350 2600 0    50   ~ 0
IO-EXP-1_INTB
Text Label 5350 2700 0    50   ~ 0
IO-EXP-1_!INTA
Text Label 6650 3800 0    50   ~ 0
IO-EXP-1_INTA
Text Label 6650 3900 0    50   ~ 0
IO-EXP-1_INTB
Wire Wire Line
	7300 3800 6650 3800
Wire Wire Line
	7300 3900 6650 3900
NoConn ~ 1100 7600
NoConn ~ 1100 7500
NoConn ~ 1100 7400
NoConn ~ 1100 7300
NoConn ~ 1100 7200
NoConn ~ 1100 7100
NoConn ~ 1100 7000
NoConn ~ 1100 6900
NoConn ~ 1100 6800
NoConn ~ 1100 6700
NoConn ~ 1100 6600
NoConn ~ 1100 6500
NoConn ~ 1100 6400
NoConn ~ 1100 6300
NoConn ~ 1100 6200
NoConn ~ 1100 6100
NoConn ~ 1100 5900
NoConn ~ 2200 7600
NoConn ~ 2200 7500
NoConn ~ 2200 7400
NoConn ~ 2200 7300
NoConn ~ 2200 7200
NoConn ~ 2200 7100
NoConn ~ 2200 7000
NoConn ~ 2200 6900
NoConn ~ 2200 6800
NoConn ~ 2200 6700
NoConn ~ 2200 6600
NoConn ~ 2200 6500
NoConn ~ 2200 6400
NoConn ~ 2200 6300
NoConn ~ 2200 6200
NoConn ~ 2200 6100
NoConn ~ 2200 6000
NoConn ~ 2200 5900
NoConn ~ 2200 5800
NoConn ~ 2950 5900
NoConn ~ 2950 6000
NoConn ~ 2950 6100
NoConn ~ 2950 6200
NoConn ~ 2950 6300
NoConn ~ 2950 6400
NoConn ~ 2950 6500
NoConn ~ 2950 6600
NoConn ~ 2950 6700
NoConn ~ 2950 6800
NoConn ~ 2950 6900
NoConn ~ 2950 7000
NoConn ~ 2950 7100
NoConn ~ 2950 7200
NoConn ~ 2950 7300
Text Label 5100 5750 0    50   ~ 0
5V
Text Label 4800 6050 0    50   ~ 0
GND
$Comp
L Regulator_Linear:LM7805_TO220 U1
U 1 1 607C7E05
P 4800 5750
F 0 "U1" H 4800 5992 50  0000 C CNN
F 1 "LM7805_TO220" H 4800 5901 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4800 5975 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 4800 5700 50  0001 C CNN
	1    4800 5750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J2
U 1 1 60817DDE
P 1350 1050
F 0 "J2" H 1400 1467 50  0000 C CNN
F 1 "VL53L1-SATEL connector 1" H 1400 1376 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Horizontal" H 1350 1050 50  0001 C CNN
F 3 "~" H 1350 1050 50  0001 C CNN
	1    1350 1050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	600  850  1050 850 
Wire Wire Line
	600  950  1050 950 
Wire Wire Line
	600  1050 1050 1050
Text Label 1550 1050 0    50   ~ 0
VDD
Text Label 600  1050 0    50   ~ 0
GND
Text Label 600  950  0    50   ~ 0
I2C-1_SDA
Text Label 600  850  0    50   ~ 0
I2C-1_SCL
NoConn ~ 1050 1150
NoConn ~ 1550 1150
NoConn ~ 1050 1250
Text Label 1550 1250 0    50   ~ 0
GND
Text Label 2500 850  0    50   ~ 0
VDD
Text Label 1550 1600 0    50   ~ 0
VL53L1-2_INT
Text Label 1550 1700 0    50   ~ 0
VL53L1-2_XSHUT
$Comp
L Device:R R3
U 1 1 608D989F
P 2350 1600
F 0 "R3" V 2250 1600 50  0000 C CNN
F 1 "10 k" V 2250 1800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2280 1600 50  0001 C CNN
F 3 "~" H 2350 1600 50  0001 C CNN
	1    2350 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	2200 1600 1550 1600
$Comp
L Device:R R4
U 1 1 608D98A6
P 2350 1700
F 0 "R4" V 2450 1700 50  0000 C CNN
F 1 "10 k" V 2450 1900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2280 1700 50  0001 C CNN
F 3 "~" H 2350 1700 50  0001 C CNN
	1    2350 1700
	0    1    1    0   
$EndComp
Text Label 2500 1700 0    50   ~ 0
VDD
Wire Wire Line
	2200 1700 1550 1700
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J3
U 1 1 608D98AE
P 1350 1800
F 0 "J3" H 1400 2217 50  0000 C CNN
F 1 "VL53L1-SATEL connector 2" H 1400 2126 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Horizontal" H 1350 1800 50  0001 C CNN
F 3 "~" H 1350 1800 50  0001 C CNN
	1    1350 1800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	600  1600 1050 1600
Wire Wire Line
	600  1700 1050 1700
Wire Wire Line
	600  1800 1050 1800
Text Label 1550 1800 0    50   ~ 0
VDD
Text Label 600  1800 0    50   ~ 0
GND
Text Label 600  1700 0    50   ~ 0
I2C-1_SDA
Text Label 600  1600 0    50   ~ 0
I2C-1_SCL
NoConn ~ 1050 1900
NoConn ~ 1550 1900
NoConn ~ 1050 2000
Text Label 1550 2000 0    50   ~ 0
GND
Text Label 2500 1600 0    50   ~ 0
VDD
Text Label 1550 2350 0    50   ~ 0
VL53L1-3_INT
Text Label 1550 2450 0    50   ~ 0
VL53L1-3_XSHUT
$Comp
L Device:R R5
U 1 1 608DAE0F
P 2350 2350
F 0 "R5" V 2250 2350 50  0000 C CNN
F 1 "10 k" V 2250 2550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2280 2350 50  0001 C CNN
F 3 "~" H 2350 2350 50  0001 C CNN
	1    2350 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	2200 2350 1550 2350
$Comp
L Device:R R6
U 1 1 608DAE16
P 2350 2450
F 0 "R6" V 2450 2450 50  0000 C CNN
F 1 "10 k" V 2450 2650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2280 2450 50  0001 C CNN
F 3 "~" H 2350 2450 50  0001 C CNN
	1    2350 2450
	0    1    1    0   
$EndComp
Text Label 2500 2450 0    50   ~ 0
VDD
Wire Wire Line
	2200 2450 1550 2450
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J5
U 1 1 608DAE1E
P 1350 2550
F 0 "J5" H 1400 2967 50  0000 C CNN
F 1 "VL53L1-SATEL connector 3" H 1400 2876 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Horizontal" H 1350 2550 50  0001 C CNN
F 3 "~" H 1350 2550 50  0001 C CNN
	1    1350 2550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	600  2350 1050 2350
Wire Wire Line
	600  2450 1050 2450
Wire Wire Line
	600  2550 1050 2550
Text Label 1550 2550 0    50   ~ 0
VDD
Text Label 600  2550 0    50   ~ 0
GND
Text Label 600  2450 0    50   ~ 0
I2C-1_SDA
Text Label 600  2350 0    50   ~ 0
I2C-1_SCL
NoConn ~ 1050 2650
NoConn ~ 1550 2650
NoConn ~ 1050 2750
Text Label 1550 2750 0    50   ~ 0
GND
Text Label 2500 2350 0    50   ~ 0
VDD
Text Label 1550 3100 0    50   ~ 0
VL53L1-4_INT
Text Label 1550 3200 0    50   ~ 0
VL53L1-4_XSHUT
$Comp
L Device:R R7
U 1 1 608DCC29
P 2350 3100
F 0 "R7" V 2250 3100 50  0000 C CNN
F 1 "10 k" V 2250 3300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2280 3100 50  0001 C CNN
F 3 "~" H 2350 3100 50  0001 C CNN
	1    2350 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	2200 3100 1550 3100
$Comp
L Device:R R8
U 1 1 608DCC30
P 2350 3200
F 0 "R8" V 2450 3200 50  0000 C CNN
F 1 "10 k" V 2450 3400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2280 3200 50  0001 C CNN
F 3 "~" H 2350 3200 50  0001 C CNN
	1    2350 3200
	0    1    1    0   
$EndComp
Text Label 2500 3200 0    50   ~ 0
VDD
Wire Wire Line
	2200 3200 1550 3200
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J6
U 1 1 608DCC38
P 1350 3300
F 0 "J6" H 1400 3717 50  0000 C CNN
F 1 "VL53L1-SATEL connector 4" H 1400 3626 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Horizontal" H 1350 3300 50  0001 C CNN
F 3 "~" H 1350 3300 50  0001 C CNN
	1    1350 3300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	600  3100 1050 3100
Wire Wire Line
	600  3200 1050 3200
Wire Wire Line
	600  3300 1050 3300
Text Label 1550 3300 0    50   ~ 0
VDD
Text Label 600  3300 0    50   ~ 0
GND
Text Label 600  3200 0    50   ~ 0
I2C-1_SDA
Text Label 600  3100 0    50   ~ 0
I2C-1_SCL
NoConn ~ 1050 3400
NoConn ~ 1550 3400
NoConn ~ 1050 3500
Text Label 1550 3500 0    50   ~ 0
GND
Text Label 2500 3100 0    50   ~ 0
VDD
Text Label 8750 3800 0    50   ~ 0
BUMPER_LEFT
Text Label 8750 3900 0    50   ~ 0
BUMPER_RIGHT
Wire Wire Line
	9450 3000 8750 3000
Wire Wire Line
	8750 3100 9450 3100
Text Label 8750 3000 0    50   ~ 0
ODO_LEFT_HV
Text Label 8750 3100 0    50   ~ 0
ODO_RIGHT_HV
Wire Wire Line
	9450 4000 9150 4000
Text Label 9150 4000 0    50   ~ 0
GND
Text Label 5350 2000 0    50   ~ 0
I2C-1_SCL
Text Label 5350 1800 0    50   ~ 0
MOTOR_OUT_LEFT
Text Label 5350 1900 0    50   ~ 0
MOTOR_OUT_RIGHT
$Comp
L Device:R R15
U 1 1 6085161B
P 6350 1900
F 0 "R15" V 6300 1700 50  0000 C CNN
F 1 "10 k" V 6350 1900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 6280 1900 50  0001 C CNN
F 3 "~" H 6350 1900 50  0001 C CNN
	1    6350 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R R14
U 1 1 60851A93
P 6350 1800
F 0 "R14" V 6300 1600 50  0000 C CNN
F 1 "10 k" V 6350 1800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 6280 1800 50  0001 C CNN
F 3 "~" H 6350 1800 50  0001 C CNN
	1    6350 1800
	0    1    1    0   
$EndComp
Text Label 6500 1800 0    50   ~ 0
GND
Text Label 6500 1900 0    50   ~ 0
GND
Text Label 8750 3200 0    50   ~ 0
MOTOR_DIR_LEFT
Text Label 8750 3300 0    50   ~ 0
MOTOR_DIR_RIGHT
Wire Wire Line
	9450 2800 8750 2800
Wire Wire Line
	9450 2900 8750 2900
Text Label 8750 2800 0    50   ~ 0
MOTOR_OUT_LEFT
Text Label 8750 2900 0    50   ~ 0
MOTOR_OUT_RIGHT
$Comp
L Connector:Conn_01x03_Male J8
U 1 1 6085065B
P 9650 3900
F 0 "J8" H 9600 3800 50  0000 C CNN
F 1 "Bumper" H 9500 3900 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 9650 3900 50  0001 C CNN
F 3 "~" H 9650 3900 50  0001 C CNN
	1    9650 3900
	-1   0    0    1   
$EndComp
Wire Wire Line
	3500 3100 3750 3100
Text Label 3500 3100 0    50   ~ 0
5V
$Comp
L ESP8266:NodeMCU_1.0_(ESP-12E) U2
U 1 1 60864097
P 4550 2400
F 0 "U2" H 4550 3487 60  0000 C CNN
F 1 "NodeMCU_1.0_(ESP-12E)" H 4550 3381 60  0000 C CNN
F 2 "ESP8266:NodeMCU1.0(12-E)" H 3950 1550 60  0001 C CNN
F 3 "" H 3950 1550 60  0000 C CNN
	1    4550 2400
	1    0    0    -1  
$EndComp
NoConn ~ 3750 2900
NoConn ~ 3750 2800
NoConn ~ 3750 2500
NoConn ~ 3750 2400
NoConn ~ 3750 2300
NoConn ~ 3750 2200
NoConn ~ 3750 2100
NoConn ~ 3750 2000
NoConn ~ 3750 1700
NoConn ~ 3750 1800
NoConn ~ 3750 1900
NoConn ~ 5350 3100
NoConn ~ 5350 3000
Text Label 5350 2200 0    50   ~ 0
VDD
Text Label 5350 2300 0    50   ~ 0
GND
NoConn ~ 3750 2600
NoConn ~ 3750 2700
NoConn ~ 3750 3000
NoConn ~ 2200 7700
$Comp
L Transistor_BJT:BC548 Q3
U 1 1 608DF190
P 5900 1150
F 0 "Q3" H 6091 1196 50  0000 L CNN
F 1 "BC548" H 6091 1105 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 6100 1075 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 5900 1150 50  0001 L CNN
	1    5900 1150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R16
U 1 1 608F20F5
P 5550 1150
F 0 "R16" V 5343 1150 50  0000 C CNN
F 1 "10 k" V 5434 1150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 5480 1150 50  0001 C CNN
F 3 "~" H 5550 1150 50  0001 C CNN
	1    5550 1150
	0    1    1    0   
$EndComp
Text Label 6000 1350 0    50   ~ 0
GND
Text Label 6000 650  0    50   ~ 0
VDD
$Comp
L Device:R R17
U 1 1 608E65DA
P 6000 800
F 0 "R17" H 6070 846 50  0000 L CNN
F 1 "10 k" H 6070 755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 5930 800 50  0001 C CNN
F 3 "~" H 6000 800 50  0001 C CNN
	1    6000 800 
	1    0    0    -1  
$EndComp
Text Label 6000 950  0    50   ~ 0
IO-EXP-1_!INTA
Wire Wire Line
	5350 1800 6200 1800
Wire Wire Line
	5350 1900 6200 1900
Wire Wire Line
	5350 2000 6200 2000
Wire Wire Line
	5350 2100 6200 2100
Text Notes 4600 1000 0    40   ~ 0
The interrupt signals of the\nIO-expander are default high.\nThis conflicts with GPIO15 \nas this must not be high\nat boot. Thus the inverter.
Wire Wire Line
	7150 4100 7300 4100
$Comp
L Device:C C1
U 1 1 608E87F3
P 6300 2500
F 0 "C1" H 6415 2584 50  0000 L CNN
F 1 "100 n" H 6415 2493 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L4.6mm_W2.0mm_P2.50mm_MKS02_FKP02" H 6338 2350 50  0001 C CNN
F 3 "~" H 6300 2500 50  0001 C CNN
F 4 "must be close to microcontroller" H 6415 2409 40  0000 L CNN "Placement Notes"
	1    6300 2500
	1    0    0    -1  
$EndComp
Text Label 6300 2350 0    50   ~ 0
IO-EXP-1_INTA
Text Label 6300 2650 2    50   ~ 0
GND
$Comp
L Device:C C2
U 1 1 6092430C
P 6300 2950
F 0 "C2" H 6415 3034 50  0000 L CNN
F 1 "100 n" H 6415 2943 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L4.6mm_W2.0mm_P2.50mm_MKS02_FKP02" H 6338 2800 50  0001 C CNN
F 3 "~" H 6300 2950 50  0001 C CNN
F 4 "must be close to microcontroller" H 6415 2859 40  0000 L CNN "Placement Notes"
	1    6300 2950
	1    0    0    -1  
$EndComp
Text Label 6300 2800 0    50   ~ 0
IO-EXP-1_INTB
Text Label 6300 3100 2    50   ~ 0
GND
$Comp
L Device:CP C3
U 1 1 6092D427
P 9750 4800
F 0 "C3" H 9868 4884 50  0000 L CNN
F 1 "10 µ" H 9868 4793 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 9788 4650 50  0001 C CNN
F 3 "~" H 9750 4800 50  0001 C CNN
F 4 "must be close to io-expander" H 9868 4709 40  0000 L CNN "Placement Notes"
	1    9750 4800
	1    0    0    -1  
$EndComp
Text Label 9750 4650 0    50   ~ 0
VDD
Text Label 9750 4950 2    50   ~ 0
GND
$Comp
L Device:C C4
U 1 1 6093BDBA
P 9750 4350
F 0 "C4" H 9865 4434 50  0000 L CNN
F 1 "100 n" H 9865 4343 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L4.6mm_W2.0mm_P2.50mm_MKS02_FKP02" H 9788 4200 50  0001 C CNN
F 3 "~" H 9750 4350 50  0001 C CNN
F 4 "must be close to io-expander" H 9865 4259 40  0000 L CNN "Placement Notes"
	1    9750 4350
	1    0    0    -1  
$EndComp
Text Label 9750 4200 0    50   ~ 0
VDD
Text Label 9750 4500 2    50   ~ 0
GND
Text Label 4800 1150 0    50   ~ 0
IO-EXP-1_INTA
Wire Wire Line
	4800 1150 5400 1150
NoConn ~ 5350 1700
$Comp
L Device:LED D?
U 1 1 618B38D5
P 1550 4000
F 0 "D?" V 1589 3883 50  0000 R CNN
F 1 "Signal LED green" H 1500 3850 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 1550 4000 50  0001 C CNN
F 3 "~" H 1550 4000 50  0001 C CNN
	1    1550 4000
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 618B95BC
P 1250 4000
F 0 "R?" H 1320 4046 50  0000 L CNN
F 1 "1k" H 1320 3955 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 1180 4000 50  0001 C CNN
F 3 "~" H 1250 4000 50  0001 C CNN
	1    1250 4000
	0    1    1    0   
$EndComp
$Comp
L Device:LED D?
U 1 1 618EC6EC
P 1550 4350
F 0 "D?" V 1589 4233 50  0000 R CNN
F 1 "Signal LED yellow" H 1500 4200 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 1550 4350 50  0001 C CNN
F 3 "~" H 1550 4350 50  0001 C CNN
	1    1550 4350
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 618EC6F2
P 1250 4350
F 0 "R?" H 1320 4396 50  0000 L CNN
F 1 "1k" H 1320 4305 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 1180 4350 50  0001 C CNN
F 3 "~" H 1250 4350 50  0001 C CNN
	1    1250 4350
	0    1    1    0   
$EndComp
$Comp
L Device:LED D?
U 1 1 618EF321
P 1550 4700
F 0 "D?" V 1589 4583 50  0000 R CNN
F 1 "Signal LED red" H 1500 4550 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 1550 4700 50  0001 C CNN
F 3 "~" H 1550 4700 50  0001 C CNN
	1    1550 4700
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 618EF327
P 1250 4700
F 0 "R?" H 1320 4746 50  0000 L CNN
F 1 "1k" H 1320 4655 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 1180 4700 50  0001 C CNN
F 3 "~" H 1250 4700 50  0001 C CNN
	1    1250 4700
	0    1    1    0   
$EndComp
$Comp
L Device:LED D?
U 1 1 618F1222
P 1550 5050
F 0 "D?" V 1589 4933 50  0000 R CNN
F 1 "Signal LED blue" H 1500 4900 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 1550 5050 50  0001 C CNN
F 3 "~" H 1550 5050 50  0001 C CNN
	1    1550 5050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 618F1228
P 1250 5050
F 0 "R?" H 1320 5096 50  0000 L CNN
F 1 "0" H 1320 5005 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 1180 5050 50  0001 C CNN
F 3 "~" H 1250 5050 50  0001 C CNN
	1    1250 5050
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 619002BD
P 1800 5200
F 0 "#PWR?" H 1800 4950 50  0001 C CNN
F 1 "GND" H 1805 5027 50  0000 C CNN
F 2 "" H 1800 5200 50  0001 C CNN
F 3 "" H 1800 5200 50  0001 C CNN
	1    1800 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 5200 1800 5050
Wire Wire Line
	1800 4000 1700 4000
Wire Wire Line
	1700 4350 1800 4350
Connection ~ 1800 4350
Wire Wire Line
	1800 4350 1800 4000
Wire Wire Line
	1700 4700 1800 4700
Connection ~ 1800 4700
Wire Wire Line
	1800 4700 1800 4350
Wire Wire Line
	1700 5050 1800 5050
Connection ~ 1800 5050
Wire Wire Line
	1800 5050 1800 4700
Wire Wire Line
	650  4000 1100 4000
Text Label 650  4000 0    50   ~ 0
LED_GREEN
Wire Wire Line
	1100 4350 650  4350
Wire Wire Line
	1100 4700 650  4700
Wire Wire Line
	1100 5050 650  5050
Text Label 650  4350 0    50   ~ 0
LED_YELLOW
Text Label 650  4700 0    50   ~ 0
LED_RED
Text Label 650  5050 0    50   ~ 0
LED_BLUE
Text Label 8750 3400 0    50   ~ 0
LED_GREEN
Text Label 8750 3500 0    50   ~ 0
LED_YELLOW
Text Label 8750 3600 0    50   ~ 0
LED_RED
Text Label 8750 3700 0    50   ~ 0
LED_BLUE
$EndSCHEMATC
