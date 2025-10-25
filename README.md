# TM4C123 uC/OS-II “TimeBomb” Demo

![EK-TM4C123GXL LaunchPad](img/pic_board.jpg)

An event-driven embedded demo running on the **EK-TM4C123GXL (Cortex-M4F)** using the **uC/OS-II real-time kernel** combined with a lightweight **Active Object framework** (included in this repository).  
It demonstrates clean separation between RTOS tasks, active objects, and asynchronous events, while maintaining full portability and readability.

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

- **Code Composer Studio (CCS)** v12 or later  
- **EK-TM4C123GXL** LaunchPad  
- **TivaWare SDK** (for low-level drivers, not bundled in repo)

### To Build:

1. **Import project**:
  - `File → Import → Code Composer Studio → CCS Projects` → select this repo’s root folder.
2. **Set build variable for TivaWare (once)**:
	-In CCS: *Project → Properties → C/C++ Build → Build Variables*:
		Add: 
		•	Name: TIVAWARE_ROOT
		•	Value: your TivaWare path (e.g. C:/ti/TivaWare_C_Series-2.2.0.295)
5. **Build and flash**:
	- Connect the LaunchPad via USB  
	- Click the debug icon or *Run → Debug* to flash and start execution.

## 💡 How It Works

This project implements a small **reactive system** using the **Active Object pattern** — each component runs as an independent task that reacts to events instead of polling hardware.

### 🔁 Main Concept
The **TimeBomb Task** models a simple countdown device:
	- When the user **presses SW1**, the TimeBomb becomes *armed* and starts the 		countdown.  
	- It then **blinks the red LED five times**, using time-based events managed by 	µC/OS-II timers.  
	- After the countdown finishes, it enters the **“boom”** state — all LEDs on.  
	- The system can then be reset manually or by restarting.

## 📄 License & Credits

	- Main application code: MIT (see `LICENSE.txt`)
	- Third-party components and their licenses: see `THIRD_PARTY_NOTICES.md`

## ✨ Author
**Alexandre Panhaleux**  
Embedded Software Engineer  
[GitHub: @alexandrephl](https://github.com/alexandrephl)
