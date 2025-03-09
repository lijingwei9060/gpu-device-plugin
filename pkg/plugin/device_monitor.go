package plugin

import (
	"fmt"
	"strings"
	"time"

	"github.com/pkg/errors"
	pluginapi "k8s.io/kubelet/pkg/apis/deviceplugin/v1beta1"

	"gpu-device-plugin/pkg/erml"

	"k8s.io/klog/v2"
)

type DeviceMonitor struct {
	path    string
	devices map[string]*pluginapi.Device
	notify  chan struct{} // notify when device update
}

func NewDeviceMonitor(path string) *DeviceMonitor {
	return &DeviceMonitor{
		path:    path,
		devices: make(map[string]*pluginapi.Device),
		notify:  make(chan struct{}),
	}
}

func (d *DeviceMonitor) List() error {
	klog.Infoln("watching devices")

	devices, err := list()
	if err != nil {
		return err
	}

	for _, device := range devices {
		device_name := device.ID
		d.devices[device_name] = device
	}

	return nil
}


func list() ([]*pluginapi.Device, error){
	devices := make([]*pluginapi.Device, 0)
	erml.InitV2(false)
	defer erml.Shutdown()
	
	cnt, err := erml.GetDevCount()
	if err!= nil {
		return nil, errors.WithMessage(err, "get dev count failed")
	}
	for dev_idx := uint(0); dev_idx < cnt; dev_idx++ {
		handle, _ := erml.GetDeviceHandleByIndex(dev_idx)
		var devInfo *erml.DeviceInfo
		devInfo, err = handle.GetDevInfo()
		
		
		if err!= nil {
			return nil, errors.WithMessagef(err, "get dev [%d] info failed", dev_idx)
		}
		var health bool
		health, err = handle.GetDevIsHealth()

		if err!= nil {
			return nil, errors.WithMessagef(err, "get dev [%d] health failed", dev_idx)
		}

		if !health {
			klog.Infof("device [%s] is not healthy", devInfo.Name)
		}

		deviceHealth := pluginapi.Healthy
		if !health {
			deviceHealth = pluginapi.Unhealthy
		}
		devices = append(devices, &pluginapi.Device{
			ID:     fmt.Sprintf("%d", dev_idx),
			Health: deviceHealth,
		})
	}
	return devices, nil
}
// Watch device change
func (d *DeviceMonitor) Watch() error {
	klog.Infoln("watching devices")

	errChan := make(chan error)
	go func() {
		defer func() {
			if r := recover(); r != nil {
				errChan <- fmt.Errorf("device watcher panic:%v", r)
			}
		}()
		for {
			time.Sleep(time.Minute)
			newDevices, err := list()
			if err != nil {
				errChan <- err
				return
			}

			updated := false
			for _, newDevice := range newDevices {
				oldDevice, exists := d.devices[newDevice.ID]
				if !exists || oldDevice.Health != newDevice.Health {
					d.devices[newDevice.ID] = newDevice
					updated = true
				}
			}

			if updated {
				d.notify <- struct{}{}
			}
		}
	}()

	return <-errChan

}

func (d *DeviceMonitor) DeviceExist(id string) bool {
	_, ok := d.devices[id]
	return ok
}

// Devices transformer map to slice
func (d *DeviceMonitor) Devices() []*pluginapi.Device {
	devices := make([]*pluginapi.Device, 0, len(d.devices))
	for _, device := range d.devices {
		devices = append(devices, device)
	}
	return devices
}


func String(devs []*pluginapi.Device) string {
	ids := make([]string, 0, len(devs))
	for _, device := range devs {
		ids = append(ids, device.ID)
	}
	return strings.Join(ids, ",")
}