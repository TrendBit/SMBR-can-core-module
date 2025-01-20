# SMBR-can-core-module
An abstract Smart Mobule BioReactor module that runs as a service on the Raspberry and is connected to the CAN bus like other HW modules. It provides information such as power type and power consumption of the whole device. It is primarily used so that the entire API can be unified behind the CAN bus.

# Install
```
git clone https://github.com/TrendBit/SMBR-can-core-module.git
mkdir -p build && cd build
cmake ..
sudo make install
```

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
