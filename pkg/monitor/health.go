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
		healthInfo, _ := handle.HealthCheck()
		for system_idx := uint(0); system_idx < uint(erml.Health_System_Max); system_idx++ {
			fmt.Println("device idx:", dev_idx)
			fmt.Println("health system = ", healthInfo[system_idx].System)
			fmt.Println("health err_cnt =  ", healthInfo[system_idx].Err_Count)
			fmt.Println("health warn_cnt = ", healthInfo[system_idx].Warn_Count)
			fmt.Println("health status = ", healthInfo[system_idx].Health)
		}
	}

	erml.Shutdown()
}
