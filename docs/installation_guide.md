# LightOS Installation Guide

## System Requirements

Before installing LightOS, ensure your system meets the following requirements:

### Minimum Requirements
- CPU: x86_64 processor, 1 GHz or faster
- RAM: 1 GB
- Disk Space: 10 GB
- Display: VGA-compatible or HDMI display (1920x1080p supported)

### Recommended Requirements
- CPU: x86_64 processor, 2 GHz or faster (multi-core recommended)
- RAM: 4 GB
- Disk Space: 20 GB
- Display: 1920x1080p (Full HD) HDMI display (primary supported resolution)

## Installation Methods

LightOS can be installed using one of the following methods:

1. **Live USB Installation**: Boot from a USB drive and install LightOS.
2. **ISO Installation**: Boot from an ISO image and install LightOS.
3. **Network Installation**: Install LightOS over the network.
4. **Virtual Machine Installation**: Install LightOS in a virtual machine.

## Preparing for Installation

### Creating a Live USB

1. Download the LightOS installation image from the [official website](https://lightos.org/download).
2. Create a bootable USB drive using one of the following tools:
   - On Windows: Rufus, Etcher, or Universal USB Installer
   - On macOS: Etcher or dd command
   - On Linux: Etcher, dd command, or USB Creator

#### Using Rufus (Windows)
1. Download and run Rufus.
2. Insert your USB drive.
3. Select your USB drive in the "Device" dropdown.
4. Click the "SELECT" button and choose the LightOS ISO file.
5. Click "START" to create the bootable USB drive.

#### Using Etcher (Windows, macOS, Linux)
1. Download and run Etcher.
2. Click "Flash from file" and select the LightOS ISO file.
3. Select your USB drive.
4. Click "Flash" to create the bootable USB drive.

#### Using dd command (macOS, Linux)
1. Open a terminal.
2. Identify your USB drive using `lsblk` (Linux) or `diskutil list` (macOS).
3. Unmount the USB drive using `umount /dev/sdX` (Linux) or `diskutil unmountDisk /dev/diskX` (macOS).
4. Write the ISO to the USB drive using:
   ```
   sudo dd if=lightos.iso of=/dev/sdX bs=4M status=progress
   ```
   (Replace `/dev/sdX` with your USB drive identifier)

### Creating a Bootable DVD

1. Download the LightOS installation image from the [official website](https://lightos.org/download).
2. Burn the ISO file to a DVD using your preferred DVD burning software.

## Installation Process

### Booting from Installation Media

1. Insert the USB drive or DVD into your computer.
2. Restart your computer.
3. Enter the boot menu by pressing the appropriate key during startup (usually F12, F10, or Esc, depending on your system).
4. Select the USB drive or DVD from the boot menu.
5. The LightOS installer will start loading.

### Installation Steps

1. **Language Selection**: Choose your preferred language for the installation process.

2. **Keyboard Layout**: Select your keyboard layout.

3. **Network Configuration**:
   - Connect to a wired or wireless network (optional).
   - Configure network settings manually or use DHCP.

4. **Installation Type**:
   - **Full Installation**: Install LightOS as the only operating system.
   - **Dual Boot**: Install LightOS alongside another operating system.
   - **Custom Installation**: Manually configure partitions and installation options.

5. **Disk Partitioning**:
   - **Automatic Partitioning**: Let the installer create the necessary partitions.
   - **Manual Partitioning**: Create and configure partitions manually.

   Recommended partition scheme:
   - EFI System Partition (ESP): 512 MB (for UEFI systems)
   - Boot Partition: 1 GB
   - Root Partition: 20 GB (minimum)
   - Home Partition: Remaining space
   - Swap Partition: Equal to RAM size (up to 8 GB)

6. **User Account Setup**:
   - Enter your full name.
   - Choose a username.
   - Create a strong password.
   - Confirm your password.

7. **System Configuration**:
   - Set the hostname for your computer.
   - Select your timezone.
   - Choose whether to enable automatic updates.

8. **Installation Summary**:
   - Review your installation settings.
   - Click "Install" to begin the installation process.

9. **Installation Progress**:
   - The installer will copy files to your disk and configure the system.
   - This process may take 10-30 minutes depending on your hardware.

10. **Installation Complete**:
    - Remove the installation media.
    - Click "Restart" to reboot into your new LightOS installation.

## Post-Installation Setup

### First Boot

1. Log in using the username and password you created during installation.
2. The LightOS welcome screen will appear, guiding you through initial setup.

### System Updates

1. Open the terminal.
2. Run the following commands to update your system:
   ```
   update check
   update download all
   update install all
   ```

### Installing Additional Software

1. Open the terminal.
2. Use the package manager to install additional software:
   ```
   package search <package-name>
   package install <package-name>
   ```

### Setting Up User Accounts

1. Open the terminal.
2. Use the user management commands to create additional user accounts:
   ```
   security user add <username> <password> <uid> <gid> <home> <shell> <admin>
   ```

### Configuring the Firewall

1. Open the terminal.
2. Use the firewall commands to configure the firewall:
   ```
   firewall status
   firewall enable
   firewall add-rule <chain> <name> <action> <direction> <protocol> <source> <destination>
   ```

## Troubleshooting

### Boot Issues

If your system fails to boot after installation:

1. **Check Boot Order**: Ensure your system is configured to boot from the disk where LightOS is installed.
2. **Repair Boot Loader**: Boot from the installation media and select "Repair Boot Loader" from the advanced options.
3. **Check Disk Integrity**: Boot from the installation media and select "Check Disk" from the advanced options.

### Network Issues

If you experience network connectivity problems:

1. **Check Network Hardware**: Ensure your network hardware is properly connected and functioning.
2. **Configure Network Settings**: Use the network configuration tool to configure your network settings.
3. **Check Firewall Settings**: Ensure the firewall is not blocking necessary connections.

### Graphics Issues

If you experience display problems:

1. **Boot in Safe Mode**: Boot with the "Safe Graphics" option from the boot menu.
2. **Update Graphics Drivers**: Install the appropriate graphics drivers for your hardware.
3. **Configure Display Settings**: Use the display configuration tool to adjust your display settings.

## Uninstalling LightOS

If you need to uninstall LightOS:

1. **Backup Your Data**: Ensure you have backed up all important data.
2. **Boot from Another Operating System or Installation Media**: Boot from another operating system or the installation media.
3. **Remove LightOS Partitions**: Use a disk management tool to remove the LightOS partitions.
4. **Restore Boot Loader**: If you were dual-booting, restore the boot loader for your other operating system.

## Getting Help

If you encounter issues during installation or need assistance:

- Visit the [LightOS website](https://lightos.org)
- Join the [LightOS community forum](https://community.lightos.org)
- Submit bug reports at [bugs.lightos.org](https://bugs.lightos.org)
- Contact support at [support@lightos.org](mailto:support@lightos.org)

## Additional Resources

- [LightOS User Manual](lightos_manual.md)
- [LightOS Developer Guide](developer_guide.md)
- [LightOS API Reference](api_reference.md)
