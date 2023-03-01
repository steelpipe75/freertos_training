package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"os"
)

type MyTraceReadJson struct {
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

type MyTrace struct {
	Initialize          Initialize
	TaskSwitchedInSlice []TaskSwitchedIn
}

func ReadJson(filename string) MyTrace {
	var myTraceObj MyTrace
	fp, err := os.Open(filename)
	if err != nil {
		panic(err)
	}
	defer fp.Close()
	scanner := bufio.NewScanner(fp)
	for scanner.Scan() {
		jsonObj := MyTraceReadJson{Type: "", Data: "0"}
		jsonStr := scanner.Text()
		jsonByteArray := []byte(jsonStr)
		if err := json.Unmarshal(jsonByteArray, &jsonObj); err != nil {
			panic(err)
		}
		// fmt.Println(jsonStr)
		if jsonObj.Type == "Initialize" {
			initializeByteArray := []byte(jsonObj.Data)
			initializeObj := Initialize{Timescale: -1.0}
			if err := json.Unmarshal(initializeByteArray, &initializeObj); err != nil {
				panic(err)
			}
			myTraceObj.Initialize = initializeObj
			// fmt.Printf("%+v\r\n", initializeObj)
		} else if jsonObj.Type == "TaskSwitchedIn" {
			taskSwitchedInByteArray := []byte(jsonObj.Data)
			taskSwitchedInObj := TaskSwitchedIn{Tick: 0, In: "", Out: ""}
			if err := json.Unmarshal(taskSwitchedInByteArray, &taskSwitchedInObj); err != nil {
				panic(err)
			}
			myTraceObj.TaskSwitchedInSlice = append(myTraceObj.TaskSwitchedInSlice, taskSwitchedInObj)
			// fmt.Printf("%+v\r\n", taskSwitchedInObj)
		}
	}

	if err = scanner.Err(); err != nil {
		panic(err)
	}

	return myTraceObj
}

func WriteVCD(filename string, myTraceObj MyTrace) {
	fmt.Printf("%+v\r\n", myTraceObj.Initialize)
	for _, v := range myTraceObj.TaskSwitchedInSlice {
		fmt.Printf("%+v\r\n", v)
	}
}

func main() {
	json_filename := "example.log"
	myTraceObj := ReadJson(json_filename)
	vcd_filename := "example.vcd"
	WriteVCD(vcd_filename, myTraceObj)
}
