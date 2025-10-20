TM4C123 RTOS Demo with uC/OS-II (Active Object “TimeBomb”)

![Demo Screenshot or Board Photo](img/pic_board.jpg)

A compact embedded RTOS example on the EK-TM4C123GXL (Cortex-M4F) using the uC/OS-II kernel with a lightweight Active Object (AO) framework.
It demonstrates event-driven state machines, time-based transitions, and LED control triggered by debounced button inputs.

---

## 📂 Repository Layout

|  
├── Application/                 	# Your application logic (main, bsp, uc_ao...)  
|  
├── ucos2/               		    # µC/OS-II kernel sources 
|  
├── ucos2_ports/               		# µC/OS-II port for Cortex-M4 
|
├── ek-tm4c123gxl/               	# Board/Microcontroller-specific files  
|  
├── CMSIS/               		    # CMSIS core headers  
|  
├── targetConfig/                	# CCS Target Configurations  

---

##🛠 Build & Run Instructions

### Prerequisites

- **Code Composer Studio (CCS)** or GCC ARM toolchain  
- **TivaWare SDK** (not included in repo)  
- **µC/OS-II (V2.93)** included in this repo under ucos2/
- **Active Object framework** ((adapted from Quantum Leaps uC/AO))

### Importing in CCS

1. **Import project**:
  - `File → Import CCS Projects` → select this repo’s root folder.
2. **Tivaware path setting**:
	-When prompted, set build variable in Project Properties → C/C++ Build → Build Variables:
		•	Name: TIVAWARE_ROOT
		•	Value: your TivaWare path (e.g. C:/ti/TivaWare_C_Series-2.2.0.295)
2. **Include path setup**:
   - Add paths in Project Properties → CCS Build → GNU Compiler → Directories:
     ```
     ${ProjDirPath}/Application
     ${ProjDirPath}/ek-tm4c123gxl
     ${ProjDirPath}/CMSIS/include
     ${ProjDirPath}/ucos2
     ${ProjDirPath}/ucos2_ports
     ${TIVAWARE_ROOT}
     ```
3. **Build configuration**: Only one kernel compilation model is used:
✅ Include: ucos2/ucos_ii.c
🚫 Exclude: all other ucos2/os_*.c files
(This is already set in the project so it builds right away.)
4. **Linker script (if needed)**: Uses ek-tm4c123gxl/lesson.ld (custom script included).
5. **Build and flash** — connect the LaunchPad, flash via CCS.

## 📄 License & Credits

	- Main application code: MIT (see `LICENSE.txt`)
	- Third-party components and their licenses: see `THIRD_PARTY_NOTICES.md`

## ✨ Author
**Alexandre Panhaleux**  
Embedded Software Engineer  
[GitHub: @alexandrephl](https://github.com/alexandrephl)
