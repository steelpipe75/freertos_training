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

	"github.com/spf13/cobra"
)

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "MyTrace2TimingDiagram",
	Short: "Convert from MyTraceLogData to PlantUML Timing Diagram",
	Long:  `Convert from MyTraceLogData to PlantUML Timing Diagram`,
	Run: func(cmd *cobra.Command, args []string) {
		input, err := cmd.Flags().GetString("input")
		if err != nil {
			log.Fatal(err)
		}
		output, err := cmd.Flags().GetString("output")
		if err != nil {
			log.Fatal(err)
		}
		MyTrace2TimingDiagram(input, output)
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

	// rootCmd.PersistentFlags().StringVar(&cfgFile, "config", "", "config file (default is $HOME/.MyTrace2TimingDiagram.yaml)")

	// Cobra also supports local flags, which will only run
	// when this action is called directly.
	// rootCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
	rootCmd.Flags().StringP("input", "i", "example.log", "Help message for input")
	rootCmd.Flags().StringP("output", "o", "example.puml", "Help message for output")
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

func WriteTimingDiagram(filename string, myTraceObj MyTrace) {
	fmt.Printf("Write TimingDiagram file (%s) start.\r\n", filename)
	// todo
	fmt.Printf("Write TimingDiagram file (%s) done.\r\n", filename)
}

func MyTrace2TimingDiagram(json_filename string, td_filename string) {
	myTraceObj := ReadLog(json_filename)
	WriteTimingDiagram(td_filename, myTraceObj)
}
