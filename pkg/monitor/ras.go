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
		EccInfo, _ := handle.GetDevEccStatus()
		fmt.Println("device idx:", dev_idx)
		fmt.Println("EccStatus Enabled = ", EccInfo.Enabled)
		fmt.Println("EccStatus Pending =  ", EccInfo.Pending)
		fmt.Println("EccStatus Pdblack = ", EccInfo.Pdblack)
		fmt.Println("EccStatus Ecnt_sb = ", EccInfo.Ecnt_sb)
		fmt.Println("EccStatus Ecnt_db = ", EccInfo.Ecnt_db)
	}

	erml.Shutdown()
}
