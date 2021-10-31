// usb_serial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// usb_test_cons.cpp : Defines the entry point for the console application.
//
// See https://stackoverflow.com/questions/40775369/usb-serial-device-with-virtual-com-port-readfile-reads-zero-bytes-if-use-cre

// See as well https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/writing-usb-device-companion-apps-for-microsoft-store

#include <windows.h>
#include <setupapi.h>
#include <initguid.h>

#include <iostream>

#include "gettypeinfo.h"


// This is the GUID for the USB device class.
// It is defined in the include file Usbiodef.h of the Microsoft Windows Driver Kit.
// See also https://msdn.microsoft.com/en-us/library/windows/hardware/ff545972(v=vs.85).aspx which
// provides basic documentation on this GUID.
DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE,  0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);

// https://blog.peter.skarpetis.com/archives/2005/04/07/getting-a-handle-on-usbprintsys/
DEFINE_GUID(GUID_DEVINTERFACE_USB_PRINTER, 0x28d78fadL, 0x5a12, 0x11D1, 0xae, 0x5b, 0x00, 0x00, 0xf8, 0x03, 0xa8, 0xc2);
// (A5DCBF10-6530-11D2-901F-00C04FB951ED)


// following define allows the method UsbSerialDevice::ListEndPoint()
// to print to standard output the list of enumerated USB devices.
#define LISTENDPOINT_OUTPUT


class UsbSerialDevice
{
public:
	// See https://docs.microsoft.com/en-us/windows/win32/ipc/named-pipe-server-using-overlapped-i-o?redirectedfrom=MSDN
	// to implement time outs for Write and for Read.
    UsbSerialDevice(const wchar_t* wszVendorIdIn = nullptr);
    ~UsbSerialDevice();
    int CreateEndPoint(const wchar_t* wszVendorId = nullptr, DWORD dwDesiredAccess = (GENERIC_READ | GENERIC_WRITE));
    void CloseEndPoint(void);
    int ListEndPoint(const wchar_t* wszVendorIdIn);
    int ReadStream(void* bString, size_t nBytes);
    int WriteStream(void* bString, size_t nBytes);
	DWORD  SetWriteTimeOut(DWORD msTimeout);
	DWORD  SetReadTimeOut(DWORD msTimeout);

    DWORD     m_dwError;          // GetLastError() for last action
    DWORD     m_dwErrorWrite;     // GetLastError() for last write
    DWORD     m_dwErrorRead;      // GetLastError() for last read
    DWORD     m_dwBytesWritten;   // number of bytes last write
    DWORD     m_dwBytesRead;      // number of bytes last read
	DWORD     m_dwWait;           // WaitForSingleObject() return value

private:
    HANDLE        m_hFile;
	OVERLAPPED    m_oOverlap;
	COMMTIMEOUTS  m_timeOut;
    const unsigned short m_idLen = 255;
    wchar_t  m_wszVendorId[255 + 1] = { 0 };
};

UsbSerialDevice::UsbSerialDevice(const wchar_t* wszVendorIdIn) :
    m_dwError(0),
    m_dwErrorWrite(0),
    m_dwErrorRead(0),
    m_dwBytesWritten(0),
    m_dwBytesRead(0),
	m_dwWait(0),
    m_hFile(INVALID_HANDLE_VALUE)
{
	memset(&m_oOverlap, 0, sizeof(m_oOverlap));
	m_oOverlap.hEvent = INVALID_HANDLE_VALUE;

	if (wszVendorIdIn != nullptr) ListEndPoint(wszVendorIdIn);
}

void UsbSerialDevice::CloseEndPoint(void )
{
    if (m_hFile && m_hFile != INVALID_HANDLE_VALUE) CloseHandle(m_hFile);
	if (m_oOverlap.hEvent && m_oOverlap.hEvent != INVALID_HANDLE_VALUE) CloseHandle(m_oOverlap.hEvent);
}

UsbSerialDevice::~UsbSerialDevice()
{
    CloseEndPoint();
}


/*
 *  Returns:  -1 - file handle is invalid
 *             0 - write failed. See m_dwErrorWrite for GetLastError() value
 *             1 - write succedded.
*/
int UsbSerialDevice::WriteStream(void* bString, size_t nBytes)
{
    SetLastError(0);
    m_dwError = m_dwErrorWrite = 0;
    m_dwBytesWritten = 0;
	m_dwWait = WAIT_FAILED;

    if (m_hFile && m_hFile != INVALID_HANDLE_VALUE) {

		BOOL  bWrite = WriteFile(m_hFile, bString, nBytes, 0, &m_oOverlap);
		m_dwError = m_dwErrorWrite = GetLastError();

		if (!bWrite && m_dwError == ERROR_IO_PENDING) {

			SetLastError(0);
			m_dwError = m_dwErrorWrite = 0;

			m_dwWait = WaitForSingleObject(m_oOverlap.hEvent, m_timeOut.WriteTotalTimeoutConstant);

			BOOL bCancel = FALSE;

			switch (m_dwWait) {
			case WAIT_OBJECT_0:  // The state of the specified object is signaled.
				break;
			case WAIT_FAILED:    // The function has failed. To get extended error information, call GetLastError.
				m_dwError = m_dwErrorWrite = GetLastError();
				bCancel = CancelIo(m_hFile);
				break;
			case WAIT_TIMEOUT:   // The time-out interval elapsed, and the object's state is nonsignaled.
			case WAIT_ABANDONED: // thread owning mutex terminated before releasing or signaling object.
				bCancel = CancelIo(m_hFile);
				m_dwError = m_dwErrorRead = ERROR_COUNTER_TIMEOUT;
				break;
			}

			bWrite = GetOverlappedResult(m_hFile, &m_oOverlap, &m_dwBytesRead, FALSE);
		}

        return bWrite;  // 0 or FALSE if failed, 1 or TRUE if succeeded.
    }

    return -1;
}

/*
 *  Returns:  -1 - file handle is invalid
 *             0 - read failed. See m_dwErrorRead for GetLastError() value
 *             1 - read succedded.
*/
int UsbSerialDevice::ReadStream(void* bString, size_t nBytes)
{
	SetLastError(0);
    m_dwError = m_dwErrorRead = 0;
    m_dwBytesRead = 0;
	m_dwWait = WAIT_FAILED;

    if (m_hFile && m_hFile != INVALID_HANDLE_VALUE) {

		BOOL  bRead = ReadFile(m_hFile, bString, nBytes, &m_dwBytesRead, &m_oOverlap);
		m_dwError = m_dwErrorRead = GetLastError();

		if (!bRead && m_dwError == ERROR_IO_PENDING) {
			SetLastError(0);
			m_dwError = m_dwErrorRead = 0;

			m_dwWait = WaitForSingleObject(m_oOverlap.hEvent, m_timeOut.ReadTotalTimeoutConstant);

			BOOL bCancel = FALSE;

			switch (m_dwWait) {
			case WAIT_OBJECT_0:  // The state of the specified object is signaled.
				break;
			case WAIT_FAILED:    // The function has failed. To get extended error information, call GetLastError.
				m_dwError = m_dwErrorWrite = GetLastError();
				bCancel = CancelIo(m_hFile);
				break;
			case WAIT_TIMEOUT:   // The time-out interval elapsed, and the object's state is nonsignaled.
			case WAIT_ABANDONED: // thread owning mutex terminated before releasing or signaling object.
				bCancel = CancelIo(m_hFile);
				m_dwError = m_dwErrorRead = ERROR_COUNTER_TIMEOUT;
				break;
			}

			bRead = GetOverlappedResult(m_hFile, &m_oOverlap, &m_dwBytesRead, FALSE);
		}

        return bRead;  // 0 or FALSE if failed, 1 or TRUE if succeeded.
    }

    return -1;
}

int UsbSerialDevice::ListEndPoint(const wchar_t* wszVendorIdIn)
{
    m_dwError = ERROR_INVALID_HANDLE;

	if (wszVendorIdIn == nullptr) return 0;

    HDEVINFO    hDevInfo;

	// we need to make sure the vendor and product id codes are in lower case
	// as this is needed for the CreateFile() function to open the connection
	// to the USB device correctly. this lower case conversion applies to
	// any alphabetic characters in the identifier.
	//
	// for example "VID_0FE6&PID_811E" must be converted to "vid_0fe6&pid_811e"

	wchar_t  wszVendorId[256] = { 0 };

	for (unsigned short i = 0; i < 255 && (wszVendorId[i] = towlower(wszVendorIdIn[i])); i++);

    // We will try to get device information set for all USB devices that have a
    // device interface and are currently present on the system (plugged in).
    hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_USB_DEVICE, NULL, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
    if (hDevInfo != INVALID_HANDLE_VALUE)
    {
        DWORD    dwMemberIdx;
        BOOL     bContinue = TRUE;
        SP_DEVICE_INTERFACE_DATA         DevIntfData;

        // Prepare to enumerate all device interfaces for the device information
        // set that we retrieved with SetupDiGetClassDevs(..)
        DevIntfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        dwMemberIdx = 0;

        // Next, we will keep calling this SetupDiEnumDeviceInterfaces(..) until this
        // function causes GetLastError() to return  ERROR_NO_MORE_ITEMS. With each
        // call the dwMemberIdx value needs to be incremented to retrieve the next
        // device interface information.
        for (BOOL bContinue = TRUE; bContinue; ) {
            PSP_DEVICE_INTERFACE_DETAIL_DATA  DevIntfDetailData;
            SP_DEVINFO_DATA    DevData;
            DWORD  dwSize;

            dwMemberIdx++;
            SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &GUID_DEVINTERFACE_USB_DEVICE, dwMemberIdx, &DevIntfData);

            if (GetLastError() == ERROR_NO_MORE_ITEMS) break;

            // As a last step we will need to get some more details for each
            // of device interface information we are able to retrieve. This
            // device interface detail gives us the information we need to identify
            // the device (VID/PID), and decide if it's useful to us. It will also
            // provide a DEVINFO_DATA structure which we can use to know the serial
            // port name for a virtual com port.

            DevData.cbSize = sizeof(DevData);

            // Get the required buffer size. Call SetupDiGetDeviceInterfaceDetail with
            // a NULL DevIntfDetailData pointer, a DevIntfDetailDataSize
            // of zero, and a valid RequiredSize variable. In response to such a call,
            // this function returns the required buffer size at dwSize.

            SetupDiGetDeviceInterfaceDetail(hDevInfo, &DevIntfData, NULL, 0, &dwSize, NULL);

            // Allocate memory for the DeviceInterfaceDetail struct. Don't forget to
            // deallocate it later!
            DevIntfDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
            DevIntfDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &DevIntfData, DevIntfDetailData, dwSize, &dwSize, &DevData))
            {
                if (wcsstr(DevIntfDetailData->DevicePath, wszVendorId)) {
                    wcscpy_s(m_wszVendorId, DevIntfDetailData->DevicePath);
                }
#if defined(LISTENDPOINT_OUTPUT)
                printf("  Device Path: %S\n", DevIntfDetailData->DevicePath);
                GetMoreInformation(hDevInfo, DevData);
#endif
            }

            HeapFree(GetProcessHeap(), 0, DevIntfDetailData);
        }

        SetupDiDestroyDeviceInfoList(hDevInfo);
    }

    return 0;
}

int UsbSerialDevice::CreateEndPoint(const wchar_t* wszVendorIdIn, DWORD dwDesiredAccess)
{
    if (wszVendorIdIn) {
		ListEndPoint(wszVendorIdIn);
    }

    m_dwError = ERROR_INVALID_HANDLE;

    // Finally we can start checking if we've found a useable device,
    // by inspecting the DevIntfDetailData->DevicePath variable.
    //
    // The DevicePath looks something like this for a Brecknell 67xx Series Serial Scale
    // \\?\usb#vid_1a86&pid_7523#6&28eaabda&0&2#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
    //
    // The VID for a particular vendor will be the same for a particular vendor's equipment.
    // The PID is variable for each device of the vendor.
    //
    // As you can see it contains the VID/PID for the device, so we can check
    // for the right VID/PID with string handling routines.

    // See https://github.com/Microsoft/Windows-driver-samples/blob/master/usb/usbview/vndrlist.h

    // See https://blog.peter.skarpetis.com/archives/2005/04/07/getting-a-handle-on-usbprintsys/
    // which describes a sample USB thermal receipt printer test application.

	SetLastError(0);
	m_hFile = CreateFile(m_wszVendorId, dwDesiredAccess, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, 0);
    if (m_hFile == INVALID_HANDLE_VALUE) {
        m_dwError = GetLastError();
//        wprintf(_T("   CreateFile() failed. GetLastError() = %d\n"), m_dwError);
    }
    else {
		m_oOverlap.hEvent = CreateEvent(
			NULL,    // default security attribute 
			TRUE,    // manual-reset event 
			TRUE,    // initial state = signaled 
			NULL);   // unnamed event object 

		GetCommTimeouts(m_hFile, &m_timeOut);
        m_timeOut.ReadIntervalTimeout = 0;
        m_timeOut.ReadTotalTimeoutMultiplier = 0;
        m_timeOut.ReadTotalTimeoutConstant = 5000;
		m_timeOut.WriteTotalTimeoutMultiplier = 0;
		m_timeOut.WriteTotalTimeoutConstant = 5000;
		SetCommTimeouts(m_hFile, &m_timeOut);
		m_dwError = 0;   // GetLastError();
		return 1;
    }

    return 0;
}

DWORD  UsbSerialDevice::SetWriteTimeOut(DWORD msTimeout)
{
	DWORD  dwTimeOut = 0;

	if (m_hFile && m_hFile != INVALID_HANDLE_VALUE) {
		GetCommTimeouts(m_hFile, &m_timeOut);
		dwTimeOut = m_timeOut.WriteTotalTimeoutConstant;
		m_timeOut.WriteTotalTimeoutConstant = msTimeout;
		SetCommTimeouts(m_hFile, &m_timeOut);
	}

	return dwTimeOut;
}

DWORD  UsbSerialDevice::SetReadTimeOut(DWORD msTimeout)
{
	DWORD  dwTimeOut = 0;

	if (m_hFile && m_hFile != INVALID_HANDLE_VALUE) {
		GetCommTimeouts(m_hFile, &m_timeOut);
		dwTimeOut = m_timeOut.ReadTotalTimeoutConstant;
		m_timeOut.ReadTotalTimeoutConstant = msTimeout;
		SetCommTimeouts(m_hFile, &m_timeOut);
	}

	return dwTimeOut;
}

int main(int argc, const TCHAR* argv[])
{
    wchar_t scaleVidPid[] = L"vid_1a86&pid_7523";      // USB vendor id of scale
    wchar_t printerVidPid[] = L"VID_0FE6&PID_811E";    // USB vendor id of ACE thermal printer. 
	wchar_t printerVidPid2[] = L"vid_0404&pid_0312";   // USB vendor id of NCR 7197 thermal printer.

    UsbSerialDevice  myDev(printerVidPid);

    myDev.CreateEndPoint(nullptr, (GENERIC_READ | GENERIC_WRITE));
    switch (myDev.m_dwError) {
    case 0:
        // no error so just ignore.
        break;
    case ERROR_FILE_NOT_FOUND:
        wprintf(L"   CreateFile() failed. GetLastError() = %d\n      ERROR_FILE_NOT_FOUND: File or device not found.\n", myDev.m_dwError);
        break;
    case ERROR_PATH_NOT_FOUND:
        wprintf(L"   CreateFile() failed. GetLastError() = %d\n      ERROR_PATH_NOT_FOUND: The path is invalid.\n      CreateFile() failed?\n", myDev.m_dwError);
        break;
    case ERROR_ACCESS_DENIED:
        wprintf(L"   CreateFile() failed. GetLastError() = %d\n      ERROR_ACCESS_DENIED: Access to device is denied.\n      Is it already in use?\n", myDev.m_dwError);
        break;
    case ERROR_GEN_FAILURE:
        wprintf(L"   CreateFile() failed. GetLastError() = %d\n      ERROR_GEN_FAILURE: A device attached to the system is not functioning.\n      Is it an HID?\n", myDev.m_dwError);
        break;
    case ERROR_INVALID_HANDLE:
        wprintf(L"   CreateFile() failed. GetLastError() = %d\n      ERROR_INVALID_HANDLE: The handle is invalid.\n      CreateFile() failed?\n", myDev.m_dwError);
        break;
	case ERROR_INVALID_PARAMETER:
		wprintf(L"   CreateFile() failed. GetLastError() = %d\n      ERROR_INVALID_PARAMETER: The handle is invalid.\n      CreateFile() failed?\n", myDev.m_dwError);
		break;
    default:
        wprintf(L"   CreateFile() failed. GetLastError() = %d\n", myDev.m_dwError);
        break;
    }

	wprintf(L"\n\nSleep for 5 seconds.\n");

	Sleep(5000);

	wprintf(L"Continuing.\n\n");

    if (myDev.m_dwError == 0) {
        char   reqWeight[] = "\x1d!\x00this is a line\n\x1d!\x01this is another line\n\x1d!\x11 and this is a third line\n";    // "W\r";
        char   reqPaperCut[] = "\n\n\n\n\n\x1bm";  // feed lines then paper cut.
		char   reqPrinterStatus[] = "\x1dr1";      // request printer status
		unsigned char uchPrinterStatus[8] = { 0 };   // printer status response
		bool   bPrinterOk = false;
        char   resWeight[256] = { 0 };

		wprintf(L"    Request printer status.\n");

		// get printer status
		if (myDev.WriteStream(reqPrinterStatus, sizeof(reqPrinterStatus)/sizeof(reqPrinterStatus[0])) == 1) {
			wprintf(L"    Sent request now get response.\n");
			myDev.ReadStream(uchPrinterStatus, sizeof(uchPrinterStatus));
			wprintf(L"   ReadStream() status uchPrinterStatus 0x%2.2x 0x%2.2x. GetLastError() = %d\n", uchPrinterStatus[0], uchPrinterStatus[1], myDev.m_dwErrorRead);
			bPrinterOk = true;
		} else {
			switch (myDev.m_dwErrorWrite) {
			case ERROR_INVALID_FUNCTION:
				wprintf(L"   WriteStream() failed reqPrinterStatus. GetLastError() = %d\n      ERROR_INVALID_FUNCTION: A parameter is invalid.\n", myDev.m_dwErrorWrite);
				break;
			case ERROR_FILE_NOT_FOUND:
				wprintf(L"   WriteStream() failed reqPrinterStatus. GetLastError() = %d\n      ERROR_FILE_NOT_FOUND: A parameter is invalid.\n", myDev.m_dwErrorWrite);
				break;
			case ERROR_PATH_NOT_FOUND:
				wprintf(L"   WriteStream() failed reqPrinterStatus. GetLastError() = %d\n      ERROR_PATH_NOT_FOUND: A parameter is invalid.\n", myDev.m_dwErrorWrite);
				break;
			case ERROR_ACCESS_DENIED:
				wprintf(L"   WriteStream() failed reqPrinterStatus. GetLastError() = %d\n      ERROR_ACCESS_DENIED: A parameter is invalid.\n", myDev.m_dwErrorWrite);
				break;
			case ERROR_INVALID_HANDLE:
				wprintf(L"   WriteStream() failed reqPrinterStatus. GetLastError() = %d\n      ERROR_INVALID_HANDLE: A parameter is invalid.\n", myDev.m_dwErrorWrite);
				break;
			case ERROR_INVALID_PARAMETER:
				wprintf(L"   WriteStream() failed reqPrinterStatus. GetLastError() = %d\n      ERROR_INVALID_PARAMETER: A parameter is invalid.\n", myDev.m_dwErrorWrite);
				break;
			case ERROR_OPERATION_ABORTED:
				wprintf(L"   WriteStream() failed. GetLastError() = %d\n      ERROR_OPERATION_ABORTED: CancelIo() called.\n", myDev.m_dwErrorWrite);
				break;
			default:
				wprintf(L"    WriteStream() failed reqPrinterStatus. GetLastError() = %d.\n", myDev.m_dwErrorWrite);
				break;
			}
		}
		
		if (bPrinterOk) {
			if (myDev.WriteStream(reqWeight, sizeof(reqWeight) / sizeof(reqWeight[0])) != 1) {
				switch (myDev.m_dwErrorWrite) {
				case ERROR_ACCESS_DENIED:
					wprintf(L"   WriteStream() failed. GetLastError() = %d\n      ERROR_ACCESS_DENIED: check connectivity.\n", myDev.m_dwErrorWrite);
					break;
				case ERROR_INVALID_PARAMETER:
					wprintf(L"   WriteStream() failed. GetLastError() = %d\n      ERROR_INVALID_PARAMETER: A parameter is invalid.\n", myDev.m_dwErrorWrite);
					break;
				case ERROR_OPERATION_ABORTED:
					wprintf(L"   WriteStream() failed. GetLastError() = %d\n      ERROR_OPERATION_ABORTED: CancelIo() called.\n", myDev.m_dwErrorWrite);
					break;
				default:
					wprintf(L"    WriteStream() failed. GetLastError() = %d.\n", myDev.m_dwErrorWrite);
					break;
				}
			}
			Sleep(100);
			if (myDev.WriteStream(reqPaperCut, strlen(reqPaperCut)) != 1) {
				switch (myDev.m_dwErrorWrite) {
				case ERROR_INVALID_PARAMETER:
					wprintf(L"   WriteStream() failed. GetLastError() = %d\n      ERROR_INVALID_PARAMETER: A parameter is invalid.\n", myDev.m_dwErrorWrite);
					break;
				case ERROR_OPERATION_ABORTED:
					wprintf(L"   WriteStream() failed. GetLastError() = %d\n      ERROR_OPERATION_ABORTED: CancelIo() called.\n", myDev.m_dwErrorWrite);
					break;
				default:
					wprintf(L"    WriteStream() failed. GetLastError() = %d.\n", myDev.m_dwErrorWrite);
					break;
				}
			}
		}
    }

    return 0;
}