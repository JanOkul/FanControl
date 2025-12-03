# FanControl
FanControl is a lightweight, add-on smart controller for my remote controlled fan. It uses an ESP-32 to receive commands and control the fan wirelessly. It does not require any internal modifications or rewiring of the fan’s hardware.
It comes with a small Android application to control from your phone.

## How it works
1. The ESP-32 sets up a small HTTP server on the local network and listens for commands.
2. A client sends a blank HTTP GET request to a URL (e.g /power).
3. The server receives this request and sends out an infrared signal through an infrared LED wired to the ESP-32.
4. The fan reads these signals and powers on or off.

## Directory structure
- /esp/ - ESP-32 Code.
- /android/ - Android studio application.

## Notes
- Commands are not configurable after install; they must be changed within the code itself before building.
- The server must have a static IP address, and you must add the complete address in the Android application settings page.
- The HTTP server is completely stateless, to prevent synchronisation issues between the fan and controller.
