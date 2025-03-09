package main

import (
	"fmt"
	"go_erml"
)

func main() {
	erml.InitV2(false)
	cnt, _ := erml.GetDevCount()
	for dev_idx := uint(0); dev_idx < cnt; dev_idx++ {
		handle, _ := erml.GetDeviceHandleByIndex(dev_idx)
		thermalInfoV2, _ := handle.GetDevTempV2()
		fmt.Println("device idx:", dev_idx)
		fmt.Println("soc temp = ", thermalInfoV2.Cur_Asic_Temp)
		fmt.Println("mem temp =  ", thermalInfoV2.Cur_Mem_Temp)
		fmt.Println("board temp = ", thermalInfoV2.Cur_Board_Temp)
	}
	erml.Shutdown()
}
