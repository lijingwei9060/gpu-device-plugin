package main

import (
	"fmt"
	"gpu-device-plugin/pkg/erml"
)

func main() {
	erml.InitV2(false)
	ver, _ := erml.GetDriverVer()
	fmt.Println("Driver Ver:  ", ver)

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

		cluster_count, _ := handle.GetClusterCount()
		fmt.Println("cluster_count = ", cluster_count)

		usage, _ := handle.GetDevDtuUsage()
		fmt.Println("dev usage = ", usage)

		state, _ := handle.GetDevState()
		fmt.Println("device state ", state)

		hwArchName, _ := handle.GetHwArchName()
		fmt.Println("hw arch name = ", hwArchName)

		major, main, _ := handle.GetDevMajorMain()
		fmt.Println("device major ", major, "main ", main)

		pci_link, _ := handle.GetPcieLinkInfo()
		fmt.Println("link speed = ", pci_link.Link_Speed)
		fmt.Println("link width = ", pci_link.Link_Width)

		pwrInfo, _ := handle.GetDevPwr()
		fmt.Println("Pwr Info", pwrInfo.Cur_Pwr_Consumption)

		thermalInfoV2, _ := handle.GetDevTempV2()
		fmt.Println("soc temp = ", thermalInfoV2.Cur_Asic_Temp)
		fmt.Println("mem temp =  ", thermalInfoV2.Cur_Mem_Temp)
		fmt.Println("board temp = ", thermalInfoV2.Cur_Board_Temp)

		uuid, _ := handle.GetDevUuid()
		fmt.Println("device uuid ", uuid)

		health, _ := handle.GetDevHealth()
		fmt.Println("device health ", health)

		health, _ = handle.GetDevIsHealth()
		fmt.Println("device health ", health)

		// healthInfo, _ := handle.HealthCheck()
		// for system_idx := uint(0); system_idx < uint(erml.Health_System_Max); system_idx++ {
		// 	fmt.Println("health system = ", healthInfo[system_idx].System)
		// 	fmt.Println("health err_cnt =  ", healthInfo[system_idx].Err_Count)
		// 	fmt.Println("health warn_cnt = ", healthInfo[system_idx].Warn_Count)
		// 	fmt.Println("health status = ", healthInfo[system_idx].Health)
		// }
	}

	erml.Shutdown()
}
