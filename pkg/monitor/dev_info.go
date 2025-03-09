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
		devInfo, _ := handle.GetDevInfo()

		fmt.Println("device idx:", dev_idx)
		fmt.Println("name = ", devInfo.Name)
		fmt.Println("vendor_id = ", devInfo.Vendor_Id)
		fmt.Println("device_id =  ", devInfo.Device_Id)
		fmt.Println("domain_id =  ", devInfo.Domain_Id)
		fmt.Println("bus_id =  ", devInfo.Bus_Id)
		fmt.Println("dev_id =  ", devInfo.Dev_Id)
		fmt.Println("func_id = ", devInfo.Func_Id)
	}

	erml.Shutdown()
}
