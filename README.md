# Local Web Interface for LED strips controlled by ESP microcontroller
An easy to use web iterface for adressable LED light strips controlled by an ESP microcontroller

| Desktop | Mobile |
| ------- | ------ |
| <img width="1122" height="607" alt="Web interface on desktop devices" src="https://github.com/user-attachments/assets/713e594d-de61-4644-b553-2db95d87b55d" /> | <img width="404" height="778" alt="Web interface on mobile devices" src="https://github.com/user-attachments/assets/c2683450-f76c-4fa0-a3a6-2dba9d201d40" /> |

# Features 
- Zero config `.local` adress using [mDNS](https://en.wikipedia.org/wiki/Multicast_DNS)
- Easy to use web interface
  - Mobile friendly
  - The web interface allows multiple
 concurrent connections and syncs them via websocket
- Light strip animations

# Configuration
You need to configure some parameters in `config.h`
- `ssid`: The SSID of your wifi network
- `password`: The password of your wifi network
- `mdnsHostname`: The local hostname you want this LED strip to have
  - You will be able to reach the web interface at `http://{mdnsHostname}.local`
- `LED_COUNT`: The amount of LEDs your adressable LED strip has
- `LED_PIN`: The pin where the data channel of your LED strip is connected to your ESP

# Installation
I use [PlatformIO](https://platformio.org/) to compile the code and upload it to my ESP 8266.
If you use a different ESP model it could be neccessary to add a new PlatformIO configuration for you specific board in `platformio.ini`.
Once configured it should be pretty straight forward to push the code to your ESP using PlatformIO.

# Startup
- Upon providing power to the ESP, the LED strip should glow blue
- While glowing blue, the ESP will try to connect to your wifi and initialize mDNS
  - This should not take very long. If it does, your WIFI SSID or password are probably wrong. You can take a look at the logs to see where it gets stuck
- Upon successfull connection and initialization the LED strip will turn white
- You can now connect to the web interface at `http://{mdnsHostname}.local`
