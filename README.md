# Malcolm: ARP Spoofing Utility for Linux

Malcolm is an ARP spoofing utility for Linux that allows you to perform man-in-the-middle (MITM) attacks on local networks. By manipulating the Address Resolution Protocol (ARP), Malcolm can intercept and modify network traffic between two parties, enabling various attack scenarios such as eavesdropping, traffic manipulation, and denial of service (DoS) attacks.


## Features

- ARP spoofing for targeted or broadcast attacks
- MITM attack capabilities
- Compatible with Linux operating systems

## Installation

To install `malcolm`, clone the repository and compile the source code:

```bash
git clone https://github.com/SebastienWae/malcolm.git
cd malcolm
make
```

## Usage

To use Malcolm, you need to run the compiled binary with root privileges. The basic usage is as follows:

```bash
sudo ./malcolm <source_ip> <source_mac> <target_ip> <target_mac>
```

For example, to perform an ARP spoofing attack between a victim machine (10.0.0.15) and a server (10.0.0.20), you can use the following command:

```bash
sudo ./malcolm 10.0.0.20 aa:bb:cc:dd:ee:ff 10.0.0.15 11:22:33:44:55:66
```

## Detection and Prevention
To detect ARP spoofing attacks, you can use the arp -a command to display the ARP table and check for multiple IP addresses with the same MAC address. You can also use tools like Wireshark to analyze network traffic and detect suspicious activities.

To prevent ARP spoofing attacks, consider using security mechanisms like dynamic ARP inspection (DAI) on network switches or implementing anti-spoofing techniques on individual hosts or network equipment (en.wikipedia.org).

## Disclaimer
This tool is for educational purposes only. Using it for unauthorized network activities is illegal and unethical. Always obtain permission from the network owner before performing any ARP spoofing activities.

## License

Malcolm is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

