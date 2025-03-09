FROM m.daocloud.io/docker.io/library/golang:latest AS builder
# 设置工作目录
WORKDIR /app
# 复制go.mod和go.sum以提高构建效率
COPY go.mod go.sum ./
COPY pkg/ ./pkg/
COPY cmd/ ./cmd/
ENV GOPROXY=https://goproxy.cn,direct
# 下载依赖
RUN go mod download
# 将当前目录的所有文件复制到工作目录中

# 构建Go应用
# Create necessary directories and copy header files and libraries
RUN mkdir -p /usr/include/erml
RUN mkdir -p /usr/lib/
COPY ./usr/include/erml /usr/include/erml/
COPY ./usr/lib/liberml.so.1.0.0.20250226 /usr/lib/
COPY ./usr/lib/liberml_static.a /usr/lib/
RUN ln -s /usr/lib/liberml.so.1.0.0.20250226 /usr/lib/liberml.so.1
RUN ln -s /usr/lib/liberml.so.1 /usr/lib/liberml.so

# Build the project
RUN CGO_CFLAGS="-I/usr/include/erml" CGO_LDFLAGS="-L/usr/lib -lerml"  CGO_ENABLED=1 GOOS=linux go build -o bin/jy-gpu-plugin cmd/main.go



FROM m.daocloud.io/docker.io/library/ubuntu:22.04

ENV TZ=Asia/Shanghai

WORKDIR /app

COPY --from=builder /usr/lib/liberml.so.1.0.0.20250226 /usr/lib/
COPY --from=builder /usr/lib/liberml_static.a /usr/lib/
RUN ln -s /usr/lib/liberml.so.1.0.0.20250226 /usr/lib/liberml.so.1
RUN ln -s /usr/lib/liberml.so.1 /usr/lib/liberml.so
COPY --from=builder /app/bin/jy-gpu-plugin .

# 执行应用程序
ENTRYPOINT ["./jy-gpu-plugin"]