package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"net/http"
	"os"
	"strconv"
	"sync"

	"github.com/gorilla/mux"
)

type comment struct {
	ID    int    `json:"id"`
	Name  string `json:"name"`
	Email string `json:"email"`
	Body  string `json:"body"`
}

type store struct {
	mu      *sync.Mutex
	lastIdx int
	Data    []*comment `json:"data"`
}

func newStore(data string) (*store, error) {
	f, err := os.Open(data)
	if err != nil {
		return nil, err
	}
	var s store
	if err := json.NewDecoder(f).Decode(&s); err != nil {
		return nil, err
	}
	s.mu = &sync.Mutex{}
	s.lastIdx = len(s.Data)
	return &s, nil
}

func (s *store) commentByID(id int) *comment {
	s.mu.Lock()
	defer s.mu.Unlock()
	for _, i := range s.Data {
		if i.ID == id {
			return i
		}
	}
	return nil
}

func (s *store) deleteComment(id int) bool {
	s.mu.Lock()
	defer s.mu.Unlock()
	for _, i := range s.Data {
		if i.ID == id {
			i.ID = -1
			return true
		}
	}
	return false
}

func (s *store) updateComment(id int, c *comment) bool {
	s.mu.Lock()
	defer s.mu.Unlock()
	for _, i := range s.Data {
		if i.ID == id {
			i.Name = c.Name
			i.Email = c.Email
			i.Body = c.Body
			return true
		}
	}
	return false
}

func (s *store) newComment(c *comment) *comment {
	s.mu.Lock()
	defer s.mu.Unlock()
	c.ID = s.lastIdx + 1
	s.lastIdx++
	s.Data = append(s.Data, c)
	return c
}

func (s *store) commentsFromRange(size, from int) []*comment {
	var comments []*comment
	for i := from; i < size; {
		if s.Data[i].ID > 0 {
			comments = append(comments, s.Data[i])
			i++
		}
	}
	return comments
}

type api struct {
	ds *store
}

func (a *api) allComments(w http.ResponseWriter, r *http.Request) {
	params := r.URL.Query()
	w.Header().Set("Content-Type", "application/json")
	switch {
	case len(params) > 0:
		s := params.Get("size")
		f := params.Get("from")
		if s == "" || f == "" {
			break
		}
		size, err := strconv.Atoi(s)
		if err != nil {
			log.Println(err)
			http.Error(w, "internal server error", http.StatusInternalServerError)
			return
		}
		from, err := strconv.Atoi(f)
		if err != nil {
			log.Println(err)
			http.Error(w, "internal server error", http.StatusInternalServerError)
			return
		}
		if from < 1 {
			br := errors.New("bad request: from needs to be larger than 0")
			log.Println(br)
			http.Error(w, br.Error(), http.StatusBadRequest)
			return
		}
		c := a.ds.commentsFromRange(size, from-1)
		w.WriteHeader(http.StatusOK)
		if err := json.NewEncoder(w).Encode(&c); err != nil {
			log.Println(err)
			return
		}
		return
	default:
		w.WriteHeader(http.StatusOK)
		if err := json.NewEncoder(w).Encode(&a.ds); err != nil {
			log.Println(err)
			return
		}
		return
	}
}

func (a *api) comment(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	id := vars["id"]
	cid, err := strconv.Atoi(id)
	if err != nil {
		log.Println(err)
		http.Error(w, "internal server error", http.StatusInternalServerError)
		return
	}
	c := a.ds.commentByID(cid)
	if c == nil {
		w.WriteHeader(http.StatusOK)
		return
	}
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	if err := json.NewEncoder(w).Encode(&c); err != nil {
		log.Println(err)
		return
	}
}
func (a *api) updateComment(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	id := vars["id"]
	cid, err := strconv.Atoi(id)
	if err != nil {
		log.Println(err)
		http.Error(w, "internal server error", http.StatusInternalServerError)
		return
	}
	var c comment
	if err := json.NewDecoder(r.Body).Decode(&c); err != nil {
		log.Println(err)
		http.Error(w, "internal server error", http.StatusInternalServerError)
		return
	}
	defer r.Body.Close()
	ok := a.ds.updateComment(cid, &c)
	if !ok {
		w.WriteHeader(http.StatusAccepted)
		return
	}
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusAccepted)
	if err := json.NewEncoder(w).Encode(&c); err != nil {
		log.Println(err)
		return
	}
}

func (a *api) deleteComment(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	id := vars["id"]
	cid, err := strconv.Atoi(id)
	if err != nil {
		log.Println(err)
		http.Error(w, "internal server error", http.StatusInternalServerError)
		return
	}
	c := a.ds.commentByID(cid)
	if c == nil {
		w.WriteHeader(http.StatusAccepted)
		return
	}
	n := *c
	if ok := a.ds.deleteComment(cid); !ok {
		w.WriteHeader(http.StatusAccepted)
		return
	}
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	if err := json.NewEncoder(w).Encode(&n); err != nil {
		log.Println(err)
		return
	}
}

func (a *api) addComment(w http.ResponseWriter, r *http.Request) {
	var c comment
	if err := json.NewDecoder(r.Body).Decode(&c); err != nil {
		log.Println(err)
		http.Error(w, "internal server error", http.StatusInternalServerError)
		return
	}
	defer r.Body.Close()
	n := a.ds.newComment(&c)
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusCreated)
	if err := json.NewEncoder(w).Encode(n); err != nil {
		log.Println(err)
		return
	}
}

const commentsPath = "/comments"
const commentsByIDPath = commentsPath + "/{id}"

func main() {
	ds, err := newStore("../../data.json")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	a := api{ds}
	r := mux.NewRouter()
	r.HandleFunc(commentsPath, a.allComments).Methods(http.MethodGet)
	r.HandleFunc(commentsPath, a.addComment).Methods(http.MethodPost)
	r.HandleFunc(commentsByIDPath, a.comment).Methods(http.MethodGet)
	r.HandleFunc(commentsByIDPath, a.updateComment).Methods(http.MethodPut)
	r.HandleFunc(commentsByIDPath, a.deleteComment).Methods(http.MethodDelete)
	if err := http.ListenAndServe(":8080", r); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}
