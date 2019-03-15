package main

import "github.com/go-redis/redis"
import "fmt"
import "time"
import "github.com/shirou/gopsutil/mem"

func ExampleNewClient() {
	client := redis.NewClient(&redis.Options{
		Addr:     "localhost:6379",
		Password: "", // no password set
		DB:       0,  // use default DB
	})

	pong, err := client.Ping().Result()
	fmt.Println(pong, err)
	// Output: PONG <nil>

	for i := 1; i < 500000; i++ {
		now := time.Now()
		for j := 1; j < 1024*1024; j++ {
			key := fmt.Sprintf("key%d-%d", i, j)
			value := fmt.Sprintf("value%d-%d", i, j)
			err = client.Set(key, value, 0).Err()
			if err != nil {
				panic(err)
			}

			_, err := client.Get(key).Result()
			if err == redis.Nil {
				fmt.Println("key does not exist")
			} else if err != nil {
				panic(err)
			} 
		}

		then := time.Now()
		diff := then.Sub(now)

		v,_ := mem.VirtualMemory()
		fmt.Println("Elapsed", diff.Seconds(), 
			    "Mem tatal:%v, free:%v, usedPercent:%f%%", v.Total, v.Free, v.UsedPercent)
	}
}

func main() {
	ExampleNewClient()
}

