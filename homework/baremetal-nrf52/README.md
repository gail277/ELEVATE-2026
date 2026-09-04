# nRF52840DK Bare Metal Template

This project provides a bare metal template for developing applications on the nRF52840 SoC using the nRF52840DK development kit. It serves as a starting point for embedded developers looking to work with this powerful Nordic Semiconductor chip without an operating system.

## Prerequisites

Before you begin, ensure you have the following tools installed:

- ARM GCC Toolchain (`arm-none-eabi-gcc`)
- GNU Make
- SEGGER J-Link Software and Documentation Pack
- nRF Command Line Tools (optional, but useful for some operations)

## Getting Started

1. Clone this repository:

   ```
   git clone https://portal.virscient.com/elevate/baremetal-nrf52.git
   cd baremetal-nrf52
   ```

   Note: HTTP is only enabled for cloning. SSH cloning is not supported.

2. Connect your nRF52840DK to your computer via USB.

3. Ensure your board is not in debug lockdown (see [Troubleshooting](#troubleshooting) if it is).

## Project Structure

- `src/main.c`: Main application code
- `src/startup.c`: Startup code and vector table
- `src/nrf52840_xxaa.ld`: Linker script
- `Makefile`: Build system configuration
- `build/`: Directory for build outputs

## Configuring the Toolchain

Before building the project, you need to update the Makefile to include the path to your ARM GCC Toolchain.

1. Download the appropriate toolchain for your system:

   - Linux: [arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz](https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz)
   - macOS: [arm-gnu-toolchain-13.3.rel1-darwin-x86_64-arm-none-eabi.pkg](https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-darwin-x86_64-arm-none-eabi.pkg)

2. Extract the toolchain on your system.

3. Open the Makefile in a text editor.

4. Locate the line that sets the `TOOLCHAIN_PATH` variable.

5. Update this line with the path to your installed toolchain. For example:

   ```makefile
   TOOLCHAIN_PATH = /path/to/your/arm-gnu-toolchain-13.3.rel1
   ```

6. Save the Makefile.

## Building the Project

To build the project, simply run:

```
make
```

This will compile the source files and generate the binary in the `build` directory.

## Programming and Debugging

This project uses SEGGER J-Link for programming and debugging. Follow these steps:

1. Start the GDB server:

   ```
   JLinkGDBServer -select USB -device nRF52840_xxAA -endian little -if SWD -noir -noLocalhostOnly -nologtofile -port 2331 -SWOPort 2332 -TelnetPort 2333
   ```

2. In a new terminal, start GDB and connect to the board:

   ```
   arm-none-eabi-gdb -ex "target remote localhost:2331" ./build/nrf52840_bare_metal.elf
   ```

3. Inside GDB, use the following commands to program and run:

   ```
   monitor reset 0
   load
   monitor reset
   continue
   ```

Note: If the nRF52840DK is connected to a windows machine, and the USB isn't able to be passed through to the VM/WSL, you will need to run JLinkGDBServer on the windows machine and connect to it from VM/WSL. Ideally USB would connect directly to the linux machine.

## Windows Users (WSL)

If you're using Windows and running the project in WSL (Windows Subsystem for Linux), your setup will depend on whether you can pass the USB connection of the nRF52840DK through to your Linux environment.

### Ideal Scenario: Direct USB Connection to Linux

Ideally, you should connect the nRF52840DK directly to your Linux machine or successfully pass it through to your VM/WSL environment. In this case, you can follow the standard setup and debugging instructions provided earlier in this README.

### Alternative Scenario: nRF52840DK Connected to Windows Host

If the nRF52840DK is connected to your Windows machine and USB passthrough to VM/WSL is not possible, follow these steps:

1. Install the ARM GCC Toolchain and Make within WSL for building the project.

2. Install SEGGER J-Link Software on your Windows host system.

3. On your Windows machine, start the GDB server:

   ```
   "C:\Program Files (x86)\SEGGER\JLink\JLinkGDBServerCL.exe" -select USB -device nRF52840_xxAA -endian little -if SWD -noir -port 2331 -SWOPort 2332 -TelnetPort 2333
   ```

4. In WSL, connect GDB to the server running on Windows:

   ```
   arm-none-eabi-gdb -ex "target remote localhost:2331" ./build/nrf52840_bare_metal.elf
   ```

5. Use GDB commands as usual for debugging.

### Building the Project

Regardless of your debugging setup, you can build the project within WSL using the standard `make` command as described in the [Building the Project](#building-the-project) section.

### Debugging Commands

- `step`: Step through code line by line
- `next`: Step over function calls
- `break [function/line number]`: Set a breakpoint
- `continue`: Resume execution
- `print [variable]`: Print the value of a variable

## Troubleshooting

### Debug Lockdown

If your nRF52840DK is in debug lockdown, you won't be able to program or debug it. To recover:

1. Install nRF Command Line Tools if you haven't already.
2. Run the following command:

```

nrfjprog --recover

```

This will unlock the device, allowing you to program and debug it.

## Resources

- [nRF52840DK Documentation](https://docs.nordicsemi.com/bundle/ug_nrf52840_dk/page/UG/dk/intro.html)
- [nRF52840 Product Specification](https://docs.nordicsemi.com/bundle/ps_nrf52840/page/keyfeatures_html5.html)
- [GNU Debugger (GDB) Manual](https://www.gnu.org/software/gdb/documentation/)
- [Video: CppCon 2015: Greg Law "Give me 15 minutes & I'll change your view of GDB"](https://www.youtube.com/watch?v=PorfLSr3DDI)
- [Using GDB with Nordic Devices](https://devzone.nordicsemi.com/guides/nrf-connect-sdk-guides/b/getting-started/posts/using-gdb-with-nordic-devices)

## Contributing

Contributions to improve this template are welcome. Please feel free to submit issues or pull requests.
