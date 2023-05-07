package cn.yuebo.hello240.adapter;

import android.content.Context;
import android.widget.TextView;

import java.util.List;

import cn.yuebo.hello240.R;

/**
 * @author yuebo
 * @date 2023/3/24
 */
public class SpinnerAdapter extends SimpleAdapter<SpinnerItem> {

    public SpinnerAdapter(Context context, List<SpinnerItem> dataList) {
        super(context, dataList);
    }

    @Override
    public int getLayoutId() {
        return R.layout.item_screen_size;
    }

    @Override
    public void bindData(ViewHolder viewHolder, SpinnerItem data, int position) {
        TextView tvTitle = viewHolder.getView(R.id.tvTitle);
        tvTitle.setText(data.getTitle());
    }

}
