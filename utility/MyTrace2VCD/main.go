package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"os"
)

type MyTrace struct {
	Type string
	Data string
}

type Initialize struct {
	Timescale float64
}

type TaskSwitchedIn struct {
	Tick int
	In   string
	Out  string
}

func ReadJson(filename string) {
	fp, err := os.Open(filename)
	if err != nil {
		panic(err)
	}
	defer fp.Close()
	scanner := bufio.NewScanner(fp)
	for scanner.Scan() {
		jsonObj := MyTrace{Type: "", Data: "0"}
		jsonStr := scanner.Text()
		jsonByteArray := []byte(jsonStr)
		if err := json.Unmarshal(jsonByteArray, &jsonObj); err != nil {
			panic(err)
		}
		fmt.Println(jsonStr)
		if jsonObj.Type == "Initialize" {
			initializeByteArray := []byte(jsonObj.Data)
			initializeObj := Initialize{Timescale: -1.0}
			if err := json.Unmarshal(initializeByteArray, &initializeObj); err != nil {
				panic(err)
			}
			fmt.Printf("%+v\r\n", initializeObj)
		} else if jsonObj.Type == "TaskSwitchedIn" {
			taskSwitchedInByteArray := []byte(jsonObj.Data)
			taskSwitchedInObj := TaskSwitchedIn{Tick: 0, In: "", Out: ""}
			if err := json.Unmarshal(taskSwitchedInByteArray, &taskSwitchedInObj); err != nil {
				panic(err)
			}
			fmt.Printf("%+v\r\n", taskSwitchedInObj)
		}
	}

	if err = scanner.Err(); err != nil {
		panic(err)
	}
}

func main() {
	json_filename := "example.log"
	ReadJson(json_filename)
}
