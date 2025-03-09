package main

import (
	"go_erml"
	"fmt"
)

func main() {
	erml.InitV2(false)
	cnt, _ := erml.GetDevCount()
	for dev_idx := uint(0); dev_idx < cnt; dev_idx++ {
		handle, _ := erml.GetDeviceHandleByIndex(dev_idx)
		port_num, _ := handle.GetEslPortNum()
		for port_idx := uint(0); port_idx < port_num; port_idx++ {
			port_info, _ := handle.GetEslPortInfo(port_idx)
			if port_info.Connected != 0 {
				fmt.Println("device idx: ", dev_idx, "port idx: ", port_idx, " connected!")
				fmt.Println("vendor_id = ", port_info.Vendor_Id)
				fmt.Println("device_id = ", port_info.Device_Id)
				fmt.Println("domain_id = ", port_info.Domain_Id)
				fmt.Println("bus_id = ", port_info.Bus_Id)
				fmt.Println("dev_id = ", port_info.Dev_Id)
				fmt.Println("func_id = ", port_info.Func_Id)
				fmt.Println("port_id = ", port_info.Port_Id)
				fmt.Println("port_type = ", port_info.Port_Type)

				fmt.Println("remote card_id = ", port_info.Remote_Card_Id)
				fmt.Println("remote vendor_id = ", port_info.Remote_Vendor_Id)
				fmt.Println("remote device_id = ", port_info.Remote_Device_Id)
				fmt.Println("remote domain_id = ", port_info.Remote_Domain_Id)
				fmt.Println("remote bus_id = ", port_info.Remote_Bus_Id)
				fmt.Println("remote dev_id = ", port_info.Remote_Dev_Id)
				fmt.Println("remote func_id = ", port_info.Remote_Func_Id)
				fmt.Println("remote port_id = ", port_info.Remote_Port_Id)
				fmt.Println("remote port_type = ", port_info.Remote_Port_Type)
			} else {
				fmt.Println("device idx: ", dev_idx, "port idx: ", port_idx, " connected!")
			}
		}
	}

	erml.Shutdown()
}
