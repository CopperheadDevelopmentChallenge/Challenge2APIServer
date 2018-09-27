package main

type User struct {
	Body  string `json:"body"`
	Email string `json:"email"`
	ID    int    `json:"id"`
	Name  string `json:"name"`
}
