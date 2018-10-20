package com.github.briandowns.dal;

import java.util.List;

import com.google.gson.annotations.SerializedName;


public class Store {

    @SerializedName("data")
    private List<Comment> data;

    public List<Comment> getData() {
        return data;
    }

    public void setData(List<Comment> data) {
        this.data = data;
    }

    @Override
    public String toString() {
        return "Store{" +
                ", data=" + data +
                '}';
    }
}
