package main

import (
	"encoding/json"
	"io/ioutil"
	"sync"
)

type Store struct {
	mu       *sync.RWMutex
	file     string
	Comments []*Comment `json:"data"`
}

func (s Store) Filename() string {
	return s.file
}

func (s *Store) SetDataFile(file string) {
	s.mu = &sync.RWMutex{}
	s.file = file
}

func (s Store) Find(offset, pageSize int) []*Comment {
	s.mu.RLock()
	defer s.mu.RUnlock()
	if offset == 0 && pageSize == 0 {
		return s.Comments
	}
	results := s.Comments[offset:pageSize]
	return results
}

func (s Store) FindByID(id int) *Comment {
	s.mu.RLock()
	defer s.mu.RUnlock()
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
	return c
}

func (s *Store) Update(id int, c *Comment) *Comment {
	c.ID = id
	return s.Save(c)
}

func (s *Store) Save(c *Comment) *Comment {
	s.mu.RLock()
	defer s.mu.RUnlock()
	for _, comment := range s.Comments {
		if c.ID == comment.ID {
			comment.Body = c.Body
			comment.Email = c.Email
			comment.Name = c.Name
			return comment
		}
	}
	return nil
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
