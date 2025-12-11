# Battery Management System for 48V Lithium-Ion Battery Pack

**A comprehensive BMS solution for 48V Li-Ion battery packs with real-time monitoring and protection**

</div>

---

## Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Specifications](#-specifications)
- [Hardware Components](#-hardware-components)
- [PCB Design](#-pcb-design)
- [Software Interface](#-software-interface)
- [Installation](#-installation)
- [Usage](#-usage)
- [Results](#-results)
- [Safety](#-safety)
- [Contributing](#-contributing)
- [Acknowledgments](#-acknowledgments)

---

## Overview

This project presents a complete Battery Management System (BMS) designed for 48V Lithium-Ion battery packs. The system provides comprehensive monitoring, protection, and balancing capabilities for applications such as electric vehicles, e-bikes, and energy storage systems.

### Key Highlights

-  13S10P Configuration (130 cells total)
-  Real-time monitoring via JBD Tools software
- Active cell balancing
- Multiple protection mechanisms
- Open-source PCB design using KiCad
- Tested with 1.2kW BLDC Hub Motor

---

## Features

### Protection Systems

| Feature | Description |
|---------|-------------|
| **Overvoltage Protection** | Individual cell and pack monitoring |
| **Undervoltage Protection** | Prevents deep discharge damage |
| **Overcurrent Protection** | Charging (6.5A) and discharging limits |
| **Temperature Monitoring** | Real-time thermal tracking |
| **Cell Balancing** | Automatic balancing (±40mV threshold) |
| **Short Circuit Protection** | Immediate fault detection |
| **MOSFET Control** | Separate charge/discharge management |

### Monitoring Capabilities

- Real-time voltage for all 13 series cells
- Pack voltage measurement
- Bidirectional current sensing
- Temperature monitoring
- State of Charge (SoC) estimation
- Cycle count tracking
- Comprehensive alert system

---

## Specifications

### Battery Pack

```yaml
Voltage: 48V (46.8V nominal)
Configuration: 13S10P (13 series, 10 parallel)
Total Cells: 130 cells
Cell Type: 18650 Lithium-Ion
Cell Voltage: 3.6V nominal
Cell Capacity: 2900mAh (2.9Ah)
Pack Capacity: 29Ah
Total Energy: ~1.4kWh
```

### Load System

```yaml
Motor Type: BLDC Hub Motor
Motor Rating: 48V, 530 RPM, 1.2kW
Charger: 54.2V, 6A
Max Charging Current: 6.35A
Operating Temperature: -20°C to 60°C
```

### Protection Thresholds

| Parameter | Min | Max | Unit |
|-----------|-----|-----|------|
| Cell Voltage | 2.5 | 4.2 | V |
| Pack Voltage | 32.5 | 54.6 | V |
| Charge Current | - | 6.5 | A |
| Discharge Current | - | 30 | A |
| Temperature | -10 | 60 | °C |

---

## Hardware Components

### Main Components

```
├── BMS Protection Board
│   ├── Microcontroller (STM32/ATmega)
│   ├── Voltage Sensing ICs
│   ├── Current Sensors
│   └── UART Communication
│
├── Power Stage
│   ├── Charge MOSFET
│   ├── Discharge MOSFET
│   └── Gate Drivers
│
├── Sensors
│   ├── NTC Thermistors
│   ├── Current Shunt Resistors
│   └── Voltage Divider Networks
│
└── Load System
    ├── BLDC Hub Motor
    ├── Motor Controller
    └── Throttle Interface
```

### Bill of Materials (BOM)

| Component | Quantity | Description |
|-----------|----------|-------------|
| 18650 Li-Ion Cells | 130 | 2900mAh, 3.6V nominal |
| BMS IC | 1 | 13S protection board |
| N-Channel MOSFETs | 2 | Charge/Discharge control |
| Current Sensor | 1 | Hall effect or shunt-based |
| NTC Thermistor | 2-3 | Temperature monitoring |
| Nickel Strips | As needed | For spot welding |
| PCB | 1 | Custom designed |
| Connectors | Various | XT60, JST, etc. |

---

## PCB Design

### Design Tools

**Software:** [KiCad](https://www.kicad.org/) (Open-source EDA)

### Features
- Schematic capture with hierarchical design
- Multi-layer PCB layout
- DRC (Design Rule Check) compliant
- Gerber file generation
- 3D visualization
- SPICE simulation support

### Design Process

```mermaid
graph LR
    A[Schematic Design] --> B[Footprint Assignment]
    B --> C[PCB Layout]
    C --> D[Component Routing]
    D --> E[DRC Check]
    E --> F[Gerber Generation]
    F --> G[Manufacturing]
```

### PCB Specifications

- **Layers:** 2-4 layers
- **Dimensions:** Optimized for battery pack integration
- **Copper Weight:** 2oz for power traces
- **Finish:** HASL or ENIG
- **Files Included:**
  - Gerber files
  - Drill files
  - Pick and place files
  - BOM (Bill of Materials)

---

## Software Interface

### JBD Tools Software

#### Key Features

- **UART Communication** - Standard serial interface
- **Parameter Configuration** - Adjust protection thresholds
- **Real-time Monitoring** - Live data visualization
- **Alert System** - Protection status notifications
- **Data Logging** - Track battery performance

#### Monitored Parameters

```yaml
Cell Voltages: V1-V13 (individual)
Pack Voltage: Total voltage
Current: Charge/Discharge (A)
Temperature: Real-time (°C)
SoC: State of Charge (%)
Capacity: Remaining/Full (mAh)
Cycles: Total charge cycles
Balance Status: ON/OFF per cell
FET Status: Charge/Discharge MOSFET
Alerts: All protection triggers
```

---

## Installation

### Hardware Setup

#### 1. Battery Pack Assembly

```bash
# Materials Required
- 130x 18650 cells (matched capacity)
- Spot welding machine (5kVA recommended)
- Nickel strips (0.15mm thickness)
- Insulation sheets
- Heat shrink wrap
- BMS board
```

**Assembly Steps:**

1. **Cell Testing & Sorting**
   - Test all cells for capacity matching
   - Sort into groups with <50mAh difference
   - Charge all cells to 3.7V

2. **Spot Welding**
   - Create 10P groups using nickel strips
   - Connect 13 groups in series
   - Use proper welding current (50-80A, 0.1-0.2s)

3. **BMS Integration**
   - Connect balance wires (13 connections)
   - Connect main power terminals
   - Install temperature sensors
   - Mount BMS securely on pack

4. **Insulation & Packaging**
   - Apply insulation sheets
   - Wrap with heat shrink
   - Add terminal connectors

#### 2. Software Setup

```bash
# Download JBD Tools
# Available for Windows PC

1. Install JBD Tools software
2. Connect BMS via USB-UART adapter
3. Configure COM port settings
   - Baud rate: 9600
   - Data bits: 8
   - Stop bits: 1
   - Parity: None
```

### Configuration

```python
# Example protection parameters

Cell_OVP = 4.2V      # Overvoltage protection
Cell_UVP = 2.5V      # Undervoltage protection
Pack_OVP = 54.6V     # Pack overvoltage
Pack_UVP = 32.5V     # Pack undervoltage
Charge_OCP = 6.5A    # Charge overcurrent
Discharge_OCP = 30A  # Discharge overcurrent
Balance_Voltage = 40mV  # Balance threshold
Temp_High = 60°C     # High temperature limit
Temp_Low = -10°C     # Low temperature limit
```

---

## 📖 Usage

### Basic Operation

#### Charging

```bash
1. Connect approved charger (54.2V, 6A max)
2. Monitor charging via JBD Tools
3. Charging stops automatically when:
   - Pack voltage reaches 54.6V
   - Any cell reaches 4.2V
   - Current exceeds 6.5A
   - Temperature exceeds limits
```

#### Discharging

```bash
1. Connect load (e.g., BLDC motor)
2. Monitor discharge parameters
3. BMS will disconnect if:
   - Pack voltage < 32.5V
   - Any cell < 2.5V
   - Current exceeds limits
   - Temperature out of range
```

#### Cell Balancing

```bash
# Automatic process
- Activates when cell voltage difference > 40mV
- Balancing current: 50-100mA typical
- Status shown in JBD Tools as "ON"
- Typically occurs near full charge
```

### JBD Tools Interface

#### Main Display

```
┌─────────────────────────────────────────┐
│ Cell Voltages (V)  │  Balance │ Status  │
├─────────────────────────────────────────┤
│ Cell 1:  3.632     │          │  ✓      │
│ Cell 2:  3.631     │          │  ✓      │
│ ...                │          │         │
│ Cell 13: 3.637     │          │  ✓      │
├─────────────────────────────────────────┤
│ Pack Voltage:  47.39V                   │
│ Current:       0.00A                    │
│ Temperature:   24.3°C                   │
│ SoC:           45%                      │
│ Capacity:      12960/29000 mAh         │
│ Cycles:        22                       │
├─────────────────────────────────────────┤
│ Charge FET:    ON    Discharge FET: ON  │
└─────────────────────────────────────────┘
```

---

## 📊 Results

### Performance Validation

#### Test 1: Normal Operation
- **Pack Voltage:** 47.39V
- **Cell Voltage Range:** 3.632V - 3.667V (35mV spread)
- **Temperature:** 24°C
- **Status:** All protection systems operational 

#### Test 2: Charging Process
- **Charging Current:** 6.02A
- **Cell Voltage Range:** 3.693V - 3.737V
- **SoC:** 45% → Charging
- **Protection:** Overcurrent limit enforced at 6.5A 

#### Test 3: Cell Balancing
- **Balance Threshold:** 40mV
- **Active Balancing:** Cells 1, 5, 9 (marked "ON")
- **Pack Voltage:** 51.81V
- **SoC:** 74%
- **Result:** Successful voltage equalization 

#### Test 4: Discharging
- **Discharge Current:** -0.86A (motor at low speed)
- **Cell Voltage Range:** 3.617V - 3.658V
- **Pack Voltage:** 47.26V
- **SoC:** 45%
- **Motor Performance:** Stable operation 

### Key Achievements
 **Prototype Validation** - 4-cell design successfully scaled to 13S10P  
 **MATLAB Simulation** - Circuit analysis confirmed  
 **Protection Systems** - All safety mechanisms functional  
 **Temperature Monitoring** - Real-time tracking operational  
 **MOSFET Control** - Proper switching behavior verified  
 **Load Testing** - 1.2kW motor successfully powered  

---

## Safety

> Improper handling can result in fire, explosion, or injury.
> Always follow safety procedures.

### Safety Guidelines

#### DO:
- Use appropriate safety equipment (goggles, gloves)
- Work in well-ventilated areas
- Keep fire extinguisher nearby (Class D)
- Monitor temperature during operation
- Use manufacturer-approved chargers
- Inspect cells for damage before assembly
- Test BMS functionality before full assembly
- Store at 3.7-3.8V for long-term storage

#### DON'T:
- Exceed rated voltage or current limits
- Short circuit battery terminals
- Puncture or damage cells
- Expose to extreme temperatures
- Use damaged or swollen cells
- Charge unattended
- Bypass protection systems
- Mix cells of different capacities

### Emergency Procedures

```bash
# In case of thermal runaway:
1. Move to safe area away from flammable materials
2. Use Class D fire extinguisher
3. DO NOT use water
4. Evacuate area if fire spreads
5. Call emergency services
```

---

## Troubleshooting

### Common Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| BMS not communicating | UART connection | Check COM port, baud rate |
| Cells not balancing | Voltage difference < 40mV | Normal, wait until higher SoC |
| Charge FET off | Overvoltage detected | Check cell voltages, reset BMS |
| Discharge FET off | Undervoltage detected | Charge battery pack |
| High temperature | Excessive current | Reduce load, improve cooling |
| SoC inaccurate | Calibration needed | Full charge/discharge cycle |

---

## Contributing

Contributions are welcome! Please follow these guidelines:

### How to Contribute

1. **Fork the repository**
2. **Create a feature branch** (`git checkout -b feature/AmazingFeature`)
3. **Commit changes** (`git commit -m 'Add AmazingFeature'`)
4. **Push to branch** (`git push origin feature/AmazingFeature`)
5. **Open a Pull Request**

### Development Setup

```bash
# Clone repository
git clone https://github.com/yourusername/48v-bms.git

# Navigate to project
cd 48v-bms

# Open KiCad project
kicad bms-48v.kicad_pro
```

### Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Focus on improving the project
- Follow coding standards

---

## 📁 Project Structure

```
48v-bms/
├── hardware/
│   ├── kicad/
│   │   ├── bms-schematic.kicad_sch
│   │   ├── bms-pcb.kicad_pcb
│   │   └── libraries/
│   ├── gerber/
│   │   └── manufacturing_files/
│   └── bom/
│       └── bill_of_materials.csv
│
├── software/
│   ├── jbd-tools/
│   │   └── configuration_files/
│   ├── matlab/
│   │   └── simulation_files/
│   └── firmware/
│       └── (firmware)
│
├── README.md
```

---



## 🎓 Acknowledgments

### Institution
**Department of Electrical and Electronics Engineering**  
**BNM Institute of Technology (BNMIT)**  
Academic Year: 2021-2022

### Special Thanks
- Faculty advisors for guidance and support
- Lab technicians for equipment access
- Fellow students for collaboration
- Open-source community (KiCad, GitHub)

### References
- Battery Management System Design Standards
- IEEE Standards for Battery Testing
- Lithium-Ion Battery Safety Guidelines
- KiCad PCB Design Best Practices

---

## Contact

### Project Maintainers

- **Email:** likithm18ee022@bnmit.edu.in
- **Institution:** BNMIT, Department of EEE

<div align="center">

**Made by BNMIT EEE Department**

[⬆ Back to Top](#-battery-management-system-for-48v-lithium-ion-battery-pack)

</div>
