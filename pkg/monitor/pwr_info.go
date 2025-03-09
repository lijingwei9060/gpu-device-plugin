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
		pwrInfo, _ := handle.GetDevPwr()
		fmt.Println("device idx:", dev_idx)
		fmt.Println("Pwr Info", pwrInfo.Cur_Pwr_Consumption)
	}
	erml.Shutdown()
}
