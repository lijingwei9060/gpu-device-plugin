IMG = registry.cn-shanghai.aliyuncs.com/casoul1/jy-gpu-plugin:$(shell date +v%Y%m%d%H%M%S)

.PHONY: build
build:
    GOOS=linux go build -o bin/jy-gpu-plugin cmd/main.go

.PHONY: build-image
build-image:
    docker build -t ${IMG} .


