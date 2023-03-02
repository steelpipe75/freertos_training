package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"os"
	"strings"

	"github.com/elamre/vcd"
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
	// fmt.Printf("%+v\r\n", myTraceObj.Initialize)
	timescaleStr := fmt.Sprintf("%vms", myTraceObj.Initialize.Timescale)
	// fmt.Printf("%+v\r\n", timescaleStr)

	taskNameMap := make(map[string]bool)
	taskNameSlice := []string{}
	for _, v := range myTraceObj.TaskSwitchedInSlice {
		v.In = strings.Replace(v.In, " ", "_", -1)
		v.Out = strings.Replace(v.Out, " ", "_", -1)
		if v.In != "" {
			if !taskNameMap[v.In] {
				taskNameMap[v.In] = true
				taskNameSlice = append(taskNameSlice, v.In)
			}
		}
		if v.Out != "" {
			if !taskNameMap[v.Out] {
				taskNameMap[v.Out] = true
				taskNameSlice = append(taskNameSlice, v.Out)
			}
		}
		// fmt.Printf("%+v\r\n", v)
	}
	fmt.Printf("%+v\r\n", taskNameMap)
	fmt.Printf("%+v\r\n", taskNameSlice)

	vcdVariableSlice := []vcd.VcdDataType{}
	for _, t := range taskNameSlice {
		vcdVariable := vcd.NewVariable(t, "wire", 1)
		vcdVariableSlice = append(vcdVariableSlice, vcdVariable)
	}

	writer, e := vcd.New(filename, timescaleStr)
	if e != nil {
		panic(e)
	}
	defer writer.Close()
	_, e = writer.RegisterVariables("TaskSwitchedIn", vcdVariableSlice...)

	for _, v := range myTraceObj.TaskSwitchedInSlice {
		v.In = strings.Replace(v.In, " ", "_", -1)
		v.Out = strings.Replace(v.Out, " ", "_", -1)
		if v.Out != "" {
			e = writer.SetValue(uint64(v.Tick), "0", v.Out)
			if e != nil {
				panic(e)
			}
		}
		if v.In != "" {
			e = writer.SetValue(uint64(v.Tick), "1", v.In)
			if e != nil {
				panic(e)
			}
		}
	}
}

func main() {
	json_filename := "example.log"
	myTraceObj := ReadJson(json_filename)
	vcd_filename := "example.vcd"
	WriteVCD(vcd_filename, myTraceObj)
}
