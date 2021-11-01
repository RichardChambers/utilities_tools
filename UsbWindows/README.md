This folder contains some experiments with USB on the Windows platform.

I'm looking into being able to connect to a thermal receipt printer over a USB connection
rather than a [Serial Port connection](https://en.wikipedia.org/wiki/Serial_port)
(physical COM port or a virtual COM port).

The purpose of this material is to develop some knowledge and body of code for being
able to interact with a USB device such as a thermal receipt printer or a scale or
other USB connected serial device that does not create a Virtual Serial Port when
the USB cable is connected to a Windows PC.

### Basics of the code

Using the Windows API with USB devices for communicating with a serial communications
devices involves overcoming a couple of issues that are handled more easily with
the Serial Port interface (COM1:, etc.).

The Serial Port interface offers a couple of functions, `GetCommTimeouts()` and
`SetCommTimeouts()` using the `COMMTIMEOUTS` structure, which provide a way to
manage the communication with a serial communications device using an [RS-232
standard](https://en.wikipedia.org/wiki/RS-232) type of signaling with time outs,
data lines, and control lines. Using
the [`ReadFile()`](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile)
and [`WriteFile()`](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile)
with a Serial Port provides the RS-232 standard
signaling along with serial communication over a cable between two RS-232 communication
devices.

The USB approach is more of a file model of communication of sending and receiving
a stream of bytes. The USB approach doesn't have the RS-232 standard type of signaling
and using the `ReadFile()` and `WriteFile()` Windows API functions are much like using
these functions to read and write from a file or a pipe or similar byte oriented data source/sink.

However it is possible to implement a time out functionality with the Windows API
by using overlapped I/O along with one of the wait functions -
[`WaitForSingleObject()`](https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject) or
[`WaitForMultipleObjects()`](https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects) - along with a specified time out.

### Code and files

The `main()` and entry point for this console application is in the file `usb_serial.cpp`. This file
contains a C++ class, `UsbSerialDevice` which provides the basic Serial Communications device interface
that allows the following:
 - `ListEndPoint()` provides a way to locate a USB path by specifying a vendor id and product id
 - `CreateEndPoint()` provides a way to open a connection to a USB path with the `CreateFile()` Windows API function
 - `ReadStream()` provides a way to read from a USB connection with a time out
 - `WriteStream()` proivdes a way to write to a USB connection with a time out

The I/O methods, `ReadStream()` and `WriteStream()`, block and return once the I/O completes or there is a time out.

The file `GetTypeInfo.c` contains the code to enumerate through the various USB device data structures and print
to Standard Out some of that information. This code is not required for the `UsbSerialDevice` class to actually
use the Windows API for communicating with a USB device. There is a defined constant, `LISTENDPOINT_OUTPUT`, in
file `usb_serial.cpp` which if commented out will remove the printed listing code of `GetTypeInfo.c`.
