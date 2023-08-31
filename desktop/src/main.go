package main

import (
	"os"

	"github.com/Shopify/go-lua"
)

func backend_init(l *lua.State) {

}

func main() {
	l := lua.NewState()
	lua.OpenLibraries(l)

	if err := lua.DoFile(l, "libs/boundless_fusion/main.lua"); err != nil {
		panic(err)
	}

	backend_init(l)

	lua.DoFile(l, "game/main.lua")

	os.Exit(0)
}
