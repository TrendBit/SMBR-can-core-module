# SMBR-can-core-module
An abstract Smart Mobule BioReactor module that runs as a service on the Raspberry and is connected to the CAN bus like other HW modules. It provides information such as power type and power consumption of the whole device. It is primarily used so that the entire API can be unified behind the CAN bus.

# Install
```
git clone https://github.com/TrendBit/SMBR-can-core-module.git
mkdir -p build && cd build
cmake ..
sudo make install
```
Then start with `core-module` or install as service.

# Service
```bash
sudo make install-service
# Restart daemon to load new service description
sudo systemctl daemon-reload
# Enable service startup after next reboot
sudo systemctl enable core-module
# Start service now
sudo systemctl start core-module
# Check if service is running
sudo systemctl status core-module
```

# Probe 
To test the functionality it is possible to run the core-module with the `--probe` switch, which prints out a summary of the measured and detected values and exits the program immediately.

```
$ core-module --probe

5V sense: 5.043 V
Vin sense: 11.957 V
PoE sense: 0.033 V
Current sense: 0.268 A
Power draw: 3.204 W
PoE status: Disconnected
Vin status: Connected
Board temperature: 33.5 °C
Core temperature: 46.7 °C
Core load: 0.2 %
Device UID: 0x9f12c24e653c
Short ID: 0xd177
IP Address: 192.168.1.101
Hostname: smpbr
```
