package cn.yuebo.hello240.adapter;

/**
 * @author yuebo
 * @date 2023/3/24
 */
public class SpinnerItem {
    private String title;
    private int value;

    public SpinnerItem(String title, int value) {
        this.title = title;
        this.value = value;
    }

    public String getTitle() {
        return title;
    }

    public int getValue() {
        return value;
    }
}
