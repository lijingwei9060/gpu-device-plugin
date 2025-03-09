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
		thermalInfo, _ := handle.GetDevTemp()
		fmt.Println("device idx:", dev_idx)
		fmt.Println("soc temp = ", thermalInfo.Cur_Dev_Temp)
		fmt.Println("hbm0 temp =  ", thermalInfo.Cur_Hbm0_Temp)
		fmt.Println("hbm1 temp = ", thermalInfo.Cur_Hbm1_Temp)
	}
	erml.Shutdown()
}
