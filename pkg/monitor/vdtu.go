package main

import (
	"fmt"
	"time"
	"go_erml"
)

func main() {
	// vdtu_usage()
	// get_vdtu_count()
	vdtu_mem()
}

func get_vdtu_count() {

	erml.InitV2(false)
	cnt, _ := erml.GetDevCount()
	for dev_idx := uint(0); dev_idx < cnt; dev_idx++ {
		hdl, _ := erml.GetDeviceHandleByIndex(dev_idx)
		fmt.Println("vDTU numbers")
		cnt, _ := hdl.GetVdevCount()
		fmt.Println(cnt)
	}
	erml.Shutdown()
}

func vdtu_usage() {
	no_driver := false
	erml.InitV2(no_driver)
	cnt, _ := erml.GetDevCount()
	for dev_idx := uint(0); dev_idx < cnt; dev_idx++ {
		hdl, _ := erml.GetDeviceHandleByIndex(dev_idx)
		vdev_cnt, _ := hdl.GetVdevCount()
		for tmp_idx := uint(0); tmp_idx < vdev_cnt; tmp_idx++ {
			vdev_idx := dev_idx * 4 + tmp_idx
			usage, _ := hdl.GetVdevDtuUsage(vdev_idx)
			fmt.Println("vdtu_usage", vdev_idx)
			fmt.Println(usage)
			time.Sleep(2 * time.Second)
		}
	}
	erml.Shutdown()
}

func vdtu_mem() {
	no_driver := false
	erml.InitV2(no_driver)
	cnt, _ := erml.GetDevCount()
	for dev_idx := uint(0); dev_idx < cnt; dev_idx++ {
		hdl, _ := erml.GetDeviceHandleByIndex(dev_idx)
		vdev_cnt, _ := hdl.GetVdevCount()
		for tmp_idx := uint(0); tmp_idx < vdev_cnt; tmp_idx++ {
			vdev_idx := dev_idx * 4 + tmp_idx
			memInfo, _ := hdl.GetVdevDtuMem(vdev_idx)
			fmt.Println("vdtu_mem", vdev_idx)
			fmt.Println(memInfo.Mem_Total_Size)
			fmt.Println(memInfo.Mem_Used)
			time.Sleep(2 * time.Second)
		}
	}
	erml.Shutdown()
}