package cn.yuebo.hello240.socket;

import android.util.Log;

import java.nio.charset.StandardCharsets;
import java.util.List;

import io.netty.bootstrap.Bootstrap;
import io.netty.buffer.ByteBuf;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.FixedLengthFrameDecoder;
import io.netty.handler.codec.string.StringDecoder;

/**
 * @author yuebo
 * @date 2023/3/17
 */
public class NettyClient {

    SocketChannel socketChannel;
    EventLoopGroup eventLoopGroup;
    Bootstrap bootstrap;
    Callback callback;

    public NettyClient() {
        this(null);
    }

    public NettyClient(Callback callback) {
        this.callback = callback;
        bootstrap = new Bootstrap();
        eventLoopGroup = new NioEventLoopGroup();
        bootstrap.group(eventLoopGroup).channel(NioSocketChannel.class).handler(new ChannelInitializer<Channel>() {
            @Override
            protected void initChannel(Channel ch) throws Exception {
                ChannelPipeline pipeline = ch.pipeline();
//                pipeline.addLast(new LoggingHandler(LogLevel.INFO));
                pipeline.addLast(new FixedLengthFrameDecoder(2));
                pipeline.addLast(new StringDecoder(StandardCharsets.UTF_8) {
                    @Override
                    protected void decode(ChannelHandlerContext ctx, ByteBuf msg, List<Object> out) throws Exception {
                        super.decode(ctx, msg, out);
                        Log.d("bobo", "decode() out=" + out);
                    }
                });
            }
        });
    }

    public void connect(String ip, int port) {
        ChannelFuture future = bootstrap.connect(ip, port);
        future.addListener(new ChannelFutureListener() {
            @Override
            public void operationComplete(ChannelFuture future) throws Exception {
                if (future.isSuccess()) {
                    Log.d("bobo", "connect success");
                    socketChannel = (SocketChannel) future.channel();
                } else {
                    Log.d("bobo", "connect failed");
                }
            }
        });
    }

    public void send(ByteBuf byteBuf) {
        if (socketChannel != null) {
            ChannelFuture future = socketChannel.writeAndFlush(byteBuf);
            future.addListener(new ChannelFutureListener() {
                @Override
                public void operationComplete(ChannelFuture future) throws Exception {
                    if (future.isSuccess()) {
                        Log.d("bobo", "send success");
                    } else {
                        Log.d("bobo", "send fail, reason=" + future.cause().getMessage());
                    }
                }
            });
        }
    }

    public void disconnect() {
        if (socketChannel != null) {
            socketChannel.close();
            socketChannel = null;
        }
    }

    public void release() {
        if (eventLoopGroup != null) {
            eventLoopGroup.shutdownGracefully();
            eventLoopGroup = null;
        }
    }

    public interface Callback {
        void onReceive(String text);
    }
}
