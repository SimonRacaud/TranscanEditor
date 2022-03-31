package main

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"github.com/AutoScanTrad/page"
)

type App struct {
	app    fyne.App
	window fyne.Window
	viewer page.Viewer
}

func (app *App) Init() {
	app.window = app.app.NewWindow("Title")
	app.window.SetMaster()
	//window.SetIcon(resourceIconPng)

	//	app.SetIcon(resourceIconPng)
	//start_page := page.NewStartPage()
	//start_page.Init(app)
	app.window.SetContent(app.viewer.LoadUI())
	app.viewer.LoadFolder("../dataset/debug")

	app.window.Resize(fyne.NewSize(800, 600))
	app.window.ShowAndRun()
}

func NewApp() App {
	return App{
		app: app.New(),
	}
}
