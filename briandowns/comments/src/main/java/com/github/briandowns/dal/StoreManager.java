package com.github.briandowns.dal;

import com.google.gson.Gson;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.List;

public class StoreManager {


    private Integer lastIdx;
    private Store store;

    public StoreManager(String file) {
        BufferedReader bufferedReader = null;
        try {
            bufferedReader = new BufferedReader(new FileReader(file));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return;
        }
        Gson gson = new Gson();
        this.store = gson.fromJson(bufferedReader, Store.class);

    }

    public List<Comment> getComments() {
        return this.store.getData();
    }

    public List<Comment> getComments(Integer size, Integer from) {
        return null;
    }

    public Comment getComment(Integer id) {
        Comment comment = null;
        return this.store.getData().stream()
                .filter(c -> c.getId()
                .equals(id)).findFirst()
                .orElse(comment);
    }

    public Comment updateComment(Integer id, Comment comment) {
        return null;
    }

    public Comment deleteComment(Integer id, Comment comment) {
        return null;
    }

    public Comment newComment(Comment comment) {
        return null;
    }
}
