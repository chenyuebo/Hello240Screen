package cn.yuebo.hello240.adapter;

import android.content.Context;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;

import androidx.annotation.LayoutRes;

import java.util.List;

/**
 * @author yuebo
 * @date 2023/3/24
 */
public abstract class SimpleAdapter<T> extends BaseAdapter {

    private Context context;
    private List<T> dataList;

    public SimpleAdapter(Context context, List<T> dataList) {
        this.context = context;
        this.dataList = dataList;
    }

    @Override
    public int getCount() {
        return dataList == null ? 0 : dataList.size();
    }

    @Override
    public T getItem(int position) {
        return dataList.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder viewHolder = null;
        if (convertView == null) {
            convertView = LayoutInflater.from(context).inflate(getLayoutId(), parent, false);
            viewHolder = new ViewHolder(convertView);
            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
        }
        T data = (T) getItem(position);
        bindData(viewHolder, data, position);
        return convertView;
    }

    @LayoutRes
    public abstract int getLayoutId();

    public abstract void bindData(ViewHolder viewHolder, T data, int position);

    public static class ViewHolder {
        private View itemView;
        private SparseArray<View> views = new SparseArray<>();

        public ViewHolder(View itemView) {
            this.itemView = itemView;
        }

        public <T> T getView(int id) {
            View view = views.get(id);
            if (view == null) {
                view = itemView.findViewById(id);
            }
            if (view != null) {
                return (T) view;
            }
            return null;
        }
    }
}
