package cn.yuebo.hello240.utils;

import java.io.Closeable;
import java.io.IOException;

/**
 * @author yuebo
 * @date 2023/3/15
 */
public class StreamUtil {

    public static void close(Closeable stream) {
        if (stream != null) {
            try {
                stream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
