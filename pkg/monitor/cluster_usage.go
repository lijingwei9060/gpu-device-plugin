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
		cluster_count, _ := handle.GetClusterCount()

		for cluster_idx := uint(0); cluster_idx < cluster_count; cluster_idx++ {
			cluster_usage, _ := handle.GetClusterUsage(cluster_idx)
			fmt.Println("device idx:", dev_idx)
			fmt.Println("cluster idx:", cluster_idx)
			fmt.Println("cluster_usage = ", cluster_usage)
		}
	}

	erml.Shutdown()
}
