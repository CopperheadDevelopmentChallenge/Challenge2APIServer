package com.github.briandowns;

import com.github.briandowns.dal.Comment;
import com.github.briandowns.dal.StoreManager;
import com.google.gson.Gson;

import static spark.Spark.*;

public class Main {

    Gson gson = new Gson();

    public static void main( String[] args ) {

        StoreManager storeManager = new StoreManager("../../../../../data.json");

        get("/comments", (request, response) -> {
            if (request.params().containsKey("size") && request.params().containsKey("from")) {
                Comment comment = storeManager.
            }
            response.status(200);
            return new Comment(), gson::toJson;
        });
        get("/comments/:id", (request, response) -> {
            response.status(200);
        });
        post("/comments", (request, response) -> {
            response.status(201);
        });
        delete("/comments/:id", (request, response) -> {
            response.status(202);
        });
        put("/comments:id", (request, response) -> {
            response.status(202);
        });
    }
}
