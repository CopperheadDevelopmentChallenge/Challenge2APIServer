package main

import (
	"strconv"
	"testing"
)

func BenchmarkGetAllDataMarshal(b *testing.B) {
	db, _ = newDatabase()

	b.ReportAllocs()
	b.ResetTimer()

	for i := 0; i < b.N; i++ {
		db.commentsByRange(-1, -1)
	}
}

func BenchmarkAtoiMapAccess(b *testing.B) {
	aKey := "100"

	testMap := make(map[int]int)
	testMap[100] = 100

	b.ReportAllocs()
	b.ResetTimer()

	for i := 0; i < b.N; i++ {
		k, _ := strconv.Atoi(aKey)
		if _, ok := testMap[k]; ok {
			// nothing
		}
		testMap[204] = 204
	}
}

func BenchmarkNoConversionKeyAccess(b *testing.B) {
	aKey := 100

	testMap := make(map[int]int)
	testMap[100] = 100

	b.ReportAllocs()
	b.ResetTimer()

	for i := 0; i < b.N; i++ {
		if _, ok := testMap[aKey]; ok {
			// nothing
		}
		testMap[204] = 204
	}
}

func BenchmarkStringKeyAccess(b *testing.B) {
	aKey := "100"

	testMap := make(map[string]int)
	testMap[aKey] = 100

	b.ReportAllocs()
	b.ResetTimer()

	for i := 0; i < b.N; i++ {
		if _, ok := testMap[aKey]; ok {
			// nothing
		}

		testMap["204"] = 204
	}
}
