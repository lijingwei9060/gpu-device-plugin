package plugin

import (
	"context"
	"fmt"
	"gpu-device-plugin/pkg/common"
	"strings"

	"github.com/pkg/errors"
	"k8s.io/klog/v2"

	pluginapi "k8s.io/kubelet/pkg/apis/deviceplugin/v1beta1"
)

// GetDevicePluginOptions returns options to be communicated with Device
// Manager
func (c *GpuDevicePlugin) GetDevicePluginOptions(_ context.Context, _ *pluginapi.Empty) (*pluginapi.DevicePluginOptions, error) {
	return &pluginapi.DevicePluginOptions{PreStartRequired: true}, nil
}

// ListAndWatch returns a stream of List of Devices
// Whenever a Device state change or a Device disappears, ListAndWatch
// returns the new list
func (c *GpuDevicePlugin) ListAndWatch(_ *pluginapi.Empty, srv pluginapi.DevicePlugin_ListAndWatchServer) error {
	devs := c.dm.Devices()
	klog.Infof("find devices [%s]", String(devs))

	err := srv.Send(&pluginapi.ListAndWatchResponse{Devices: devs})
	if err != nil {
		return errors.WithMessage(err, "send device failed")
	}

	klog.Infoln("waiting for device update")
	for range c.dm.notify {
		devs = c.dm.Devices()
		klog.Infof("device update,new device list [%s]", String(devs))
		_ = srv.Send(&pluginapi.ListAndWatchResponse{Devices: devs})
	}
	return nil
}


// GetPreferredAllocation returns a preferred set of devices to allocate
// from a list of available ones. The resulting preferred allocation is not
// guaranteed to be the allocation ultimately performed by the
// devicemanager. It is only designed to help the devicemanager make a more
// informed allocation decision when possible.
func (c *GpuDevicePlugin) GetPreferredAllocation(_ context.Context, _ *pluginapi.PreferredAllocationRequest) (*pluginapi.PreferredAllocationResponse, error) {
	return &pluginapi.PreferredAllocationResponse{}, nil
}


// Allocate is called during container creation so that the Device
// Plugin can run device specific operations and instruct Kubelet
// of the steps to make the Device available in the container
func (c *GpuDevicePlugin) Allocate(_ context.Context, reqs *pluginapi.AllocateRequest) (*pluginapi.AllocateResponse, error) {
	ret := &pluginapi.AllocateResponse{}
	for _, req := range reqs.ContainerRequests {
		klog.Infof("[Allocate] received request: %v", strings.Join(req.DevicesIDs, ","))
		
		resp := pluginapi.ContainerAllocateResponse{}
		
		for _, id := range req.DevicesIDs {
			if !c.dm.DeviceExist(id) {
				return nil, fmt.Errorf("invalid allocation request for '%s': unknown device: %s", common.DeviceName, id)
			}
			d := pluginapi.DeviceSpec{}
			// Expose the device node for pod.
			d.HostPath = common.HostPathPrefix + common.DeviceName + id
			d.ContainerPath = common.ContainerPathPrefix + common.DeviceName + id
			d.Permissions = "rwm"
			resp.Devices = append(resp.Devices, &d)
		}

		d := pluginapi.DeviceSpec{}
		// Expose the device node for pod.
		d.HostPath = common.HostPathPrefix + "gcuctl"
		d.ContainerPath = common.ContainerPathPrefix + "gcuctl"
		d.Permissions = "rwm"
		resp.Devices = append(resp.Devices, &d)

		resp.Envs = map[string]string{
			common.EnvName: strings.Join(req.DevicesIDs, ","),
		}
		
		ret.ContainerResponses = append(ret.ContainerResponses, &resp)
	}
	return ret, nil
}

// PreStartContainer is called, if indicated by Device Plugin during registeration phase,
// before each container start. Device plugin can run device specific operations
// such as reseting the device before making devices available to the container
func (c *GpuDevicePlugin) PreStartContainer(_ context.Context, _ *pluginapi.PreStartContainerRequest) (*pluginapi.PreStartContainerResponse, error) {
	return &pluginapi.PreStartContainerResponse{}, nil
}