package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"sync"
)

type Store struct {
	mu       *sync.Mutex
	file     string
	Comments []*Comment `json:"data"`
}

func (s *Store) SetDataFile(file string) {
	s.mu = &sync.Mutex{}
	s.file = file
}

func (s Store) FindByID(id int) *Comment {
	for _, comment := range s.Comments {
		if id == comment.ID {
			return comment
		}
	}
	return nil
}

func (s *Store) Create(c *Comment) *Comment {
	s.mu.Lock()
	defer s.mu.Unlock()
	c.ID = len(s.Comments) + 1
	s.Comments = append(s.Comments, c)
	return c
}

func (s Store) Close() {
	fmt.Println("closing datastore and writing to file ", s.file)
	defer fmt.Println("datastore closed successfully")
	b, err := json.Marshal(s.Comments)
	if err != nil {
		fmt.Errorf("could not marshal the comments: %v\n", err)
	}
	err = ioutil.WriteFile(s.file, b, 0644)
	if err != nil {
		fmt.Errorf("could not write to file %s: %v\n", s.file, err)
	}
}
