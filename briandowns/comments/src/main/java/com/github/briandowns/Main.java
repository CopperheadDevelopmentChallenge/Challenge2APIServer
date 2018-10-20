package com.github.briandowns;

import com.github.briandowns.dal.StoreManager;

import static spark.Spark.*;

public class Main {

    public static void main( String[] args ) {

        StoreManager storeManager = new StoreManager("../../../../../data.json");

        get("/comments", (request, response) -> {
            response.status(200);
        }, new JsonTransformer());
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
