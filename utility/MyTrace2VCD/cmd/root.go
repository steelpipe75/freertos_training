/*
Copyright © 2023 Nishizaki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
package cmd

import (
	"bufio"
	"encoding/json"
	"fmt"
	"log"
	"os"
	"strings"

	"github.com/elamre/vcd"
	"github.com/spf13/cobra"
)

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "MyTrace2VCD",
	Short: "Convert from MyTraceLogData to VCD",
	Long:  `Convert from MyTraceLogData to VCD`,
	Run: func(cmd *cobra.Command, args []string) {
		input, err := cmd.Flags().GetString("input")
		if err != nil {
			log.Fatal(err)
		}
		output, err := cmd.Flags().GetString("output")
		if err != nil {
			log.Fatal(err)
		}
		myTrace2VCD(input, output)
	},
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main.main(). It only needs to happen once to the rootCmd.
func Execute() {
	err := rootCmd.Execute()
	if err != nil {
		os.Exit(1)
	}
}

func init() {
	// Here you will define your flags and configuration settings.
	// Cobra supports persistent flags, which, if defined here,
	// will be global for your application.

	// rootCmd.PersistentFlags().StringVar(&cfgFile, "config", "", "config file (default is $HOME/.MyTrace2VCD.yaml)")

	// Cobra also supports local flags, which will only run
	// when this action is called directly.
	// rootCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
	rootCmd.Flags().StringP("input", "i", "example.log", "Help message for input")
	rootCmd.Flags().StringP("output", "o", "example.vcd", "Help message for output")
}

type MyTraceReadJson struct {
	Type string
	Data string
}

type Initialize struct {
	Timescale float64
}

type TaskSwitch struct {
	Tick int
	In   string
	Out  string
}

type MyTrace struct {
	Initialize      Initialize
	TaskSwitchSlice []TaskSwitch
}

func ReadLog(filename string) MyTrace {
	fmt.Printf("Read log file (%s) start.\r\n", filename)
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
		} else if jsonObj.Type == "TaskSwitch" {
			taskSwitchByteArray := []byte(jsonObj.Data)
			taskSwitchObj := TaskSwitch{Tick: 0, In: "", Out: ""}
			if err := json.Unmarshal(taskSwitchByteArray, &taskSwitchObj); err != nil {
				panic(err)
			}
			taskSwitchObj.In = strings.Replace(taskSwitchObj.In, " ", "_", -1)
			taskSwitchObj.Out = strings.Replace(taskSwitchObj.Out, " ", "_", -1)
			myTraceObj.TaskSwitchSlice = append(myTraceObj.TaskSwitchSlice, taskSwitchObj)
			// fmt.Printf("%+v\r\n", taskSwitchObj)
		}
	}

	if err = scanner.Err(); err != nil {
		panic(err)
	}

	fmt.Printf("Read log file (%s) done.\r\n", filename)
	return myTraceObj
}

func WriteVCD(filename string, myTraceObj MyTrace) {
	fmt.Printf("Write VCD file (%s) start.\r\n", filename)
	// fmt.Printf("%+v\r\n", myTraceObj.Initialize)
	timescaleStr := fmt.Sprintf("%vms", myTraceObj.Initialize.Timescale)
	// fmt.Printf("%+v\r\n", timescaleStr)

	taskNameMap := make(map[string]bool)
	taskNameSlice := []string{}
	for _, v := range myTraceObj.TaskSwitchSlice {
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
	// fmt.Printf("%+v\r\n", taskNameMap)
	// fmt.Printf("%+v\r\n", taskNameSlice)

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
	_, e = writer.RegisterVariableList("TaskSwitch.TaskRunStatets", vcdVariableSlice)
	vcdVariable := vcd.NewVariable("RuningTaskName", "string", 1)
	_, e = writer.RegisterVariables("TaskSwitch.RuningTaskName", vcdVariable)

	for _, v := range myTraceObj.TaskSwitchSlice {
		if v.Out != "" {
			e = writer.SetValue(uint64(v.Tick), "0", v.Out)
			if e != nil {
				panic(e)
			}
		}
		if v.In != "" {
			e = writer.SetValue(uint64(v.Tick), "1", v.In)
			e = writer.SetValue(uint64(v.Tick), v.In, "RuningTaskName")
			if e != nil {
				panic(e)
			}
		}
	}

	fmt.Printf("Write VCD file (%s) done.\r\n", filename)
}

func myTrace2VCD(json_filename string, vcd_filename string) {
	myTraceObj := ReadLog(json_filename)
	WriteVCD(vcd_filename, myTraceObj)
}
