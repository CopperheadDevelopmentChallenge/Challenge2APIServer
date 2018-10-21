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

func (s Store) Filename() string {
	return s.file
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
	c.ID = s.Comments[len(s.Comments)-1].ID + 1
	s.Comments = append(s.Comments, c)
	fmt.Println(len(s.Comments))
	return c
}

func (s Store) Close() error {
	b, err := json.Marshal(s.Comments)
	if err != nil {
		return err
	}
	err = ioutil.WriteFile(s.file, b, 0644)
	if err != nil {
		return err
	}
	return nil
}
