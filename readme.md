Battery Management System for 48V Lithium-Ion Battery Pack
Project Overview
This project presents the design and implementation of a Battery Management System (BMS) for a 48V Lithium-Ion battery pack. The system monitors and protects the battery pack while ensuring optimal performance and safety during charging and discharging operations.
Table of Contents

System Specifications
Key Features
Hardware Components
PCB Design
Battery Pack Assembly
Load Testing
Software Interface
Results

System Specifications
Battery Pack Configuration

Voltage: 48V (46.8V nominal)
Configuration: 13 cells in series (13S), 10 cells in parallel (10P)
Total Cells: 130 cells
Cell Type: Lithium-Ion 18650
Cell Nominal Voltage: 3.6V
Cell Capacity: 2900 mAh (2.9Ah)
Total Pack Capacity: 29Ah

Load Specifications

Motor Type: BLDC Hub Motor
Motor Rating: 48V, 530 RPM, 1.2kW
Charger: 54.2V, 6A
Maximum Charging Current: 6.35A

Key Features
Protection Functions

Overvoltage Protection: Monitors individual cell and pack voltage
Undervoltage Protection: Prevents deep discharge
Overcurrent Protection: Limits charging and discharging current
Temperature Monitoring: Tracks battery temperature in real-time
Cell Balancing: Ensures uniform voltage across series cells (±40mV threshold)
Short Circuit Protection: Immediate fault detection
MOSFET Control: Separate charge and discharge FET management

Monitoring Capabilities

Real-time voltage monitoring for all 13 series cells
Pack voltage measurement
Current sensing (charge/discharge)
Temperature monitoring
State of Charge (SoC) estimation
Cycle count tracking
Alert and indication system

Hardware Components
Main Components

BMS Protection Board: Intelligent protection system with UART communication
MOSFET Switches: For charge and discharge control
Current Sensors: For accurate current measurement
Temperature Sensors: NTC thermistors for temperature monitoring
Voltage Sensing Circuit: Individual cell voltage monitoring
Microcontroller: For system control and communication

Load System

BLDC Hub Motor with integrated design
Motor Controller Unit with multiple control pins:

Reverse line
Throttle control
Theft alarm
Battery pin
Hall sensor interface
Motor phase wires



PCB Design
Design Tools

Software: KiCad (Open-source EDA tool)
Features:

Schematic capture
PCB layout
Gerber file generation
SPICE simulation support



Design Process

Component Assignment: Footprints assigned to all components
Component Arrangement: Optimized layout for thermal and electrical performance
Routing: DRC-compliant trace routing
Manufacturing Files: Gerber files, drilling files, and BOM generation

PCB Views

2D Layout with component arrangement
Trace routing visualization
3D PCB rendering for assembly verification

Battery Pack Assembly
Spot Welding Process

Machine: 5 kVA capacity, single-phase AC
Method: Resistance spot welding
Features:

Table-mounted setup
Copper wound transformer
Pedal-operated control
Copper alloy electrodes
Optional water cooling for electrodes



Assembly Features

Nickel strip connections for series/parallel configuration
Insulation sheet covering
Shrink-wrap packaging using hot gun technique
Modular pack-module-cell structure
BMS board mounting with 13 cell connections
Removable charger pin connection

Load Testing
Test Configuration
The complete system includes:

Battery pack with BMS
BLDC Hub Motor
Motor controller
Throttle control with three speed modes
Different consumption rates per mode

Software Interface
JBD Tools Software
Dongguan JiaBaiDa Technology Co. Ltd.
Features

User-friendly interface
UART communication protocol
COM port configuration
Real-time parameter monitoring
Parameter modification capability
Free software for PC

Monitored Parameters

Individual cell voltages (13 cells)
Pack voltage
Cell voltage balance status
Temperature (°C)
Charge/discharge current
State of Charge (SoC)
Remaining capacity (mAh)
Full capacity (mAh)
Cycle count
MOSFET status (charge/discharge)
Protection status indicators
Alert notifications

Parameter Settings
Configure protection thresholds:

Cell overvoltage/undervoltage limits
Pack overvoltage/undervoltage limits
Charge/discharge overcurrent limits
Temperature limits (high/low)
Balance voltage threshold (40mV)
Short circuit protection
Time delays for protection activation

Results
Successful Implementation

Prototype Development: Four-cell series configuration successfully extended to 13S10P
MATLAB Simulation: Validated for voltage and current sensing circuits
Protection Validation: All safety limits properly enforced
Temperature Monitoring: Real-time display operational
MOSFET Operation: Proper switching based on parameter thresholds

JBD Tool Displays
Normal Operation (Fig 6.1)

All 13 cell voltages displayed (3.632V - 3.667V range)
Pack voltage: 47.39V
Current: 0.00A (idle)
Temperature: 24°C
SoC: 45%
Capacity: 12960 mAh remaining / 29000 mAh full
Cycle count: 22
Date: 2022-03-01

Charging Process (Fig 6.2)

Charging current: 6.02A
SoC: 45%
Cell voltages: 3.693V - 3.737V
Pack voltage: 48.31V
Overcharge limit: 6.5A (triggers FET off and alert)

Cell Balancing (Fig 6.3)

Balance status: "ON" when cell voltage difference >40mV
Active balancing for cells 1, 5, and 9
SoC: 74%
Pack voltage: 51.81V

Discharging Process (Fig 6.4)

Discharge current: -0.86A (negative indicates discharge)
Motor running at lowest speed
Cell voltages: 3.617V - 3.658V
Pack voltage: 47.26V
SoC: 45%

Safety Features Confirmed
The BMS successfully provides protection against:

Battery system hazards
Operating mode faults
Overvoltage and overcurrent conditions
Isolation faults
High/low temperature extremes
External environmental changes
Cell/pack parameter violations

Technical Documentation
Department
Department of EEE, BNMIT
Academic Year
2021-2022
Project Components

Circuit schematic design
PCB layout and manufacturing
Battery pack fabrication
System integration
Software configuration
Load testing and validation

Safety Notes
Important Safety Information:

Lithium-ion batteries require proper handling
BMS protection must be functional before operation
Follow all charging specifications
Monitor temperature during operation
Ensure proper ventilation during charging
Do not exceed rated current limits
Use only compatible chargers

License
This project was developed for educational purposes at BNMIT, Department of Electrical and Electronics Engineering.
Acknowledgments
Special thanks to the Department of EEE, BNMIT for providing resources and guidance for this project.
