# soundTrigger Kernel Module
The soundTrigger kernel module allows you to trigger sound playback from the Linux kernel space. It provides a misc device interface for user-space applications to communicate with the kernel module to initiate sound playback.

## Features
- Creates a device node `/dev/sound_trigger`.
- Plays sound files from an absolute path specified as a module parameter.
- Allows user-space applications to open the device and trigger sound playback.
- Configurable permissions for the device node (default: 0666).

## Prerequisites
- Linux kernel headers for your current kernel version.
- A valid `.wav` file to use for sound playback.

## Build and Installation

1. Clone this repository and navigate to the directory:

   ```bash
   git clone git@github.com:yhckatherine/soundTrigger.git
   cd soundTrigger
   ```

2. Set a valid `.wav` file to use for sound playback:

   ```bash
   cp -rf sound/sound.wav /tmp/
   ```

3. Build the kernel module:

   ```bash
   cd src
   make
   ```

4. Load the module:

   ```bash
   sudo insmod soundTrigger.ko sound_file_path="/tmp/sound.wav"
   ```

5. Verify the module is loaded:

   ```bash
   lsmod | grep soundTrigger
   ```

6. Check that the device node is created:

   ```bash
   ls -l /dev/sound_trigger
   ```

## Usage

1. To trigger sound playback:

   ```bash
   sudo chmod 646 /dev/sound_trigger
   echo "play_sound" > /dev/sound_trigger
   ```

   The sound specified by `sound_file_path` will be played.

2. Remove the module when done:

   ```bash
   sudo rmmod soundTrigger
   ```

## Configuration

- The sound file to be played is specified using the `sound_file_path` module parameter. Example:

  ```bash
  sudo insmod soundTrigger.ko sound_file_path="/absolute/path/to/sound.wav"
  ```

- Modify the permissions of the device node as needed:

  ```bash
  sudo chmod 0666 /dev/sound_trigger
  ```

## Troubleshooting

- Ensure the sound file path is absolute and points to a valid `.wav` file.
- Check the kernel log for debug messages:

  ```bash
  dmesg | tail
  ```

- Make sure you have the necessary permissions to access `/dev/sound_trigger`.

---
