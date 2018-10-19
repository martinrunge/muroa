Theory of operation
===================



Time synchonisation
-------------------


To perform some tasks on several computers at the same time, they all need a common clock source. Within a single computer, you can use the system clock as common source for serveral tasks e.g. play back audio and video and keep them in sync.

When talking about networked computers, the clock source has to be network tranparent. There are several ways to get the exact time over a network:

NTP
---

The Network Time Protocol is often used to set the clock over the internet. Usually you will use your national reference clock or a receiver as timesouce, or a pool of tim servers. The design of NTP aiming to be as accurate as possible over a long time and over long and possibly slow and high latency network connections.

Advantages

* NTP is widely accepted and used.
* There are several well tested implementations for all mayor operating systems available.
* keeps the system clocks in sync → no application support needed

Disadvantages

* quite "hard" to set up
* Takes about 5 minutes to start up

Wikipedia article on NTP: https://en.wikipedia.org/wiki/Network_Time_Protocol

PTP
---

The Precision Time Protocol is designed to keep the clocks on computers in local network in sync. It can automatically choose a reference clock withoin a network and keep all other cloks in sync to it. It relys much more on a stable network connection than NTP but starts up immediatly and can exploit some properties of local networks to achieve achieve much better accuracy.

Advantages

* can be easy to set up
* a server can start serving immediatly after startup

Disadvantages

* PTP is quite new, not many implementations avalable yet, not well known yet
* PTP relies on the quality of a local area network and does not cope with network error like NTP does
* same abbreviation as the Picture Transfer Protocol :-)

Wikipedia article on PTP: https://en.wikipedia.org/wiki/Precision_Time_Protocol

GPS
---

The Global Positioning System can be used as a very exact time source, too. But to use it, you will need a GPS receiver on each computer and you need to see the sky.

Can be used to keep base stations of mobile networks in sync, but within a LAN, it is more usable as time source for a reference clock.
